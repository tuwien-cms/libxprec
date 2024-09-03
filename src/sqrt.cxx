/* Mathematical functions to quad precision.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include "xprec/internal/utils.h"

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

XPREC_API_EXPORT
DDouble sqrt(DDouble a)
{
    double y0 = std::sqrt(a.hi());
    if (a.hi() <= 0 || !isfinite(a.hi()))
        return y0;

    // From: Karp, High Precision Division and Square Root, 1993, Table II
    // This is based on Newton-Ralphson for f(x) = a - 1/x^2:
    //
    //   x0 = approx(1/sqrt(A))
    //   x  = x + 0.5 * x * (1.0 - A * x * x)
    //
    double x0_half = 0.5 / y0;
    double delta_y = x0_half * (std::fma(-y0, y0, a.hi()) + a.lo());
    DDouble y1 = ExDouble(y0).add_small(delta_y);
    return y1;
}

static DDouble hypot_normalized(DDouble x)
{
    // computes sqrt(1 + x*x) with x >= 1
    assert(x >= std::nextafter(1.0, -INFINITY));

    // For very large arguments, we simply get x
    if (x.hi() >= 2 / std::numeric_limits<double>::epsilon())
        return x;

    return sqrt((x * x).add_small(1.0));
}

XPREC_API_EXPORT
DDouble hypot(DDouble x, DDouble y)
{
    // Handle special cases
    if (isnan(x) || isnan(y))
        return NAN;

    // Make sure we have x >= y >= 0 (at least approximately)
    x = abs(x);
    y = abs(y);
    if (y > x)
        swap(x, y);

    // Rescale and use inner function
    if (y == 0)
        return x;
    else
        return y * hypot_normalized(x / y);
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
