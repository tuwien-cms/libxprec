/* Trigonometric functions to quad precision.
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "ddouble.h"
#include "taylor.h"

static DDouble sin_kernel(DDouble x)
{
    // We need this to go out to pi/4 ~= 0.785
    // Convergence of the Taylor approx to 2e-32
    assert(greater_in_magnitude(0.94, x.hi()));

    // Taylor series of the sin around 0
    DDouble xsq = -x * x;
    DDouble r = x;
    DDouble xpow = x;
    for (int i = 3; i <= 27; i += 2) {
        xpow *= xsq;
        r += reciprocal_factorial(i) * xpow;
    }
    return r;
}

static DDouble cos_kernel(DDouble x)
{
    // We need this to go out to pi/4 ~= 0.785
    // Convergence of the Taylor approx to 2e-32
    assert(greater_in_magnitude(0.83, x.hi()));

    // Taylor series of the cos around 0
    DDouble xsq = -x * x;
    DDouble r = 1.0;
    DDouble xpow = 1.0;
    for (int i = 2; i <= 26; i += 2) {
        xpow *= xsq;
        r += reciprocal_factorial(i) * xpow;
    }
    return r;
}

static DDouble remainder_pi2(DDouble x, int &sector)
{
    // This reduction has to be done quite carefully, because of the
    // remainder.
    static const DDouble PI2(1.5707963267948966, 6.123233995736766e-17);
    static const DDouble OVER_PI2(0.6366197723675814, -3.935735335036497e-17);

    DDouble n = OVER_PI2 * x;
    if (fabs(n.hi()) < 0.5) {
        sector = 0;
        return x;
    }

    // This is a problem for very large revolution count, but there we
    // have a problem anyway.
    n = round(n);
    int64_t n_int = n.as<int64_t>();
    sector = n_int % 4;
    if (sector < 0)
        sector += 4;
    return x - PI2 * n;
}

DDouble sin_sector(DDouble x, int sector)
{
    assert(sector < 4);
    assert(fabs(x.hi()) <= M_PI/4);

    switch (sector) {
    case 0:
        return sin_kernel(x);
    case 1:
        // use sin(x) = cos(x - pi/2)
        return cos_kernel(x);
    case 2:
        // use sin(x) = -sin(x - pi)
        return -sin_kernel(x);
    default:
        return -cos_kernel(x);
    }
}

DDouble sin(DDouble x)
{
    int sector;
    x = remainder_pi2(x, sector);
    return sin_sector(x, sector);
}

DDouble cos(DDouble x)
{
    int sector;
    x = remainder_pi2(x, sector);
    return sin_sector(x, (sector + 1) % 4);
}
