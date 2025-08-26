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

inline DDouble trunc(DDouble x) { return xprec_trunc(x); }

inline DDouble ceil(DDouble x) { return xprec_ceil(x); }

inline DDouble floor(DDouble x) { return xprec_floor(x); }

inline DDouble round(DDouble x) { return xprec_round(x); }

} /* namespace xprec */
