/* C interface to ddouble
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "version.h"

#ifdef __cplusplus
extern "C" {
#endif

struct _xprec_ddouble {
    double hi;
    double lo;
};

typedef struct _xprec_ddouble xprec_ddouble;

inline xprec_ddouble xprec_addfast_dd(double a, double b);
inline xprec_ddouble xprec_addfast_dq(double a, xprec_ddouble b);
inline xprec_ddouble xprec_addfast_qd(xprec_ddouble a, double b);
inline xprec_ddouble xprec_addfast_qq(xprec_ddouble a, xprec_ddouble b);

inline xprec_ddouble xprec_add_dd(double a, double b);
inline xprec_ddouble xprec_add_dq(double a, xprec_ddouble b);
inline xprec_ddouble xprec_add_qd(xprec_ddouble a, double b);
inline xprec_ddouble xprec_add_qq(xprec_ddouble a, xprec_ddouble b);

inline xprec_ddouble xprec_mul_dd(double a, double b);
inline xprec_ddouble xprec_mul_dq(double a, xprec_ddouble b);
inline xprec_ddouble xprec_mul_qd(xprec_ddouble a, double b);
inline xprec_ddouble xprec_mul_qq(xprec_ddouble a, xprec_ddouble b);

inline xprec_ddouble xprec_div_dd(double a, double b);
inline xprec_ddouble xprec_div_dq(double a, xprec_ddouble b);
inline xprec_ddouble xprec_div_qd(xprec_ddouble a, double b);
inline xprec_ddouble xprec_div_qq(xprec_ddouble a, xprec_ddouble b);

inline xprec_ddouble xprec_reciprocal_d(double a);
inline xprec_ddouble xprec_reciprocal_q(xprec_ddouble a);

xprec_ddouble xprec_pos(xprec_ddouble a);
xprec_ddouble xprec_neg(xprec_ddouble a);

xprec_ddouble xprec_abs(xprec_ddouble a);
xprec_ddouble xprec_acos(xprec_ddouble a);
xprec_ddouble xprec_acosh(xprec_ddouble a);
xprec_ddouble xprec_asin(xprec_ddouble a);
xprec_ddouble xprec_asinh(xprec_ddouble a);
xprec_ddouble xprec_atan(xprec_ddouble a);
xprec_ddouble xprec_atan2(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_atanh(xprec_ddouble a);
xprec_ddouble xprec_ceil(xprec_ddouble a);
xprec_ddouble xprec_cos(xprec_ddouble a);
xprec_ddouble xprec_cosh(xprec_ddouble a);
xprec_ddouble xprec_exp(xprec_ddouble a);
xprec_ddouble xprec_expm1(xprec_ddouble a);
xprec_ddouble xprec_fabs(xprec_ddouble a);
xprec_ddouble xprec_fmax(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_fmin(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_floor(xprec_ddouble a);
xprec_ddouble xprec_hypot(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_log(xprec_ddouble a);
xprec_ddouble xprec_log1p(xprec_ddouble a);
xprec_ddouble xprec_logb(xprec_ddouble a);
xprec_ddouble xprec_nextafter(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_pow(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_round(xprec_ddouble a);
xprec_ddouble xprec_sin(xprec_ddouble a);
xprec_ddouble xprec_sinh(xprec_ddouble a);
xprec_ddouble xprec_sqrt(xprec_ddouble a);
xprec_ddouble xprec_tan(xprec_ddouble a);
xprec_ddouble xprec_tanh(xprec_ddouble a);

#include "internal/arith.h"

#ifdef __cplusplus
}
#endif
