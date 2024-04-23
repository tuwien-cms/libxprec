/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Most of the basic numerical algorithms are directly lifted from:
 * M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include "../ddouble.h"
#include <limits>

namespace xprec {
namespace _internal {

/**
 * Return true if x is greater or equal in magnitude as y.
 *
 * Return true if the xponent of y does not exceed the exponent of x.  NaN
 * and Inf are considered maximum magnitude, 0 is considered minimum magnitude.
 * For DDoubles, only the hi part is compared.
 */
inline bool greater_in_magnitude(double x, double y)
{
    if(std::numeric_limits<double>::is_iec559) {
        union {
            double number;
            uint64_t pattern;
        } x_u = {x}, y_u = {y};

        // Shift out sign bit
        return (x_u.pattern << 1) >= (y_u.pattern << 1);
    } else {
        return !(std::fabs(x) < std::fabs(y));
    }
}

inline bool greater_in_magnitude(DDouble x, DDouble y)
{
    return greater_in_magnitude(x.hi(), y.hi());
}

inline bool greater_in_magnitude(DDouble x, double y)
{
    return greater_in_magnitude(x.hi(), y);
}

inline bool greater_in_magnitude(double x, DDouble y)
{
    return greater_in_magnitude(x, y.hi());
}

/**
 * Return true if the mantissa part of a number is zero.
 */
inline bool is_power_of_two(double x)
{
    if (std::numeric_limits<double>::is_iec559) {
        static const uint64_t mantissa_mask = 0xFFFFFFFFFFFFFUL;
        union {
            double number;
            uint64_t pattern;
        } x_u = {x};
        return (x_u.pattern & mantissa_mask) == 0;
    } else {
        double exp;
        return std::modf(x, &exp);
    }
}

inline bool is_power_of_two(DDouble x)
{
    return x.lo() == 0 && is_power_of_two(x.hi());
}

}
}  // namespace xprec::_internal
