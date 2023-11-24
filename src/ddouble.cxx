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
