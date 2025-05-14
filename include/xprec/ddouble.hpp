/* Small double-double arithmetic library.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <cmath>
#include <cstdint>
#include <iosfwd>
#include <limits>

#include "ddouble-fwd.hpp"
#include "ddouble.h"

namespace xprec {

/**
 * Class for double-double arithmetic.
 *
 * Emulates quadruple precision with a pair of doubles.  This roughly doubles
 * the mantissa bits (and thus squares the precision of double).  The range
 * is almost the same as double, with a larger area of denormalized numbers.
 *
 * The rough cost in floating point operations (flops) and relative error as
 * multiples of u² = 1.32e-32 (round-off error or half the machine epsilon) is
 * as follows:
 *
 *   | (op)       | (op)double | error | (op)DDouble | error |
 *   |------------|-----------:|------:|------------:|------:|
 *   | add_small  |    3 flops |   2u² |    17 flops |   3u² |
 *   | + -        |   10 flops |   2u² |    20 flops |   3u² |
 *   | *          |    6 flops |   2u² |     9 flops |   4u² |
 *   | /          |   10 flops |   3u² |    28 flops |   6u² |
 *   | reciprocal |   14 flops | 2.3u² |    19 flops | 2.3u² |
 *
 * The error bounds are tight analytical bounds [^2][^3], except in the case of
 * double-double division, where the bound is 10u² but largest observed error
 * is 6u². We report the largest observed error here [^1].
 *
 * [^1]: M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
 * [^2]: J.-M. Muller and L. Rideau, ACM Trans. Math. Softw. 48, 1, 9 (2022)
 * [^3]: The flop count has been reduced by 3 for divisons/reciprocals
 */
class DDouble {
public:
    constexpr DDouble(double x) : _hi(x), _lo(0.0) { }

    // Ensure that trivially_*_constructible work.
    DDouble() = default;
    DDouble(const DDouble &) = default;
    DDouble(DDouble &&) = default;
    DDouble &operator=(const DDouble &) = default;
    DDouble &operator=(DDouble &&) = default;
    ~DDouble() = default;

    /**
     * Construct DDouble from hi and low part.
     *
     * WARNING: You MUST ensure that abs(hi) > epsilon * abs(lo).
     */
    constexpr DDouble(double hi, double lo) : _hi(hi), _lo(lo) { }

    constexpr DDouble(xprec_ddouble x) : _hi(x.hi), _lo(x.lo) { }

    /** Convert DDouble to different type */
    template <typename T>
    constexpr T as() const
    {
        return static_cast<T>(_hi) + static_cast<T>(_lo);
    }

    constexpr operator xprec_ddouble() const { return xprec_ddouble{_hi, _lo}; }
    constexpr explicit operator double() const { return _hi; }

    /** Get high part of a ddouble */
    constexpr double hi() const { return _hi; }

    /** Get low part of a ddouble */
    constexpr double lo() const { return _lo; }

    /**
     * Add small number to this.
     *
     * WARNING: You must ensure that b is small than this in magnitude!
     */
    DDouble add_small(double y) { return xprec_addfast_qd(*this, y); }

    /**
     * Add small number to this.
     *
     * WARNING: You must ensure that b is small than this in magnitude!
     */
    DDouble add_small(DDouble y) { return xprec_addfast_qq(*this, y); }

    DDouble &operator+=(double y) { return *this = xprec_add_qd(*this, y); }
    DDouble &operator-=(double y) { return *this = xprec_add_qd(*this, -y); }
    DDouble &operator*=(double y) { return *this = xprec_mul_qd(*this, y); }
    DDouble &operator/=(double y) { return *this = xprec_div_qd(*this, y); }

    DDouble &operator+=(DDouble y) { return *this = xprec_add_qq(*this, y); }
    DDouble &operator-=(DDouble y) { return *this = xprec_add_qq(*this, xprec_neg(y)); }
    DDouble &operator*=(DDouble y) { return *this = xprec_mul_qq(*this, y); }
    DDouble &operator/=(DDouble y) { return *this = xprec_div_qq(*this, y); }

