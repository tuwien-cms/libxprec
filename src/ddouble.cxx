/* Mathematical functions to quad precision.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

XPREC_API_EXPORT
DDouble sqrt(DDouble a)
{
    double sqrt_hi = std::sqrt(a.hi());
    if (a.hi() <= 0)
        return sqrt_hi;

    // From: Karp, High Precision Division and Square Root, 1993, Table II
    // This is based on Newton-Ralphson for 1/sqrt(x):
    //
    //   x0 = approx(1/sqrt(A))
    //   x  = x + 0.5 * x * (1.0 - A * x * x)
    //
    double x0 = 1.0 / sqrt_hi;
    ExDouble y0 = a.hi() * x0;
    DDouble tt0 = y0 * y0;
    double t0 = 0.5 * x0 * (a - tt0).hi();
    DDouble y1 = y0.add_small(t0);
    return y1;
}

XPREC_API_EXPORT
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
