/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Most of the basic numerical algorithms are directly lifted from:
 * M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "../ddouble.h"
#include <assert.h>
#include <math.h>

#if __STDC_VERSION__ >= 201112L

#define xprec_addfast(a, b) _Generic((a),         \
            xprec_ddouble: _Generic((b),          \
                xprec_ddouble: xprec_addfast_qq,  \
                default:       xprec_addfast_qd), \
            default: _Generic((b),                \
                xprec_ddouble: xprec_addfast_dq,  \
                default:       xprec_addfast_dd), \
            )(a, b)

#define xprec_add(a, b) _Generic((a),             \
            xprec_ddouble: _Generic((b),          \
                xprec_ddouble: xprec_add_qq,      \
                default:       xprec_add_qd),     \
            default: _Generic((b),                \
                xprec_ddouble: xprec_add_dq,      \
                default:       xprec_add_dd),     \
            )(a, b)

#define xprec_sub(a, b) _Generic((a),             \
            xprec_ddouble: _Generic((b),          \
                xprec_ddouble: xprec_add_qq,      \
                default:       xprec_add_qd),     \
            default: _Generic((b),                \
                xprec_ddouble: xprec_add_dq,      \
                default:       xprec_add_dd),     \
            )(a, -(b))

#define xprec_mul(a, b) _Generic((a),             \
            xprec_ddouble: _Generic((b),          \
                xprec_ddouble: xprec_mul_qq,      \
                default:       xprec_mul_qd),     \
            default: _Generic((b),                \
                xprec_ddouble: xprec_mul_dq,      \
                default:       xprec_mul_dd),     \
            )(a, b)

#define xprec_div(a, b) _Generic((a),             \
            xprec_ddouble: _Generic((b),          \
                xprec_ddouble: xprec_div_qq,      \
                default:       xprec_div_qd),     \
            default: _Generic((b),                \
                xprec_ddouble: xprec_div_dq,      \
                default:       xprec_div_dd),     \
            )(a, b)

#define xprec_reciprocal(a) _Generic((a),         \
            xprec_ddouble: xprec_reciprocal_q,    \
            default:       xprec_reciprocal_d,    \
            )(a)

#endif

inline xprec_ddouble xprec_reciprocal_q(xprec_ddouble a);

// ---------------------------------------------------------------------------
// double (op) double -> quad

inline xprec_ddouble xprec_addfast_dd(double a, double b)
{
    // M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
    // Algorithm 1: cost 3 flops
    double s = a + b;
    double z = s - a;
    double t = b - z;
    assert(s + t == 0 || !isfinite(s));
    return {s, t};
}

inline xprec_ddouble xprec_add_dd(double a, double b)
{
    // Algorithm 2: cost 6 flops
    double s = a + b;
    double aprime = s - b;
    double bprime = s - aprime;
    double delta_a = a - aprime;
    double delta_b = b - bprime;
    double t = delta_a + delta_b;
    return {s, t};
}

inline xprec_ddouble xprec_mul_dd(double a, double b)
{
    // Algorithm 3: cost 2 flops
    double pi = a * b;
    double rho = fma(a, b, -pi);
    return {pi, rho};
}

inline xprec_ddouble xprec_div_dd(double a, double b)
{
    // Since we are rounding faithfully, the hi part is exact
    double th = a / b;

    // Multiply hi part with b and compare exactly to a to see difference
    double rl = fma(-b, th, a);
    double tl = rl / b;
    return {th, tl};
}

inline xprec_ddouble xprec_reciprocal_d(double x)
{
    return xprec_div_dd(1, x);
}

// ---------------------------------------------------------------------------
// quad (op) double -> quad

inline xprec_ddouble xprec_addfast_qd(xprec_ddouble x, double y)
{
    // Algorithm 4 modified: cost 7 flops, error 2 u^2
    xprec_ddouble s = xprec_addfast_dd(x.hi, y);
    double v = x.lo + s.lo;
    return xprec_addfast_dd(s.hi, v);
}

inline xprec_ddouble xprec_add_qd(xprec_ddouble x, double y)
{
    // Algorithm 4: cost 10 flops, error 2 u^2
    xprec_ddouble s = xprec_add_dd(x.hi, y);
    double v = x.lo + s.lo;
    return xprec_addfast_dd(s.hi, v);
}

inline xprec_ddouble xprec_mul_qd(xprec_ddouble x, double y)
{
    // Algorithm 9: cost 6 flops, error 2 u^2
    xprec_ddouble c = xprec_mul_dd(x.hi, y);
    double cl3 = fma(x.lo, y, c.lo);
    return xprec_addfast_dd(c.hi, cl3);
}