    DDouble &operator*=(PowerOfTwo y);
    DDouble &operator/=(PowerOfTwo y);

    friend void swap(DDouble &x, DDouble &y);

    friend std::ostream &operator<<(std::ostream &out, DDouble x);

private:
    double _hi;
    double _lo;
};

inline DDouble operator+(DDouble x) { return x; }
inline DDouble operator-(DDouble x) { return xprec_neg(x); }

inline DDouble operator+(DDouble x, double y) { return xprec_add_qd(x, y); }
inline DDouble operator+(DDouble x, DDouble y) { return xprec_add_qq(x, y); }
inline DDouble operator+(double x, DDouble y) { return  xprec_add_dq(x, y); }

inline DDouble operator-(DDouble x, double y) { return xprec_add_qd(x, -y); }
inline DDouble operator-(double x, DDouble y) { return xprec_add_dq(x, -y); }
inline DDouble operator-(DDouble x, DDouble y) { return xprec_add_qq(x, -y); }

inline DDouble operator*(DDouble x, double y) { return xprec_mul_qd(x, y); }
inline DDouble operator*(DDouble x, DDouble y) { return xprec_mul_qq(x, y); }
inline DDouble operator*(double x, DDouble y) { return xprec_mul_dq(x, y); }

inline DDouble operator/(double x, DDouble y) { return xprec_div_dq(x, y); }
inline DDouble operator/(DDouble x, double y) { return xprec_div_qd(x, y); }
inline DDouble operator/(DDouble x, DDouble y) { return xprec_div_qq(x, y); }

inline DDouble reciprocal(DDouble y) { return xprec_reciprocal_q(y); }

inline bool operator==(DDouble x, DDouble y);
inline bool operator!=(DDouble x, DDouble y);
inline bool operator<=(DDouble x, DDouble y);
inline bool operator<(DDouble x, DDouble y);
inline bool operator>=(DDouble x, DDouble y);
inline bool operator>(DDouble x, DDouble y);

inline bool operator==(DDouble x, double y) { return x == DDouble(y); }
inline bool operator!=(DDouble x, double y) { return x != DDouble(y); }
inline bool operator<=(DDouble x, double y) { return x <= DDouble(y); }
inline bool operator>=(DDouble x, double y) { return x >= DDouble(y); }
inline bool operator>(DDouble x, double y) { return x > DDouble(y); }

inline bool operator==(double x, DDouble y) { return DDouble(x) == y; }
inline bool operator!=(double x, DDouble y) { return DDouble(x) != y; }
inline bool operator<=(double x, DDouble y) { return DDouble(x) <= y; }
inline bool operator>=(double x, DDouble y) { return DDouble(x) >= y; }
inline bool operator>(double x, DDouble y) { return DDouble(x) > y; }

/**
 * Class wrapping a double, but marking it for extended precision computation
 *
 * Doing arithmetic with these objects, even though they hold only a double,
 * will return a DDouble object and be accurate within double-double
 * arithmetic. However, it is usually much faster than first converting an
 * operand to a full double-double.
 *
 * Note that this means that arithmetic with these object DOES NOT return
 * an object of this ExDouble, and therefore also in-place operators are not
 * defined.
 */
class ExDouble {
public:
    constexpr ExDouble(double x) : _x(x) { }

    constexpr explicit operator double() const { return _x; }

    friend ExDouble operator+(ExDouble a) { return ExDouble(+a._x); }
    friend ExDouble operator-(ExDouble a) { return ExDouble(-a._x); }

    /**
     * Add small number to this.
     *
     * WARNING: You must ensure that b is small than this in magnitude!
     */
    DDouble add_small(double b) const { return xprec_addfast_dd(_x, b); }
    DDouble add_small(DDouble b) const { return xprec_addfast_dq(_x, b); }


private:
    double _x;
};

inline DDouble operator+(ExDouble a, ExDouble b) { return xprec_add_dd((double)a, (double)b); }
inline DDouble operator-(ExDouble a, ExDouble b) { return xprec_add_dd((double)a, -(double)b);}
inline DDouble operator*(ExDouble a, ExDouble b) { return xprec_mul_dd((double)a, (double)b); }
inline DDouble operator/(ExDouble a, ExDouble b) { return xprec_div_dd((double)a, (double)b); }

