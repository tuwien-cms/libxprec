/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../ddouble.h"

inline void xprec_swap(xprec_ddouble *x, xprec_ddouble *y)
{
    xprec_ddouble tmp;
    tmp = *x;
    *x = *y;
    *y = tmp;
}

inline xprec_ddouble xprec_ldexp(xprec_ddouble a, int n)
{
    xprec_ddouble r = {ldexp(a.hi, n), ldexp(a.lo, n)};
    return r;
}

inline xprec_ddouble xprec_scalbn(xprec_ddouble a, int n)
{
    xprec_ddouble r = {scalbn(a.hi, n), scalbn(a.lo, n)};
    return r;
}

inline int xprec_ilogb(xprec_ddouble x)
{
    return ilogb(x.hi);
}

inline xprec_ddouble xprec_logb(xprec_ddouble x)
{
    xprec_ddouble r = {logb(x.hi), 0};
    return r;
}

inline bool xprec_signbit(xprec_ddouble a)
{
    return signbit(a.hi);
}

inline xprec_ddouble xprec_copysign_qd(xprec_ddouble mag, double sgn)
{
    // The sign is determined by the hi part, however, the sign of hi and lo
    // need not be the same, so we cannot merely broadcast copysign to both
    // parts.
    return xprec_signbit(mag) != signbit(sgn) ? xprec_neg(mag) : mag;
}

inline xprec_ddouble xprec_copysign_qq(xprec_ddouble mag, xprec_ddouble sgn)
{
    return xprec_copysign_qd(mag, sgn.hi);
}

inline xprec_ddouble xprec_copysign_dq(double mag, xprec_ddouble sgn)
{
    xprec_ddouble r = {copysign(mag, sgn.hi), 0};
    return r;
}

inline xprec_ddouble xprec_abs(xprec_ddouble x)
{
    return signbit(x.hi) ? xprec_neg(x) : x;
}
