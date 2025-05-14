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

UNARY_OP(xprec_abs, abs)
UNARY_OP(xprec_acosh, acosh)
UNARY_OP(xprec_asinh, asinh)
UNARY_OP(xprec_atanh, atanh)
UNARY_OP(xprec_ceil, ceil)
UNARY_OP(xprec_cosh, cosh)
UNARY_OP(xprec_fabs, fabs)
BINARY_OP(xprec_fmax, fmax)
BINARY_OP(xprec_fmin, fmin)
UNARY_OP(xprec_floor, floor)
BINARY_OP(xprec_hypot, hypot)
UNARY_OP(xprec_logb, logb)
UNARY_OP(xprec_round, round)
UNARY_OP(xprec_sinh, sinh)
UNARY_OP(xprec_sqrt, sqrt)
UNARY_OP(xprec_tanh, tanh)
