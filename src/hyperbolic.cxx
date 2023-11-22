/* Hyperbolic functions to quad precision.
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "ddouble.h"
#include "taylor.h"

DDouble cosh(DDouble x)
{
    // simply use the definition:
    //
    //    2 cosh(x) = exp(x) + exp(-x) = exp(x) + 1/exp(x)
    //
    x = abs(x);
    DDouble res = exp(x);

    // Only add something if indeed something can change.
    if (x.hi() < 36.5)
        res += reciprocal(res);
    return PowerOfTwo(-1) * res;
}

static DDouble sinh_kernel(DDouble x)
{
    // We need to make sure that (exp(x) - exp(-x)) does not lose
    // precision, which is why we need to have this work till abs(x) < 0.15.

    // Convergence of the Taylor approx to 2e-32
    assert(greater_in_magnitude(0.155, x.hi()));

    // Taylor series of the sinh around 0
    DDouble xsq = x * x;
    DDouble r = x;
    DDouble xpow = x;
    for (int i = 3; i <= 17; i += 2) {
        xpow *= xsq;
        r = r.add_small(reciprocal_factorial(i) * xpow);
    }
    return r;
}

DDouble sinh(DDouble x)
{
    // Special values: +Inf, -Inf are all preserved
    if (!std::isfinite(x.hi()))
        return x;

    // For small values, we use the Taylor series
    if (fabs(x.hi()) < 0.15)
        return sinh_kernel(x);

    // Else we simply use the definition
    if (x.hi() < 0)
        return -sinh(-x);

    DDouble res = exp(x);

    // Only add something if indeed something can change.
    if (x.hi() < 36.5)
        res -= reciprocal(res);
    return PowerOfTwo(-1) * res;
}

static DDouble tanh_kernel(DDouble x)
{
    // Again we need to make sure that (exp(x) - exp(-x)) does not lose
    // precision, which is why we need to have this work till abs(x) < 0.15.

    // Convergence of the CF approx to 2e-32
    assert(greater_in_magnitude(0.23, x.hi()));

    // Continued fraction expansion of the tanh
    // Abramowitz and Stegun 4.5.70
    DDouble xsq = x * x;
    DDouble r;
    r = xsq / 19.0;
    r = xsq / (r + 17.0);
    r = xsq / (r + 15.0);
    r = xsq / (r + 13.0);
    r = xsq / (r + 11.0);
    r = xsq / (r + 9.0);
    r = xsq / (r + 7.0);
    r = xsq / (r + 5.0);
    r = xsq / (r + 3.0);
    r = x / (r + 1.0);
    return r;
}

DDouble tanh(DDouble x)
{
    // Special values
    if (std::isnan(x.hi()))
        return x;

    // For small values, use the continued fraction representation
    if (fabs(x.hi()) < 0.2)
        return tanh_kernel(x);

    // Asymptotically, we have +- 1
    if (fabs(x.hi()) > 36.5)
        return copysign(1.0, x.hi());

    // Otherwise, simply use defitions
    DDouble exp_x = exp(x);
    DDouble exp_mx = reciprocal(exp_x);
    return (exp_x - exp_mx) / (exp_x + exp_mx);
}
