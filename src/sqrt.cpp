/* Mathematical functions to quad precision.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.hpp"
#include "xprec/internal/utils.hpp"

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

#include <iostream>

namespace xprec {

XPREC_API_EXPORT
DDouble sqrt(DDouble a)
{
    // The double result provides a approximation to sqrt(a). It performs
    // all the special-case handling, which is why we defer to it in these
    // cases.
    double y0 = std::sqrt(a.hi());
    if (a.hi() <= 0.0 || !isfinite(a.hi()))
        return y0;

    // From: Karp, High Precision Division and Square Root, 1993, Table II
    // This is based on Newton-Ralphson for f(x) = a - 1/x^2:
    //
    //   x0 = approx(1/sqrt(A))
    //   x  = x + 0.5 * x * (1.0 - A * x * x)
    //
    double x0_half = 0.5 / y0;
    double delta_y = x0_half * (std::fma(-y0, y0, a.hi()) + a.lo());

    // We would like to do DDouble(y0, delta_y), however, delta_y may alter
    // the least significant digit of y0.
    return ExDouble(y0).add_small(delta_y);
}

XPREC_API_EXPORT
DDouble hypot(DDouble x, DDouble y)
{
    using _internal::greater_in_magnitude;

    // Make sure that the values are ordered by magnitude
    if (!greater_in_magnitude(x, y)) {
        swap(x, y);
    }

    // Check for infinities
    if (!std::isfinite(x.hi())) {
        return std::isnan(y.hi()) ? y : x;
    }

    // Splits the range in half
    static const PowerOfTwo LARGE =
        ldexp(PowerOfTwo(1), std::numeric_limits<double>::max_exponent / 2);
    static const PowerOfTwo SMALL = reciprocal(LARGE);

    if (greater_in_magnitude(x, LARGE)) {
        // For large values, scale down to avoid overflow
        x *= SMALL;
        y *= SMALL;
        return sqrt((x * x).add_small(y * y)) * LARGE;
    } else if (greater_in_magnitude(SMALL, x)) {
        // For small values, scale up to avoid underflow
        x *= LARGE;
        y *= LARGE;
        return sqrt((x * x).add_small(y * y)) * SMALL;
    } else {
        // We're fine
        return sqrt((x * x).add_small(y * y));
    }
}

XPREC_API_EXPORT
DDouble modf(DDouble x, DDouble &i)
{
    i = trunc(x);
    return x.add_small(-i);
}

XPREC_API_EXPORT
DDouble modf(DDouble x, DDouble *iptr) { return modf(x, *iptr); }

} // namespace xprec
