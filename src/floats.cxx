/* Float manipulation functions
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include "xprec/internal/utils.h"
#include "xprec/numbers.h"
#include <cassert>

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

XPREC_API_EXPORT
DDouble nextafter(DDouble x, DDouble y)
{
    using dd_limits = std::numeric_limits<DDouble>;

    // Handle no-ops
    if (x == y)
        return x;

    // direction
    double dir = x > y ? -INFINITY : INFINITY;

    // There are two ways to define z = nextafter(x, +INFINITY):
    //   1. as the smallest value z for which z != x
    //   2. as the smallest value z for which z - x != 0
    //
    // Both definitions are not applicable for x = -0.0, where z = 0.0, but
    // are otherwise equivalent.  The complication for DDouble is that due to
    // the "varying" epsilon, definition 2 is stronger than 1 (sweeps across
    // fewer numbers).
    double lo;
    if (x.lo() == 0)
        lo = std::copysign(std::numeric_limits<double>::min(), dir);
    else
        lo = std::nextafter(x.lo(), dir);

    // This should work since the representation is unique
    DDouble z = ExDouble(x.hi()).add_small(lo);

    // Handle overflows
    if (!isfinite(z)) {
        if (isinf(x) && signbit(x) != signbit(y))
            return copysign(dd_limits::max(), x);
        else
            return std::nextafter(x.hi(), dir);
    }

    // Otherwise, simply return
    assert (z != x);
    return z;
}

}
