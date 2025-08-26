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

// Basic arithmetic

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

inline xprec_ddouble xprec_add_pow2(xprec_ddouble a, double p);
inline xprec_ddouble xprec_mul_pow2(xprec_ddouble a, double p);
inline xprec_ddouble xprec_div_pow2(xprec_ddouble a, double p);

inline xprec_ddouble xprec_neg(xprec_ddouble a);

inline xprec_ddouble xprec_reciprocal_d(double a);
inline xprec_ddouble xprec_reciprocal_q(xprec_ddouble a);

inline xprec_ddouble xprec_sqrt_d(double a);
inline xprec_ddouble xprec_sqrt_q(xprec_ddouble a);

// Checks and relations

inline xprec_ddouble xprec_max(xprec_ddouble a, xprec_ddouble b);
inline xprec_ddouble xprec_min(xprec_ddouble a, xprec_ddouble b);

inline bool xprec_equal(xprec_ddouble x, xprec_ddouble y);
inline bool xprec_not_equal(xprec_ddouble x, xprec_ddouble y);
inline bool xprec_greater(xprec_ddouble x, xprec_ddouble y);
inline bool xprec_greater_equal(xprec_ddouble x, xprec_ddouble y);
inline bool xprec_less(xprec_ddouble x, xprec_ddouble y);
inline bool xprec_less_equal(xprec_ddouble x, xprec_ddouble y);

inline bool xprec_isfinite(xprec_ddouble x);
inline bool xprec_isinf(xprec_ddouble x);
inline bool xprec_isnan(xprec_ddouble x);
inline bool xprec_isnormal(xprec_ddouble x);
inline bool xprec_iszero(xprec_ddouble x);
inline int xprec_classify(xprec_ddouble x);



// Functions

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
xprec_ddouble xprec_floor(xprec_ddouble a);
xprec_ddouble xprec_hypot(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_log(xprec_ddouble a);
xprec_ddouble xprec_log1p(xprec_ddouble a);
xprec_ddouble xprec_logb(xprec_ddouble a);
xprec_ddouble xprec_modf(xprec_ddouble a, xprec_ddouble *i);
xprec_ddouble xprec_nextafter(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_pow_qq(xprec_ddouble a, xprec_ddouble b);
xprec_ddouble xprec_pow_qi(xprec_ddouble a, int b);
xprec_ddouble xprec_round(xprec_ddouble a);
xprec_ddouble xprec_sin(xprec_ddouble a);
xprec_ddouble xprec_sinh(xprec_ddouble a);
xprec_ddouble xprec_tan(xprec_ddouble a);
xprec_ddouble xprec_tanh(xprec_ddouble a);

void xprec_gauss_chebyshev(int n, xprec_ddouble x[], xprec_ddouble w[]);
void xprec_gauss_legendre(int n, xprec_ddouble x[], xprec_ddouble w[]);

#include "internal/arith.h"
#include "internal/checks.h"

#ifdef __cplusplus
}
#endif
