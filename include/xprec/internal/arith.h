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

/**
 * Return true if x is greater or equal in magnitude as y.
 *
 * Return true if the xponent of y does not exceed the exponent of x.  NaN
 * and Inf are considered maximum magnitude, 0 is considered minimum magnitude.
 */
inline bool greater_in_magnitude(double x, double y)
{
    static_assert(std::numeric_limits<double>::is_iec559, "needs IEEE floats");
    union {
        double number;
        uint64_t pattern;
    } x_u = {x}, y_u = {y};

    // Shift out sign bit
    return (x_u.pattern << 1) >= (y_u.pattern << 1);
}

inline bool greater_in_magnitude(DDouble x, DDouble y)
{
    return greater_in_magnitude(x.hi(), y.hi());
}

inline bool greater_in_magnitude(DDouble x, double y)
{
    return greater_in_magnitude(x.hi(), y);
}

inline bool greater_in_magnitude(double x, DDouble y)
{
    return greater_in_magnitude(x, y.hi());
}

// -------------------------------------------------------------------------
// PowerOfTwo

inline PowerOfTwo operator*(PowerOfTwo a, PowerOfTwo b)
{
    return PowerOfTwo(a._x * b._x);
}

inline PowerOfTwo operator/(PowerOfTwo a, PowerOfTwo b)
{
    return PowerOfTwo(a._x / b._x);
}

// -------------------------------------------------------------------------
// ExDouble

inline bool ExDouble::is_small(double b) const
{
    static_assert(std::numeric_limits<double>::is_iec559, "needs IEEE floats");
    static const uint64_t exp_mask = 0x7ff0000000000000UL;

    union {
        double number;
        uint64_t pattern;
    } x_u = {_x}, b_u = {b};
    return (x_u.pattern & exp_mask) >= (b_u.pattern & exp_mask);
}

inline DDouble ExDouble::add_small(double b) const
{
    // M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
    // Algorithm 1: cost 3 flops
    //assert(is_small(b));
    double s = _x + b;
    double z = s - _x;
    double t = b - z;
    return DDouble(s, t);
}

inline DDouble operator+(ExDouble a, ExDouble b)
{
    // Algorithm 2: cost 6 flops
    double s = (double) a + (double) b;
    double aprime = s - (double) b;
    double bprime = s - aprime;
    double delta_a = (double) a - aprime;
    double delta_b = (double) b - bprime;
    double t = delta_a + delta_b;
    return DDouble(s, t);
}

// We must place this here rather than in-place in the class, because
// the type DDouble is still incomplete

inline DDouble operator+(double a, ExDouble b) { return ExDouble(a) + b; }
inline DDouble operator+(ExDouble a, double b) { return a + ExDouble(b); }

inline DDouble operator-(ExDouble a, ExDouble b) { return a + (-b); }
inline DDouble operator-(ExDouble a, double b) { return a + (-b); }
inline DDouble operator-(double a, ExDouble b) { return a + (-b); }

inline DDouble operator*(ExDouble a, ExDouble b)
{
    // Algorithm 3: cost 2 flops
    double pi = (double) a * (double) b;
    double rho = std::fma((double) a, (double) b, -pi);
    return DDouble(pi, rho);
}

inline DDouble operator*(double a, ExDouble b) { return ExDouble(a) * b; }
inline DDouble operator*(ExDouble a, double b) { return a * ExDouble(b); }

inline DDouble reciprocal(ExDouble y)
{
    // Part of Algorithm 18 for y_lo = 0
    double th = 1.0 / (double) y;
    double rh = std::fma(-(double) y, th, 1.0);
    DDouble delta = ExDouble(rh) * th;
    return delta + th;
}

inline DDouble operator/(ExDouble a, ExDouble b)
{
    // Algorithm 18 for this special case
    return reciprocal(a) * (double)b;
}

inline DDouble operator/(double a, ExDouble b) { return ExDouble(a) / b; }
inline DDouble operator/(ExDouble a, double b) { return a / ExDouble(b); }

// -------------------------------------------------------------------------
// DDouble arithmetic