inline xprec_ddouble xprec_div_qd(xprec_ddouble x, double y)
{
    // We could have used algorithm 15 here: cost 10 flops, error 3 u^2.
    // It turns out however by using fma, we can reduce this to 7 flops:
    //
    //    x / y = (x.hi + x.lo) / y = x.hi / y + x.lo / y .
    //
    // Defining the th = double(x.hi / y), we can rewrite this further as:
    //
    //    x / y = th + (x.hi - th * y) / y + x.lo / y ,
    //
    // where the second term can be computed to double precision by fma, and
    // the together with the third term they are scaled by u, so are safe to
    // compute in double precision.
    double th = x.hi / y;
    double rl = fma(-y, th, x.hi);
    rl += x.lo;
    double tl = rl / y;
    return xprec_addfast_dd(th, tl);
}

// ---------------------------------------------------------------------------
// double (op) quad -> quad

inline xprec_ddouble xprec_addfast_dq(double x, xprec_ddouble y)
{
    // Algorithm 4 modified: cost 7 flops, error 2 u^2
    xprec_ddouble s = xprec_addfast_dd(x, y.hi);
    double v = y.lo + s.lo;
    return xprec_addfast_dd(s.hi, v);
}

inline xprec_ddouble xprec_add_dq(double x, xprec_ddouble y)
{
    return xprec_add_qd(y, x);
}

inline xprec_ddouble xprec_mul_dq(double x, xprec_ddouble y)
{
    return xprec_mul_qd(y, x);
}

inline xprec_ddouble xprec_div_dq(double x, xprec_ddouble y)
{
    return xprec_mul_qd(xprec_reciprocal_q(y), x);
}

// ---------------------------------------------------------------------------
// quad (op) quad -> quad

inline xprec_ddouble xprec_addfast_qq(xprec_ddouble x, xprec_ddouble y)
{
    // Algorithm 6: cost 17 flops, error 3 u^2 + 13 u^3
    xprec_ddouble s = xprec_addfast_dd(x.hi, y.hi);
    xprec_ddouble t = xprec_add_dd(x.lo, y.lo);
    double c = s.lo + t.hi;
    xprec_ddouble v = xprec_addfast_dd(s.hi, c);
    double w = t.lo + v.lo;
    return xprec_addfast_dd(v.hi, w);
}

inline xprec_ddouble xprec_add_qq(xprec_ddouble x, xprec_ddouble y)
{
    // Algorithm 6: cost 20 flops, error 3 u^2 + 13 u^3
    xprec_ddouble s = xprec_add_dd(x.hi, y.hi);
    xprec_ddouble t = xprec_add_dd(x.lo, y.lo);
    double c = s.lo + t.hi;
    xprec_ddouble v = xprec_addfast_dd(s.hi, c);
    double w = t.lo + v.lo;
    return xprec_addfast_dd(v.hi, w);
}

inline xprec_ddouble xprec_mul_qq(xprec_ddouble x, xprec_ddouble y)
{
    // Algorithm 12: cost 9 flops, error 4 u^2 (corrected)
    xprec_ddouble c = xprec_mul_dd(x.hi, y.hi);
    double tl0 = x.lo * y.lo;
    double tl1 = fma(x.hi, y.lo, tl0);
    double cl2 = fma(x.lo, y.hi, tl1);
    double cl3 = c.lo + cl2;
    return xprec_addfast_dd(c.hi, cl3);
}

inline xprec_ddouble xprec_div_qq(xprec_ddouble x, xprec_ddouble y)
{
    return xprec_mul_qq(xprec_reciprocal_q(y), x);
}

inline xprec_ddouble xprec_reciprocal_q(xprec_ddouble y)
{
    // Part of Algorithm 18: cost 19 flops, error 2.3 u^2
    double th = 1.0 / y.hi;
    double rh = fma(-y.hi, th, 1.0);
    double rl = -y.lo * th;
    xprec_ddouble e = xprec_addfast_dd(rh, rl);
    xprec_ddouble delta = xprec_mul_qd(e, th);

    // This saves 3 flops w.r.t. algorithm 18, which uses standard addition.
    // We should be able to do this since Taylor expanding gives:
    //
    //  1/(xh + u*xl) = th * (1 + rh/th) * (1 + u * xl/xh + ...)
    //
    return xprec_addfast_dq(th, delta);
}
