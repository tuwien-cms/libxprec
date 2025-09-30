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

static inline DDouble _hypot(DDouble x, DDouble y)
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
        return sqrt(square(x).add_small(square(y))) * LARGE;
    } else if (greater_in_magnitude(SMALL, x)) {
        // For small values, scale up to avoid underflow
        x *= LARGE;
        y *= LARGE;
        return sqrt(square(x).add_small(square(y))) * SMALL;
    } else {
        // We're fine
        return sqrt(square(x).add_small(square(y)));
    }
}

extern "C" XPREC_API_EXPORT
xprec_ddouble xprec_hypot(xprec_ddouble x, xprec_ddouble y)
{
    return _hypot(x, y);
}

extern "C" XPREC_API_EXPORT
xprec_ddouble xprec_invsqrt(xprec_ddouble x)
{
    // Use strategy similar to Karp to compute 1/sqrt(x)
    // cost 12 flops (3 of which divisions), observed error 3 u^2

    // First, give an approximation to sqrt(x)
    double sqrt_x0 = std::sqrt(x.hi);
    if (x.hi <= 0 || !isfinite(x.hi)) {
        xprec_ddouble r = {1/sqrt_x0, 0};
        return r;
    }

    // The correction term is then given by the lo part and the difference
    // to the exact sqrt
    double delta_x = fma(-sqrt_x0, sqrt_x0, x.hi) + x.lo;

    // Compute 1/sqrt_x0 to quad precision
    xprec_ddouble y0 = xprec_reciprocal_d(sqrt_x0);

    // Correct using first-order expansion
    //
    //  1/sqrt(x0 + delta_x) = 1/sqrt(x0) - delta_x / (2 * sqrt(x0)**3) + ...
    //
    // The correction term can be computed in double precision, but it is
    // important to use fma, as sqrt(x0)**3 may overflow.
    double y_lo = fma(-delta_x / (2 * x.hi), y0.hi, y0.lo);
    return xprec_addfast_dd(y0.hi, y_lo);
}

static inline DDouble _modf(DDouble x, DDouble &i)
{
    i = trunc(x);
    return x.add_small(-i);
}

extern "C" XPREC_API_EXPORT
xprec_ddouble xprec_modf(xprec_ddouble x, xprec_ddouble *iptr)
{
    return _modf(x, *(DDouble *)iptr);
}

} // namespace xprec
