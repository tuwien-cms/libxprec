/* Hyperbolic functions to quad precision.
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "ddouble.h"

static DDouble reciprocal_factorial(int n)
{
    static const DDouble RECIPROCAL_FACTORIAL[23] = {
        {0.5,0.0}, // index 0 -> 1/2!
        {0.16666666666666666,9.25185853854297e-18},
        {0.041666666666666664,2.3129646346357427e-18},
        {0.008333333333333333,1.1564823173178714e-19},
        {0.001388888888888889,-5.300543954373577e-20},
        {0.0001984126984126984,1.7209558293420705e-22},
        {2.48015873015873e-5,2.1511947866775882e-23},
        {2.7557319223985893e-6,-1.858393274046472e-22},
        {2.755731922398589e-7,2.3767714622250297e-23},
        {2.505210838544172e-8,-1.448814070935912e-24},
        {2.08767569878681e-9,-1.20734505911326e-25},
        {1.6059043836821613e-10,1.2585294588752098e-26},
        {1.1470745597729725e-11,2.0655512752830745e-28},
        {7.647163731819816e-13,7.03872877733453e-30},
        {4.779477332387385e-14,4.399205485834081e-31},
        {2.8114572543455206e-15,1.6508842730861433e-31},
        {1.5619206968586225e-16,1.1910679660273754e-32},
        {8.22063524662433e-18,2.2141894119604265e-34},
        {4.110317623312165e-19,1.4412973378659527e-36},
        {1.9572941063391263e-20,-1.3643503830087908e-36},
        {8.896791392450574e-22,-7.911402614872376e-38},
        {3.868170170630684e-23,-8.843177655482344e-40},
        {1.6117375710961184e-24,-3.6846573564509766e-41}
        };

    assert((n + 2) * sizeof(DDouble) < sizeof(RECIPROCAL_FACTORIAL));
    if (n <= 1)
        return DDouble(1.0);
    else
        return RECIPROCAL_FACTORIAL[n - 2];
}

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
