/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include xprec_ddouble.h instead.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../ddouble.h"
#include <assert.h>
#include <math.h>

inline xprec_ddouble xprec_ceil(xprec_ddouble x)
{
    // If hi was not an integer, it means that rounding it up/down/towards zero
    // already gives our answer.  This also covers NaN since then x != x.
    // We cannot simply truncate both hi and lo since they may have the same
    // sign
    double hi = ceil(x.hi);
    if (hi != x.hi) {
        xprec_ddouble r = {hi, 0};
        return r;
    }

    // hi is an integer, so modify lo instead.  This may actually increase the
    // magnitude above the limit, so let's renormalize to be safe.
    double lo = ceil(x.lo);
    return xprec_addfast_dd(hi, lo);
}

inline xprec_ddouble xprec_floor(xprec_ddouble x)
{
    // If hi was not an integer, it means that rounding it up/down/towards zero
    // already gives our answer.  This also covers NaN since then x != x.
    // We cannot simply truncate both hi and lo since they may have the same
    // sign
    double hi = floor(x.hi);
    if (hi != x.hi) {
        xprec_ddouble r = {hi, 0};
        return r;
    }

    // hi is an integer, so modify lo instead.  This may actually increase the
    // magnitude above the limit, so let's renormalize to be safe.
    double lo = floor(x.lo);
    return xprec_addfast_dd(hi, lo);
}

inline xprec_ddouble xprec_trunc(xprec_ddouble x)
{
    // If hi was not an integer, it means that rounding it up/down/towards zero
    // already gives our answer.  This also covers NaN since then x != x.
    // We cannot simply truncate both hi and lo since they may have opposite
    // signs.
    double hi = trunc(x.hi);
    if (hi != x.hi) {
        xprec_ddouble r = {hi, 0};
        return r;
    }

    // hi is an integer, so modify lo instead.  Here, one needs to be careful
    // to respect the truncation direction that hi requires, and so we have
    // to round towards -+infinity, for x > 0 and x < 0, repectively.
    //
    // This may actually increase the  magnitude above the limit, so let's
    // renormalize to be safe.
    double lo = signbit(x.hi) ? ceil(x.lo) : floor(x.lo);
    return xprec_addfast_dd(hi, lo);
}

inline xprec_ddouble xprec_round(xprec_ddouble x)
{
    // trunc is fast, so it makes sense to use this as a building block.
    double nudge = copysign(0.5, x.hi);
    return xprec_trunc(xprec_add_qd(x, nudge));
}
