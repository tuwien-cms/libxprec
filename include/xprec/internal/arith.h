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

// ---------------------------------------------------------------------------
// double (op) double -> quad

inline bool xprec_is_valid(xprec_ddouble x)
{
    return x.hi + x.lo == x.hi || !isfinite(x.hi);
}

// Compensated addition collapses to simple addition if the optimizer is
// permitted to break floating point rules and reassociate operations. This
// is the INSANE default on Intel icx, and equally insanely, often recommended
// as `-ffast-math` to coders as a make-my-program-faster hack.
//
// We set pragmas for GCC and MSVC here and for clang inside the function scope
// to override it. For GCC, that change must be permanent, since push_options
// stops GCC from inlining the corresponding functions.
//
#if defined(_MSC_VER)
#   pragma float_control(precise, on, push)
#elif defined(__clang__) || defined(__INTEL_LLVM_COMPILER)
    // do nothing
#elif defined(__GNUC__)
#   pragma GCC optimize ("-fno-associative-math")
#endif

inline xprec_ddouble xprec_addfast_dd(double a, double b)
{
    // M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
    // Algorithm 1: cost 3 flops
    #if defined(__clang__) || defined(__INTEL_LLVM_COMPILER)
    #   pragma clang fp reassociate (off)
    #endif
    double s = a + b;
    double z = s - a;
    double t = b - z;
    xprec_ddouble r = {s, t};
    return r;
}

inline xprec_ddouble xprec_add_dd(double a, double b)
{
    // Algorithm 2: cost 6 flops
    #if defined(__clang__) || defined(__INTEL_LLVM_COMPILER)
    #   pragma clang fp reassociate (off)
    #endif
    double s = a + b;
    double aprime = s - b;
    double bprime = s - aprime;
    double delta_a = a - aprime;
    double delta_b = b - bprime;
    double t = delta_a + delta_b;
    xprec_ddouble r = {s, t};
    return r;
}

// Restore old FP settings
#if defined(_MSC_VER)
#   pragma float_control(pop)
#endif

inline xprec_ddouble xprec_mul_dd(double a, double b)
{
    // Algorithm 3: cost 2 flops
    double pi = a * b;
    double rho = fma(a, b, -pi);
    xprec_ddouble r = {pi, rho};
    return r;
}

inline xprec_ddouble xprec_div_dd(double a, double b)
{
    // Cost 3 flops (2 of which divisions), observed error 1 u^2
    // Since we are rounding faithfully, the hi part is exact
    double th = a / b;

    // Multiply hi part with b and compare exactly to a to see difference
    double rl = fma(-b, th, a);
    double tl = rl / b;
    xprec_ddouble r = {th, tl};
    return r;
}

inline xprec_ddouble xprec_reciprocal_d(double x)
{
    return xprec_div_dd(1, x);
}

inline xprec_ddouble xprec_sqrt_d(double a)
{
    // Karp, Table II, cost 4 flops, error 1 u^2
    double y0 = sqrt(a);
    xprec_ddouble r = {y0, 0};
    if (a > DBL_MIN && isfinite(a)) {
        double delta_y = fma(-y0, y0, a) / y0;
        r.lo = 0.5 * delta_y;
    }
    return r;
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
// quad (op) power of two -> quad

inline xprec_ddouble xprec_add_pow2(xprec_ddouble a, double p)
{
    // This can be added quickly because the mantissa part is zero.
    return xprec_addfast_qd(a, p);
}

inline xprec_ddouble xprec_mul_pow2(xprec_ddouble a, double p)
{
    xprec_ddouble r = {a.hi * p, a.lo * p};
    return r;
}

inline xprec_ddouble xprec_div_pow2(xprec_ddouble a, double p)
{
    return xprec_mul_pow2(a, 1 / p);
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

inline xprec_ddouble xprec_neg(xprec_ddouble x)
{
    xprec_ddouble r = {-x.hi, -x.lo};
    return r;
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

inline xprec_ddouble xprec_sqrt_q(xprec_ddouble a)
{
    // Karp, Table II, cost 8 flops, error 2 u^2
    // The double result provides a approximation to sqrt(a). It performs
    // all the special-case handling, which is why we defer to it in these
    // cases.
    double y0 = sqrt(a.hi);
    if (a.hi <= 0 || !isfinite(a.hi)) {
        xprec_ddouble r = {y0, 0};
        return r;
    }

    // This is based on Newton-Ralphson for f(x) = a - 1/x^2:
    //
    //   x0 = approx(1/sqrt(A))
    //   x  = x + 0.5 * x * (1.0 - A * x * x)
    //
    double delta_y = (fma(-y0, y0, a.hi) + a.lo) / (2 * y0);

    // delta_y may alter the least significant digit of y0.
    return xprec_addfast_dd(y0, delta_y);
}

inline xprec_ddouble xprec_square(xprec_ddouble x)
{
    // Simple squaring algorithm
    // Cost 7 flops
    xprec_ddouble y = xprec_mul_dd(x.hi, x.hi);
    double y_lo = fma(2 * x.lo, x.hi, y.lo);
    return xprec_addfast_dd(y.hi, y_lo);
}