inline DDouble reciprocal(ExDouble y) { return xprec_reciprocal_d((double)y); }
inline DDouble sqrt(ExDouble y) { return xprec_sqrt_d((double)y); }

/**
 * Class for wrapping a power of two.
 *
 * Double-double can be multiplied by powers of two much quicker and at no
 * loss of precision, since we simply scale all hunks.
 */
class PowerOfTwo {
public:
    /**
     * Initialize power of two.
     *
     * WARNING: You MUST ensure that x is a power of two or zero.
     */
    constexpr explicit PowerOfTwo(double x) : _x(x) { }

    constexpr operator double() const { return _x; }

private:
    double _x;
};

inline PowerOfTwo operator+(PowerOfTwo x) { return x; }
inline PowerOfTwo operator-(PowerOfTwo x) { return PowerOfTwo((double)(-x)); }

inline DDouble operator+(PowerOfTwo x, DDouble y) { return xprec_add_pow2(y, (double)x); }
inline DDouble operator+(DDouble y, PowerOfTwo x) { return xprec_add_pow2(y, (double)x); }
inline DDouble operator-(PowerOfTwo x, DDouble y) { return xprec_add_pow2(xprec_neg(y), (double)x); }
inline DDouble operator-(DDouble x, PowerOfTwo y) { return xprec_add_pow2(x, (double)(-y)); }

inline double operator*(PowerOfTwo a, double b) { return (double)a * b; }
inline double operator*(double a, PowerOfTwo b) { return a * (double)b; }
inline DDouble operator*(DDouble x, PowerOfTwo y) { return xprec_mul_pow2(x, (double)y); }
inline DDouble operator*(PowerOfTwo x, DDouble y) { return xprec_mul_pow2(y, (double)x); }
inline PowerOfTwo operator*(PowerOfTwo a, PowerOfTwo b) { return PowerOfTwo((double)a * (double)b); }

inline double operator/(double a, PowerOfTwo b) { return a / (double)b; }
inline DDouble operator/(DDouble x, PowerOfTwo y) { return xprec_div_pow2(x, (double)y); }
inline PowerOfTwo operator/(PowerOfTwo a, PowerOfTwo b) { return PowerOfTwo((double)a / (double)b); }

inline DDouble &DDouble::operator*=(PowerOfTwo y) { return *this = xprec_mul_pow2(*this, (double)y); }
inline DDouble &DDouble::operator/=(PowerOfTwo y) { return *this = xprec_div_pow2(*this, (double)y); }

inline PowerOfTwo ldexp(PowerOfTwo x, int m) { return PowerOfTwo(std::ldexp((double) x, m)); }
inline PowerOfTwo reciprocal(PowerOfTwo x) { return PowerOfTwo(1.0 / (double)x); }


// C++ forbids overloading functions in the std namespace, which is why we
// define it outside of that.
//
// Type-generic code should use argument-dependent lookup (ADL), i.e., use
// "using std::sin" and then call "sin".

DDouble abs(DDouble a);
inline DDouble acos(DDouble a) { return xprec_acos(a); }
DDouble acosh(DDouble a);
inline DDouble asin(DDouble a)  { return xprec_asin(a); }
DDouble asinh(DDouble a);
inline DDouble atan(DDouble a)  { return xprec_atan(a); }
inline DDouble atan2(DDouble a, DDouble b)  { return xprec_atan2(a, b); }
DDouble atanh(DDouble a);
DDouble ceil(DDouble a);
inline DDouble cos(DDouble a) { return xprec_cos(a); }
DDouble cosh(DDouble a);
DDouble exp(DDouble a);
DDouble expm1(DDouble a);
DDouble fabs(DDouble a);
DDouble fmax(DDouble a, DDouble b);
DDouble fmin(DDouble a, DDouble b);
DDouble floor(DDouble a);
DDouble hypot(DDouble a, DDouble b);
DDouble ldexp(DDouble a, int m);
DDouble log(DDouble a);
DDouble log1p(DDouble a);
DDouble logb(DDouble a);
DDouble modf(DDouble a, DDouble *b);
DDouble nextafter(DDouble a, DDouble b);
DDouble pow(DDouble a, DDouble b);
DDouble pow(DDouble a, int b);
DDouble round(DDouble a);
DDouble scalbn(DDouble a, int m);
inline DDouble sin(DDouble a) { return xprec_sin(a); }
DDouble sinh(DDouble a);
DDouble sqrt(DDouble a);
inline DDouble tan(DDouble a) { return xprec_tan(a); }
DDouble tanh(DDouble a);

