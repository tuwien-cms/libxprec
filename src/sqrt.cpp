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
