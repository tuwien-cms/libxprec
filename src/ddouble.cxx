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

static DDouble expm1_kernel(DDouble x)
{
    // We need to make sure that (1 + x) does not lose possible significant
    // digits, so no matter what strategy we choose here, the convergence
    // needs to go out to x = log(1.5) = 0.22

    // Convergence of the CF approx to 2e-32
    assert(greater_in_magnitude(0.3, x.hi()));

    // Continued fraction expansion of the exponential function
    //  6*div + div_d + 6*add_d + add_sm + mul_p = 253 flops
    DDouble xsq = x * x;
    DDouble r;
    r = xsq / 34.0 + 30.0;
    r = xsq / r + 26.0;
    r = xsq / r + 22.0;
    r = xsq / r + 18.0;
    r = xsq / r + 14.0;
    r = xsq / r + 10.0;
    r = xsq / r + 6.0;
    r = (-x).add_small(xsq / r) + 2.0;
    r = x / r;
    r *= PowerOfTwo(1);
    return r;
}

//#include "mpfloat.h"

DDouble exp(DDouble x)
{
    // x = y/2 + z
    double y = round(2 * x.hi());
    DDouble z = x - y/2;

    // exp(z + y/4) = (1 + expm1(z)) exp(1/4)^y
    DDouble exp_z = 1.0 + expm1_kernel(z);

    const static DDouble EXP_HALF(1.6487212707001282, -4.731568479435833e-17);
    DDouble exp_y = pow(EXP_HALF, int(y));
    //DDouble exp_y= exp(MPFloat(y/2)).as_ddouble();

    return exp_z * exp_y;
}