int fpclassify(DDouble x);
int ilogb(DDouble x);
bool isfinite(DDouble x);
bool isinf(DDouble x);
bool isnan(DDouble x);
bool isnormal(DDouble x);
bool iszero(DDouble x);

/**
 * Gauss-Chebyshev quadrature rule.
 *
 * Expects x and (optionally) w to be arrays of at least size n. Fill x with
 * the Type-I Gauss-Chebyshev quadrature nodes of order n, i.e., the roots of
 * the n-th Chebyshev polynomial. If w is given, store the quadrature weights
 * there.
 */
void gauss_chebyshev(int n, DDouble x[], DDouble w[] = nullptr);

/**
 * Gauss-Legendre quadrature rule.
 *
 * Expects x and (optionally) w to be arrays of at least size n. Fill x with
 * the Gauss-Legendre quadrature nodes of order n, i.e., the roots of the n-th
 * Legendre polynomial. If w is given, store the quadrature weights there.
 */
void gauss_legendre(int n, DDouble x[], DDouble w[] = nullptr);

/** Trigonometric complement sqrt(1 - x*x) to full precision. */
DDouble trig_complement(DDouble x);

} /* namespace xprec*/

namespace std {

/**
 * Specialization of numerical limits for the double-double type.
 */
template <>
class numeric_limits<xprec::DDouble> {
    using DDouble = xprec::DDouble;
    using _double = numeric_limits<double>;

public:
    static constexpr bool is_specialized = true;
    static constexpr bool is_signed = true;
    static constexpr bool is_integer = false;
    static constexpr bool is_exact = false;

    static constexpr bool has_infinity = _double::has_infinity;
    static constexpr bool has_quiet_NaN = _double::has_quiet_NaN;
    static constexpr bool has_signaling_NaN = _double::has_signaling_NaN;
    static constexpr float_denorm_style has_denorm = _double::has_denorm;
    static constexpr bool has_denorm_loss = false;

    static constexpr float_round_style round_style = _double::round_style;
    static constexpr int digits = 2 * _double::digits + 1;
    static constexpr int digits10 = 2 * _double::digits10;
    static constexpr int max_digits10 = 2 * _double::max_digits10;

    static constexpr int radix = _double::radix;

    static constexpr int min_exponent = _double::min_exponent + _double::digits;
    static constexpr int min_exponent10 =
        _double::min_exponent10 + _double::digits10;
    static constexpr int max_exponent = _double::max_exponent;
    static constexpr int max_exponent10 = _double::max_exponent10;

    static constexpr DDouble min() noexcept;
    static constexpr DDouble max() noexcept;
    static constexpr DDouble lowest() noexcept;
    static constexpr DDouble epsilon() noexcept;
    static constexpr DDouble round_error() noexcept;

    static constexpr DDouble infinity() noexcept;
    static constexpr DDouble quiet_NaN() noexcept;
    static constexpr DDouble signaling_NaN() noexcept;
    static constexpr DDouble denorm_min() noexcept;

    static constexpr bool is_bounded = numeric_limits<double>::is_bounded;
    static constexpr bool is_iec559 = false;
    static constexpr bool is_modulo = false;

    static constexpr bool traps = false;
    static constexpr bool tinyness_before = false;
};

} /* namespace std */

#include "internal/arith.hpp"
#include "internal/checks.hpp"
#include "internal/functions.hpp"
#include "internal/limits.hpp"
