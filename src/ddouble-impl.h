/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Most of the basic numerical algorithms are directly lifted from:
 * M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "ddouble.h"

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

inline DDouble ExDouble::fast_sum(double b) const {
    // M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
    // Algorithm 1: cost 3 flops
    double s = _x + b;
    double z = s - _x;
    double t = b - z;
    return DDouble(s, t);
}

// -------------------------------------------------------------------------
// ExDouble

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

inline DDouble operator/(PowerOfTwo x, ExDouble y)
{
    // Algorithm lifted from DoubleFloats.jl
    // TODO: figure out if this also works with arbitrary float x
    double a = x, b = y;

    // Compute high part first
    double hi = a / b;

    // We should have a == hi * b + lo * b. Store the difference, rounded
    // correctly:
    double lo = std::fma(-hi, b, a) / b;
    return DDouble(hi, lo);
}

inline DDouble sqrt(ExDouble x)
{
    // Lifted from DoubleFloats.jl
    double a = x;
    double hi = std::sqrt(a);
    double lo = std::fma(-hi, hi, a) / (2 * hi);
    return DDouble(hi, lo);
}

// -------------------------------------------------------------------------
// DDouble arithmetic

inline DDouble operator+(DDouble x, double y)
{
    // Algorithm 4: cost 10 flops, error 2 u^2
    DDouble s = ExDouble(x._hi) + y;
    double v = x._lo + s._lo;
    return ExDouble(s._hi).fast_sum(v);
}

inline DDouble operator+(DDouble x, DDouble y)
{
    // Algorithm 6: cost 20 flops, error 3 u^2 + 13 u^3
    DDouble s = ExDouble(x._hi) + y._hi;
    DDouble t = ExDouble(x._lo) + y._lo;
    double c = s._lo + t._hi;
    DDouble v = ExDouble(s._hi).fast_sum(c);
    double w = t._lo + v._lo;
    return ExDouble(v._hi).fast_sum(w);
}

inline DDouble operator*(DDouble x, double y)
{
    // Algorithm 9: cost 6 flops, error 2 u^2
    DDouble c = ExDouble(x._hi) * y;
    double cl3 = std::fma(x._lo, y, c._lo);
    return ExDouble(c.hi()).fast_sum(cl3);
}

inline DDouble operator*(DDouble x, DDouble y)
{
    // Algorithm 12: cost 9 flops, error 5 u^2
    DDouble c = ExDouble(x._hi) * y._hi;
    double tl0 = x._lo * y._lo;
    double tl1 = std::fma(x._hi, y._lo, tl0);
    double cl2 = std::fma(x._lo, y._hi, tl1);
    double cl3 = c._lo + cl2;
    return ExDouble(c._hi).fast_sum(cl3);
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
    return th.fast_sum(tl);
}

inline DDouble operator/(DDouble x, DDouble y)
{
    // Algorithm 18: cost 31 flops, error 10 u^2
    double th = 1 / y._hi;
    ExDouble rh = 1 - y._hi * th;
    ExDouble rl = -y._lo * th;
    DDouble e = rh.fast_sum(rl);
    DDouble delta = e * th;
    DDouble m = delta + th;
    return x * m;
}

inline DDouble operator*(DDouble x, PowerOfTwo y)
{
    return DDouble(x._hi * (double)y, x._lo * (double)y);
}

inline DDouble operator/(DDouble x, PowerOfTwo y)
{
    return DDouble(x._hi / (double)y, x._lo / (double)y);
}

// -------------------------------------------------------------------------
// DDouble relational operators

inline bool operator==(DDouble x, DDouble y)
{
    return x.hi() == y.hi() && x.lo() == y.lo();
}

inline bool operator!=(DDouble x, DDouble y)
{
    return x.hi() != y.hi() || x.lo() != y.lo();
}

inline bool operator<=(DDouble x, DDouble y)
{
    return x.hi() < y.hi() || (x.hi() == y.hi() && x.lo() <= y.lo());
}

inline bool operator<(DDouble x, DDouble y)
{
    return x.hi() < y.hi() || (x.hi() == y.hi() && x.lo() < y.lo());
}

inline bool operator>=(DDouble x, DDouble y)
{
    return x.hi() > y.hi() || (x.hi() == y.hi() && x.lo() >= y.lo());
}

inline bool operator>(DDouble x, DDouble y)
{
    return x.hi() > y.hi() || (x.hi() == y.hi() && x.lo() > y.lo());
}

// -------------------------------------------------------------------------
// DDouble basic functions

inline void swap(DDouble &x, DDouble &y)
{
    std::swap(x._hi, y._hi);
    std::swap(x._lo, y._lo);
}

inline DDouble ldexp(DDouble a, int n)
{
    return DDouble(std::ldexp(a.hi(), n), std::ldexp(a.lo(), n));
}

inline bool signbit(DDouble a)
{
    return std::signbit(a.hi());
}

inline DDouble copysign(DDouble mag, double sgn)
{
    // The sign is determined by the hi part, however, the sign of hi and lo
    // need not be the same, so we cannot merely broadcast copysign to both
    // parts.
    return signbit(mag) != std::signbit(sgn) ? -mag : mag;
}

inline DDouble copysign(DDouble mag, DDouble sgn)
{
    return copysign(mag, sgn.hi());
}

inline DDouble copysign(double mag, DDouble sgn)
{
    return DDouble(std::copysign(mag, sgn.hi()));
}

// -------------------------------------------------------------------------
// Numeric limits

constexpr DDouble std::numeric_limits<DDouble>::min() noexcept
{
    // Whereas the maximum exponent is the same for double and DDouble,
    // Denormalization in the low part means that the min exponent for
    // normalized values is lower.
    return DDouble(_double::min() / _double::epsilon());
}

constexpr DDouble std::numeric_limits<DDouble>::max() noexcept
{
    return DDouble(_double::max(),
                    _double::max() * _double::epsilon() / _double::radix);
}

constexpr DDouble std::numeric_limits<DDouble>::lowest() noexcept
{
    return DDouble(_double::lowest(),
                    _double::lowest() / _double::epsilon() / _double::radix);
}

constexpr DDouble std::numeric_limits<DDouble>::epsilon() noexcept
{
    return DDouble(_double::epsilon() * _double::epsilon() / _double::radix);
}

constexpr DDouble std::numeric_limits<DDouble>::round_error() noexcept
{
    return DDouble(_double::round_error());
}

constexpr DDouble std::numeric_limits<DDouble>::infinity() noexcept
{
    return DDouble(_double::infinity(), _double::infinity());
}

constexpr DDouble std::numeric_limits<DDouble>::quiet_NaN() noexcept
{
    return DDouble(_double::quiet_NaN(), _double::quiet_NaN());
}

constexpr DDouble std::numeric_limits<DDouble>::signaling_NaN() noexcept
{
    return DDouble(_double::signaling_NaN(), _double::signaling_NaN());
}

constexpr DDouble std::numeric_limits<DDouble>::denorm_min() noexcept
{
    return DDouble(_double::denorm_min());
}