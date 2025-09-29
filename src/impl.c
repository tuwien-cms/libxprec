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

extern inline bool xprec_equal(xprec_ddouble x, xprec_ddouble y);
extern inline bool xprec_not_equal(xprec_ddouble x, xprec_ddouble y);
extern inline bool xprec_greater(xprec_ddouble x, xprec_ddouble y);
extern inline bool xprec_greater_equal(xprec_ddouble x, xprec_ddouble y);
extern inline bool xprec_less(xprec_ddouble x, xprec_ddouble y);
extern inline bool xprec_less_equal(xprec_ddouble x, xprec_ddouble y);

extern inline bool xprec_isfinite(xprec_ddouble x);
extern inline bool xprec_isinf(xprec_ddouble x);
extern inline bool xprec_isnan(xprec_ddouble x);
extern inline bool xprec_isnormal(xprec_ddouble x);
extern inline bool xprec_iszero(xprec_ddouble x);
extern inline int xprec_classify(xprec_ddouble x);

extern inline xprec_ddouble xprec_ceil(xprec_ddouble x);
extern inline xprec_ddouble xprec_floor(xprec_ddouble x);
extern inline xprec_ddouble xprec_trunc(xprec_ddouble x);
extern inline xprec_ddouble xprec_round(xprec_ddouble x);

extern inline void xprec_swap(xprec_ddouble *x, xprec_ddouble *y);
extern inline int xprec_ilogb(xprec_ddouble x);
extern inline bool xprec_signbit(xprec_ddouble a);

extern inline xprec_ddouble xprec_abs(xprec_ddouble x);
extern inline xprec_ddouble xprec_ldexp(xprec_ddouble a, int n);
extern inline xprec_ddouble xprec_logb(xprec_ddouble x);
extern inline xprec_ddouble xprec_scalbn(xprec_ddouble a, int n);
extern inline xprec_ddouble xprec_copysign_qd(xprec_ddouble mag, double sgn);
extern inline xprec_ddouble xprec_copysign_qq(xprec_ddouble mag, xprec_ddouble sgn);
extern inline xprec_ddouble xprec_copysign_dq(double mag, xprec_ddouble sgn);
