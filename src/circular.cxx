/* Trigonometric functions to quad precision.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include "taylor.h"

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

static const DDouble PI(3.141592653589793, 1.2246467991473532e-16);
static const DDouble PI_2(1.5707963267948966, 6.123233995736766e-17);
static const DDouble PI_4(0.7853981633974483, 3.061616997868383e-17);

static DDouble sin_kernel(DDouble x)
{
    // We need this to go out to pi/4 ~= 0.785
    // Convergence of the Taylor approx to 2e-32
    assert(greater_in_magnitude(0.94, x));

    // Taylor series of the sin around 0
    DDouble xsq = -x * x;
    DDouble r = x;
    DDouble xpow = x;
    for (int i = 3; i <= 27; i += 2) {
        xpow *= xsq;
        r = r.add_small(reciprocal_factorial(i) * xpow);
    }
    return r;
}

static DDouble cos_kernel(DDouble x)
{
    // We need this to go out to pi/4 ~= 0.785
    // Convergence of the Taylor approx to 2e-32
    assert(greater_in_magnitude(0.83, x));

    // Taylor series of the cos around 0
    DDouble xsq = -x * x;
    DDouble r = 1.0;
    DDouble xpow = xsq;
    r = r.add_small(PowerOfTwo(-1) * xpow);
    for (int i = 4; i <= 26; i += 2) {
        xpow *= xsq;
        r = r.add_small(reciprocal_factorial(i) * xpow);
    }
    return r;
}

static DDouble remainder_pi2(DDouble x, int &sector)
{
    // This reduction has to be done quite carefully, because of the
    // remainder.
    DDouble n = x / PI_2;
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
    return x - PI_2 * n;
}

static DDouble sin_sector(DDouble x, int sector)
{
    assert(sector >= 0 && sector < 4);
    assert(fabs(x.hi()) <= nextafter(PI_4.hi(), 1));

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

XPREC_API_EXPORT
DDouble sin(DDouble x)
{
    int sector;
    x = remainder_pi2(x, sector);
    return sin_sector(x, sector);
}

XPREC_API_EXPORT
DDouble cos(DDouble x)
{
    // For small values, we shall use the cosine directly
    if (fabs(x.hi()) < PI_4.hi())
        return cos_kernel(x);

    // Otherwise, use common code.
    int sector;
    x = remainder_pi2(x, sector);
    return sin_sector(x, (sector + 1) % 4);
}

XPREC_API_EXPORT
void sincos(DDouble x, DDouble &s, DDouble &c)
{
    // XXX This should be improved
    s = sin(x);
    c = cos(x);
}

XPREC_API_EXPORT
DDouble tan(DDouble x)
{
    DDouble s, c;
    sincos(x, s, c);
    return s / c;
}

XPREC_API_EXPORT
DDouble asin(DDouble x)
{
    // Compute a approximation to double precision
    DDouble y0 = asin(x.hi());
    if (!isfinite(y0))
        return y0;

    // This is where Taylor fails
    if (fabs(x) == 1.0) {
        static const DDouble PI_2(1.5707963267948966, 6.123233995736766e-17);
        return copysign(PI_2, x);
    }

    // Perform Taylor expansion:
    //
    //    asin(x) = asin(x0) + (x - x0) / sqrt(1 - x0**2)
    //            = y0 + (x - sin(y0)) / cos(y0)
    //
    DDouble x0, w;
    sincos(y0, x0, w);

    DDouble y = y0 + (x - x0) / w;
    return y;
}

XPREC_API_EXPORT
DDouble acos(DDouble x)
{
    // Compute a approximation to double precision
    DDouble y0 = acos(x.hi());
    if (!isfinite(y0))
        return y0;

    // This is where Taylor fails
    if (x == 1.0)
        return 0.0;
    if (x == -1.0) {
        return PI;
    }

    // Perform Taylor expansion:
    //
    //    acos(x) = acos(x0) - (x - x0) / sqrt(1 - x0**2)
    //            = y0 - (x - cos(y0)) / sin(y0)
    //
    DDouble x0, w, diff;

    sincos(y0, w, x0);
    diff = (x0 - x) / w;
    y0 += diff;
    return y0;
}

XPREC_API_EXPORT
DDouble atan(DDouble x)
{
    // For large values, use reflection formula
    if (fabs(x.hi()) > 1.0) {
        DDouble y = copysign(PI_2, x);
        if (isfinite(x))
            y -= atan(reciprocal(x));
        return y;
    }

    // Again use Taylor expansion
    DDouble y0 = atan(x.hi());
    if (!isfinite(y0))
        return y0;

    DDouble s, c, x0;

    sincos(y0, s, c);
    x0 = s / c;
    y0 += (x - x0) * c * c;

    return y0;
}

XPREC_API_EXPORT
DDouble atan2(DDouble y, DDouble x)
{
    // Special values
    if (isnan(x) || isnan(y))
        return NAN;
    if (iszero(y))
        return x.hi() >= 0 ? 0.0 : PI;
    if (iszero(x))
        return copysign(PI_2, y);

    DDouble res = atan(y / x);
    if (x.hi() < 0)
        res = copysign(PI, y).add_small(res);
    return res;
}
