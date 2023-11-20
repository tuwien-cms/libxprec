/* Mathematical functions to quad precision.
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "ddouble.h"
#include <cstring>

DDouble sqrt(DDouble a)
{
    // From: Karp, High Precision Division and Square Root, 1993
    double sqrt_hi = sqrt(a.hi());
    if (a.hi() <= 0)
        return sqrt_hi;

    double x = 1.0 / sqrt_hi;
    ExDouble ax = a.hi() * x;
    DDouble ax_squared = ax * ax;
    double diff = (a - ax_squared).hi() * x * 0.5;
    return ax + diff;
}

DDouble hypot(DDouble x, DDouble y)
{
    // Make sure that the values are ordered by magnitude
    if (!greater_in_magnitude(x, y)) {
        swap(x, y);
    }

    // Check for infinities
    if (!std::isfinite(x.hi())) {
        return std::isnan(y.hi()) ? y : x;
    }

    // Splits the range in half
    static const PowerOfTwo LARGE(std::numeric_limits<double>::max_exponent / 2);
    static const PowerOfTwo SMALL  = PowerOfTwo(0) / LARGE;

    if (greater_in_magnitude(x.hi(), LARGE)) {
        // For large values, scale down to avoid overflow
        x *= SMALL;
        y *= SMALL;
        return sqrt((x * x).add_small(y * y)) * LARGE;
    } else if (greater_in_magnitude(SMALL, x.hi())) {
        // For small values, scale up to avoid underflow
        x *= LARGE;
        y *= LARGE;
        return sqrt((x * x).add_small(y * y)) * SMALL;
    } else {
        // We're fine
        return sqrt((x * x).add_small(y * y));
    }
}

DDouble pow(DDouble x, int n)
{
    if (n < 0) {
        DDouble res = pow(x, -n);
        return reciprocal(res);
    }
    if (n == 0) {
        // XXX handle nan's etc.
        return DDouble(1.0);
    }

    // Get first non-zero power
    while ((n & 1) == 0) {
        n >>= 1;
        x *= x;
    }

    // Multiply and square
    DDouble res = x;
    while (n >>= 1) {
        x *= x;
        if ((n & 1) == 1)
            res *= x;
    }
    return res;
}

static DDouble expm1_taylor(DDouble x)
{
    // For the absolute error to be < epsilon/2
    assert(greater_in_magnitude(1.1815e-3, x.hi()));

    // Taylor series up to order 8
    //  1 + x ( 1 + x/2 ( 1 + x/3 ( ... (1 + x/8)...)
    DDouble r;
    r = x * PowerOfTwo(-3) + 1.0;
    r = x * r / 7.0 + 1.0;
    r = x * r / 6.0 + 1.0;
    r = x * r / 5.0 + 1.0;
    r = x * r * PowerOfTwo(-2) + 1.0;
    r = x * r / 3.0 + 1.0;
    r = x * r * PowerOfTwo(-1) + 1.0;
    r = x * r;
    return r;
}

static DDouble expm1_cf(DDouble x)
{
    // For the absolute error to be < epsilon/2
    assert(greater_in_magnitude(1.085e-2, x.hi()));

    // Continued fraction expansion of the exponential function
    //  4*div + div_d + 4*add_d + add_sm + mul_p = 193 flops
    DDouble xsq = x * x;
    DDouble r;
    r = xsq / 18.0 + 14.0;
    r = xsq / r + 10.0;
    r = xsq / r + 6.0;
    r = (-x).add_small(xsq / r) + 2.0;
    r = x / r;
    r *= PowerOfTwo(1);
    return r;
}

static DDouble reduce_mod2(DDouble x, int n, double &quot)
{
    double hi = ldexp(x.hi(), -n);
    hi = ldexp(modf(hi, &quot), n);
    return DDouble(hi, x.lo());
}

DDouble exp(DDouble x)
{
    // 1/128
    double y;
    DDouble z = reduce_mod2(x, -7, y);

    // exp(z + y/128) = exp(z) exp(1/128)^y
    DDouble exp_z = 1.0 + expm1_cf(z);

    const static DDouble EXP128TH(1.007843097206448, -6.872774751042842e-17);
    DDouble exp_y = pow(EXP128TH, int(y));

    return exp_z * exp_y;
}
