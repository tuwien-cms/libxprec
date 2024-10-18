/* C Interface
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include "xprec/ddouble.hpp"

using xprec::DDouble;

#define UNARY_OP(cfunc, cxxop)                                          \
    extern "C"                                                          \
    xprec_ddouble cfunc(xprec_ddouble a)                                \
    {                                                                   \
        DDouble r = cxxop(DDouble(a.hi, a.lo));                         \
        return {r.hi(), r.lo()};                                        \
    }

#define BINARY_OP(cfunc, cxxop)                                         \
    extern "C"                                                          \
    xprec_ddouble cfunc(xprec_ddouble a, xprec_ddouble b)               \
    {                                                                   \
        DDouble r = cxxop(DDouble(a.hi, a.lo), DDouble(b.hi, b.lo));    \
        return {r.hi(), r.lo()};                                        \
    }

BINARY_OP(xprec_add, operator+)
BINARY_OP(xprec_sub, operator-)
BINARY_OP(xprec_mul, operator*)
BINARY_OP(xprec_div, operator/)

UNARY_OP(xprec_pos, operator+)
UNARY_OP(xprec_neg, operator-)
UNARY_OP(xprec_reciprocal, reciprocal)

UNARY_OP(xprec_abs, abs)
UNARY_OP(xprec_acos, acos)
UNARY_OP(xprec_acosh, acosh)
UNARY_OP(xprec_asin, asin)
UNARY_OP(xprec_asinh, asinh)
UNARY_OP(xprec_atan, atan)
BINARY_OP(xprec_atan2, atan2)
UNARY_OP(xprec_atanh, atanh)
UNARY_OP(xprec_ceil, ceil)
UNARY_OP(xprec_cos, cos)
UNARY_OP(xprec_cosh, cosh)
UNARY_OP(xprec_exp, exp)
UNARY_OP(xprec_expm1, expm1)
UNARY_OP(xprec_fabs, fabs)
BINARY_OP(xprec_fmax, fmax)
BINARY_OP(xprec_fmin, fmin)
UNARY_OP(xprec_floor, floor)
BINARY_OP(xprec_hypot, hypot)
UNARY_OP(xprec_log, log)
UNARY_OP(xprec_log1p, log1p)
UNARY_OP(xprec_logb, logb)
BINARY_OP(xprec_nextafter, nextafter)
BINARY_OP(xprec_pow, pow)
UNARY_OP(xprec_round, round)
UNARY_OP(xprec_sin, sin)
UNARY_OP(xprec_sinh, sinh)
UNARY_OP(xprec_sqrt, sqrt)
UNARY_OP(xprec_tan, tan)
UNARY_OP(xprec_tanh, tanh)