inline DDouble operator+(DDouble x, double y)
{
    // Algorithm 4: cost 10 flops, error 2 u^2
    DDouble s = ExDouble(x._hi) + y;
    double v = x._lo + s._lo;
    return ExDouble(s._hi).add_small(v);
}

inline DDouble operator+(DDouble x, DDouble y)
{
    // Algorithm 6: cost 20 flops, error 3 u^2 + 13 u^3
    DDouble s = ExDouble(x._hi) + y._hi;
    DDouble t = ExDouble(x._lo) + y._lo;
    double c = s._lo + t._hi;
    DDouble v = ExDouble(s._hi).add_small(c);
    double w = t._lo + v._lo;
    return ExDouble(v._hi).add_small(w);
}

inline DDouble DDouble::add_small(double y)
{
    // Algorithm 4 modified: cost 7 flops, error 2 u^2
    DDouble s = ExDouble(_hi).add_small(y);
    double v = _lo + s._lo;
    return ExDouble(s._hi).add_small(v);
}

inline DDouble DDouble::add_small(DDouble y)
{
    // Algorithm 6: cost 17 flops, error 3 u^2 + 13 u^3
    DDouble s = ExDouble(_hi).add_small(y._hi);
    DDouble t = ExDouble(_lo) + y._lo;
    double c = s._lo + t._hi;
    DDouble v = ExDouble(s._hi).add_small(c);
    double w = t._lo + v._lo;
    return ExDouble(v._hi).add_small(w);
}

inline DDouble operator*(DDouble x, double y)
{
    // Algorithm 9: cost 6 flops, error 2 u^2
    DDouble c = ExDouble(x._hi) * y;
    double cl3 = std::fma(x._lo, y, c._lo);
    return ExDouble(c.hi()).add_small(cl3);
}

inline DDouble operator*(DDouble x, DDouble y)
{
    // Algorithm 12: cost 9 flops, error 4 u^2 (corrected)
    DDouble c = ExDouble(x._hi) * y._hi;
    double tl0 = x._lo * y._lo;
    double tl1 = std::fma(x._hi, y._lo, tl0);
    double cl2 = std::fma(x._lo, y._hi, tl1);
    double cl3 = c._lo + cl2;
    return ExDouble(c._hi).add_small(cl3);
}

inline DDouble operator/(DDouble x, double y)
{
    // Algorithm 15: cost 10 flops, error 3 u^2
    ExDouble th = x._hi / y;
    DDouble pi = th * y;
    double delta_h = x._hi - pi._hi;
    double delta_tee = delta_h - pi._lo;
    double delta = delta_tee + x._lo;
    double tl = delta / y;
    return th.add_small(tl);
}

inline DDouble reciprocal(DDouble y)
{
    // Part of Algorithm 18: cost 22 flops, error 2.3 u^2
    double th = 1.0 / y._hi;
    double rh = std::fma(-y._hi, th, 1.0);
    double rl = -y._lo * th;
    DDouble e = ExDouble(rh).add_small(rl);
    DDouble delta = e * th;
    return delta + th;
}

inline DDouble operator/(DDouble x, DDouble y)
{
    // Algorithm 18: cost 31 flops, error 10 u^2 (6 u^2 obs.)
    return x * reciprocal(y);
}

inline DDouble operator/(double x, DDouble y)
{
    // Algorithm 18: cost 28 flops
    return x * reciprocal(y);
}

inline DDouble operator*(DDouble x, PowerOfTwo y)
{
    return DDouble(x._hi * (double)y, x._lo * (double)y);
}

inline DDouble operator*(PowerOfTwo x, DDouble y)
{
    return y * x;
}

inline DDouble operator/(DDouble x, PowerOfTwo y)
{
    return DDouble(x._hi / (double)y, x._lo / (double)y);
}

inline DDouble &DDouble::operator*=(PowerOfTwo y)
{
    return *this = *this * y;
}

inline DDouble &DDouble::operator/=(PowerOfTwo y)
{
    return *this = *this / y;
}
