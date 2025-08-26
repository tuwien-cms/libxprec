/* C Interface
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"

extern inline xprec_ddouble xprec_addfast_dd(double a, double b);
extern inline xprec_ddouble xprec_addfast_dq(double a, xprec_ddouble b);
extern inline xprec_ddouble xprec_addfast_qd(xprec_ddouble a, double b);
extern inline xprec_ddouble xprec_addfast_qq(xprec_ddouble a, xprec_ddouble b);

extern inline xprec_ddouble xprec_add_dd(double a, double b);
extern inline xprec_ddouble xprec_add_dq(double a, xprec_ddouble b);
extern inline xprec_ddouble xprec_add_qd(xprec_ddouble a, double b);
extern inline xprec_ddouble xprec_add_qq(xprec_ddouble a, xprec_ddouble b);

extern inline xprec_ddouble xprec_mul_dd(double a, double b);
extern inline xprec_ddouble xprec_mul_dq(double a, xprec_ddouble b);
extern inline xprec_ddouble xprec_mul_qd(xprec_ddouble a, double b);
extern inline xprec_ddouble xprec_mul_qq(xprec_ddouble a, xprec_ddouble b);

extern inline xprec_ddouble xprec_div_dd(double a, double b);
extern inline xprec_ddouble xprec_div_dq(double a, xprec_ddouble b);
extern inline xprec_ddouble xprec_div_qd(xprec_ddouble a, double b);
extern inline xprec_ddouble xprec_div_qq(xprec_ddouble a, xprec_ddouble b);

extern inline xprec_ddouble xprec_add_pow2(xprec_ddouble a, double p);
extern inline xprec_ddouble xprec_mul_pow2(xprec_ddouble a, double p);
extern inline xprec_ddouble xprec_div_pow2(xprec_ddouble a, double p);

extern inline xprec_ddouble xprec_neg(xprec_ddouble a);

extern inline xprec_ddouble xprec_reciprocal_d(double a);
extern inline xprec_ddouble xprec_reciprocal_q(xprec_ddouble a);

extern inline xprec_ddouble xprec_sqrt_d(double a);
extern inline xprec_ddouble xprec_sqrt_q(xprec_ddouble a);

extern inline xprec_ddouble xprec_max(xprec_ddouble a, xprec_ddouble b);
extern inline xprec_ddouble xprec_min(xprec_ddouble a, xprec_ddouble b);

extern inline _Bool xprec_equal(xprec_ddouble x, xprec_ddouble y);
extern inline _Bool xprec_not_equal(xprec_ddouble x, xprec_ddouble y);
extern inline _Bool xprec_greater(xprec_ddouble x, xprec_ddouble y);
extern inline _Bool xprec_greater_equal(xprec_ddouble x, xprec_ddouble y);
extern inline _Bool xprec_less(xprec_ddouble x, xprec_ddouble y);
extern inline _Bool xprec_less_equal(xprec_ddouble x, xprec_ddouble y);

extern inline _Bool xprec_isfinite(xprec_ddouble x);
extern inline _Bool xprec_isinf(xprec_ddouble x);
extern inline _Bool xprec_isnan(xprec_ddouble x);
extern inline _Bool xprec_isnormal(xprec_ddouble x);
extern inline _Bool xprec_iszero(xprec_ddouble x);
extern inline int xprec_classify(xprec_ddouble x);
