/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "../ddouble.hpp"

namespace xprec {

inline void swap(DDouble &x, DDouble &y)
{
    double tmp;
    tmp = x._hi;
    x._hi = y._hi;
    y._hi = tmp;
    tmp = x._lo;
    x._lo = y._lo;
    y._lo = tmp;
}

inline DDouble ldexp(DDouble a, int n)
{
    return DDouble(std::ldexp(a.hi(), n), std::ldexp(a.lo(), n));
}

inline DDouble scalbn(DDouble a, int n)
{
    return DDouble(std::scalbn(a.hi(), n), std::scalbn(a.lo(), n));
}

inline int ilogb(DDouble x) { return std::ilogb(x.hi()); }

inline DDouble logb(DDouble x) { return std::logb(x.hi()); }

inline bool signbit(DDouble a) { return std::signbit(a.hi()); }

inline DDouble copysign(DDouble mag, double sgn)
{
    // The sign is determined by the hi part, however, the sign of hi and lo
    // need not be the same, so we cannot merely broadcast copysign to both
    // parts.
    return signbit(mag) != std::signbit(sgn) ? -mag : mag;
}

inline DDouble copysign(DDouble mag, DDouble sgn)
{
    return copysign(mag, sgn.hi());
}

inline DDouble copysign(double mag, DDouble sgn)
{
    return DDouble(std::copysign(mag, sgn.hi()));
}

inline DDouble abs(DDouble x) { return fabs(x); }

inline DDouble fabs(DDouble x) { return signbit(x) ? -x : x; }

inline DDouble ceil(DDouble x)
{
    // If hi was not an integer, it means that rounding it up/down/towards zero
    // already gives our answer.  This also covers NaN since then x != x.
    // We cannot simply truncate both hi and lo since they may have the same
    // sign
    double hi = std::ceil(x.hi());
    if (hi != x.hi())
        return hi;

    // hi is an integer, so modify lo instead.  This may actually increase the
    // magnitude above the limit, so let's renormalize to be safe.
    double lo = std::ceil(x.lo());
    return ExDouble(x.hi()).add_small(lo);
}

inline DDouble floor(DDouble x)
{
    // If hi was not an integer, it means that rounding it up/down/towards zero
    // already gives our answer.  This also covers NaN since then x != x.
    // We cannot simply truncate both hi and lo since they may have the same
    // sign
    double hi = std::floor(x.hi());
    if (hi != x.hi())
        return hi;

    // hi is an integer, so modify lo instead.  This may actually increase the
    // magnitude above the limit, so let's renormalize to be safe.
    double lo = std::floor(x.lo());
    return ExDouble(x.hi()).add_small(lo);
}

inline DDouble trunc(DDouble x)
{
    // If hi was not an integer, it means that rounding it up/down/towards zero
    // already gives our answer.  This also covers NaN since then x != x.
    // We cannot simply truncate both hi and lo since they may have opposite
    // signs.
    double hi = std::trunc(x.hi());
    if (hi != x.hi())
        return hi;

    // hi is an integer, so modify lo instead.  Here, one needs to be careful
    // to respect the truncation direction that hi requires, and so we have
    // to round towards -+infinity, for x > 0 and x < 0, repectively.
    //
    // This may actually increase the  magnitude above the limit, so let's
    // renormalize to be safe.
    double lo = std::signbit(x.hi()) ? std::ceil(x.lo()) : std::floor(x.lo());
    return ExDouble(x.hi()).add_small(lo);
}

inline DDouble round(DDouble x)
{
    // trunc is fast, so it makes sense to use this as a building block.
    double nudge = std::copysign(0.5, x.hi());
    return trunc(x + nudge);
}

} /* namespace xprec */
