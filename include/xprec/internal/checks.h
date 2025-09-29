/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Most of the basic numerical algorithms are directly lifted from:
 *  - M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
 *  - Karp, High Precision Division and Square Root (1993)
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "../ddouble.h"
#include <assert.h>
#include <float.h>
#include <math.h>

inline bool xprec_equal(xprec_ddouble x, xprec_ddouble y)
{
    return x.hi == y.hi && x.lo == y.lo;
}

inline bool xprec_not_equal(xprec_ddouble x, xprec_ddouble y)
{
    return x.hi != y.hi || x.lo != y.lo;
}

inline bool xprec_less_equal(xprec_ddouble x, xprec_ddouble y)
{
    return x.hi < y.hi || (x.hi == y.hi && x.lo <= y.lo);
}

inline bool xprec_less(xprec_ddouble x, xprec_ddouble y)
{
    return x.hi < y.hi || (x.hi == y.hi && x.lo < y.lo);
}

inline bool xprec_greater_equal(xprec_ddouble x, xprec_ddouble y)
{
    return x.hi > y.hi || (x.hi == y.hi && x.lo >= y.lo);
}

inline bool xprec_greater(xprec_ddouble x, xprec_ddouble y)
{
    return x.hi > y.hi || (x.hi == y.hi && x.lo > y.lo);
}

inline bool xprec_isfinite(xprec_ddouble x) { return isfinite(x.hi); }

inline bool xprec_isinf(xprec_ddouble x) { return isinf(x.hi); }

inline bool xprec_isnan(xprec_ddouble x) { return isnan(x.hi); }

inline bool xprec_isnormal(xprec_ddouble x)
{
    // Denormalization is double-double is a bit of a strange concept,
    // since the lo part may be a denormalized number even if the whole
    // number is still "normal".
    return isnormal(x.hi * DBL_EPSILON);
}

inline bool xprec_iszero(xprec_ddouble x) { return x.hi == 0; }

inline int xprec_classify(xprec_ddouble x)
{
    // This also works with zero, since that can be determined from the
    // hi part alone
    return fpclassify(x.hi);
}

inline xprec_ddouble xprec_min(xprec_ddouble a, xprec_ddouble b)
{
    // fmin considers NaN to be the largest number. (a <= b) is false with
    // either element being NaN, if a is NaN, then it is okay to return b;
    // but if b is NaN, we have to return a
    return xprec_less_equal(a, b) || xprec_isnan(b) ? a : b;
}

inline xprec_ddouble xprec_max(xprec_ddouble a, xprec_ddouble b)
{
    return xprec_less_equal(a, b) || xprec_isnan(a) ? b : a;
}
