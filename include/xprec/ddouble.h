/* Small double-double arithmetic library.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <cmath>
#include <cstdint>
#include <limits>
#include <iosfwd>

#if __cpp_lib_math_constants >= 201907L
#include <numbers>
#endif

#include "version.h"

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
 *   | /          |   10 flops |   3u² |    31 flops |   6u² |
 *   | reciprocal |   14 flops | 2.3u² |    22 flops | 2.3u² |
 *
 * The error bounds are tight analytical bounds [^2], except in the case of
 * double-double division, where the bound is 10u² but largest observed error
 * is 6u². We report the largest observed error here [^1].
 *
 * [^1]: M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
 * [^2]: J.-M. Muller and L. Rideau, ACM Trans. Math. Softw. 48, 1, 9 (2022)
 */
class DDouble {
public:
    constexpr DDouble(double x) : _hi(x), _lo(0.0) { }
    constexpr DDouble(long double x) : _hi(x), _lo(x - _hi) { }

    // TODO: requires extended precision to work.
    constexpr DDouble(int64_t x) : _hi((double)x), _lo((long double)x - _hi) {}
    constexpr DDouble(uint64_t x) : _hi((double)x), _lo((long double)x - _hi) {}
    constexpr DDouble(int32_t x) : _hi(x), _lo(0.0) { }
    constexpr DDouble(uint32_t x) : _hi(x), _lo(0.0) { }
    constexpr DDouble(int16_t x) : _hi(x), _lo(0.0) { }
    constexpr DDouble(uint16_t x) : _hi(x), _lo(0.0) { }

    // Ensure that trivially_*_constructible work.
    DDouble() = default;
    DDouble(const DDouble&) = default;
    DDouble(DDouble&&) = default;
    DDouble &operator=(const DDouble &other) = default;
    DDouble &operator=(DDouble &&other) = default;
    ~DDouble() = default;

    /**
     * Construct DDouble from hi and low part.
     *
     * WARNING: You MUST ensure that abs(hi) > epsilon * abs(lo).
     */
    constexpr DDouble(double hi, double lo) : _hi(hi), _lo(lo) { }

    /** Convert DDouble to different type */
    template <typename T>
    constexpr T as() const { return static_cast<T>(_hi) + static_cast<T>(_lo); }

    /** Get high part of a ddouble */
    constexpr double hi() const { return _hi; }

    /** Get low part of a ddouble */
    constexpr double lo() const { return _lo; }

    /**
     * Add small number to this.
     *
     * WARNING: You must ensure that b is small than this in magnitude!
     */
    DDouble add_small(double y);

    /**
     * Add small number to this.
     *
     * WARNING: You must ensure that b is small than this in magnitude!
     */
    DDouble add_small(DDouble y);

    friend DDouble operator+(DDouble x, double y);
    friend DDouble operator+(DDouble x, DDouble y);
    friend DDouble operator+(double x, DDouble y) { return y + x; }

    friend DDouble operator-(DDouble x, double y) { return x + (-y); }
    friend DDouble operator-(double x, DDouble y) { return x + (-y); }
    friend DDouble operator-(DDouble x, DDouble y) { return x + (-y); }

    friend DDouble operator+(DDouble x) { return x; }
    friend DDouble operator-(DDouble x) { return DDouble(-x._hi, -x._lo); }

    friend DDouble operator*(DDouble x, double y);
    friend DDouble operator*(DDouble x, DDouble y);
    friend DDouble operator*(double x, DDouble y) { return y * x; }

    friend DDouble operator/(double x, DDouble y);
    friend DDouble operator/(DDouble x, double y);
    friend DDouble operator/(DDouble x, DDouble y);

    friend DDouble operator*(DDouble x, PowerOfTwo y);
    friend DDouble operator*(PowerOfTwo x, DDouble y);
    friend DDouble operator/(DDouble x, PowerOfTwo y);

    friend DDouble reciprocal(DDouble y);

    DDouble &operator+=(double y) { return *this = *this + y; }
    DDouble &operator-=(double y) { return *this = *this - y; }
    DDouble &operator*=(double y) { return *this = *this * y; }
    DDouble &operator/=(double y) { return *this = *this / y; }

    DDouble &operator+=(DDouble y) { return *this = *this + y; }
    DDouble &operator-=(DDouble y) { return *this = *this - y; }
    DDouble &operator*=(DDouble y) { return *this = *this * y; }
    DDouble &operator/=(DDouble y) { return *this = *this / y; }

    DDouble &operator*=(PowerOfTwo y);
    DDouble &operator/=(PowerOfTwo y);

    friend bool operator==(DDouble x, DDouble y);
    friend bool operator!=(DDouble x, DDouble y);
    friend bool operator<=(DDouble x, DDouble y);
    friend bool operator< (DDouble x, DDouble y);
    friend bool operator>=(DDouble x, DDouble y);
    friend bool operator> (DDouble x, DDouble y);

    friend bool operator==(DDouble x, double y) { return x == DDouble(y); }
    friend bool operator!=(DDouble x, double y) { return x != DDouble(y); }
    friend bool operator<=(DDouble x, double y) { return x <= DDouble(y); }
    friend bool operator>=(DDouble x, double y) { return x >= DDouble(y); }
    friend bool operator> (DDouble x, double y) { return x > DDouble(y); }

    friend bool operator==(double x, DDouble y) { return DDouble(x) == y; }
    friend bool operator!=(double x, DDouble y) { return DDouble(x) != y; }
    friend bool operator<=(double x, DDouble y) { return DDouble(x) <= y; }
    friend bool operator>=(double x, DDouble y) { return DDouble(x) >= y; }
    friend bool operator> (double x, DDouble y) { return DDouble(x) > y; }

    friend void swap(DDouble &x, DDouble &y);

    friend std::ostream &operator<<(std::ostream &out, DDouble x);

private:
    double _hi;
    double _lo;
};

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

    constexpr operator double () const { return _x; }

    friend ExDouble operator+(ExDouble a) { return ExDouble(+a._x); }
    friend ExDouble operator-(ExDouble a) { return ExDouble(-a._x); }

    /**
     * Add small number to this.
     *
     * WARNING: You must ensure that b is small than this in magnitude!
     */
    DDouble add_small(double b) const;

    friend DDouble operator+(ExDouble a, ExDouble b);
    friend DDouble operator+(double a, ExDouble b);
    friend DDouble operator+(ExDouble a, double b);

    friend DDouble operator-(ExDouble a, ExDouble b);
    friend DDouble operator-(ExDouble a, double b);
    friend DDouble operator-(double a, ExDouble b);

    friend DDouble operator*(ExDouble a, ExDouble b);
    friend DDouble operator*(double a, ExDouble b);
    friend DDouble operator*(ExDouble a, double b);

    friend DDouble operator/(ExDouble a, ExDouble b);
    friend DDouble operator/(double a, ExDouble b);
    friend DDouble operator/(ExDouble a, double b);

    friend DDouble reciprocal(ExDouble y);

    bool is_small(double x) const;

private:
    double _x;
};

/**
 * Class for wrapping a power of two.
 *
 * Double-double can be multiplied by powers of two much quicker and at no
 * loss of precision, since we simply scale all hunks.
 */
class PowerOfTwo {
public:
    explicit PowerOfTwo(int n) : _x(std::ldexp(1.0, n)) { }

    friend PowerOfTwo operator*(PowerOfTwo a, PowerOfTwo b);
    friend PowerOfTwo operator/(PowerOfTwo a, PowerOfTwo b);

    constexpr operator double () const { return _x; }

private:
    explicit constexpr PowerOfTwo(double x) : _x(x) { }

    double _x;
};

// C++ forbids overloading functions in the std namespace, which is why we
// define it outside of that.
//
// Type-generic code should use argument-dependent lookup (ADL), i.e., use
// "using std::sin" and then call "sin".

DDouble abs(DDouble a);
DDouble acos(DDouble a);
DDouble acosh(DDouble a);
DDouble asin(DDouble a);
DDouble asinh(DDouble a);
DDouble atan(DDouble a);
DDouble atan2(DDouble a, DDouble b);
DDouble atanh(DDouble a);
DDouble ceil(DDouble a);
DDouble cos(DDouble a);
DDouble cosh(DDouble a);
DDouble exp(DDouble a);
DDouble expm1(DDouble a);
DDouble fabs(DDouble a);
DDouble floor(DDouble a);
DDouble hypot(DDouble a, DDouble b);
DDouble ldexp(DDouble a, int m);
DDouble log(DDouble a);
DDouble log1p(DDouble a);
DDouble modf(DDouble a, DDouble *b);
DDouble pow(DDouble a, DDouble b);
DDouble pow(DDouble a, int b);
DDouble round(DDouble a);
DDouble sin(DDouble a);
DDouble sinh(DDouble a);
DDouble sqrt(DDouble a);
DDouble tan(DDouble a);
DDouble tanh(DDouble a);

int fpclassify(DDouble x);
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

namespace std {

/**
 * Specialization of numerical limits for the double-double type.
 */
template <>
class numeric_limits<DDouble> {
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

    static constexpr int min_exponent =
                            _double::min_exponent + _double::digits;
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

#if __cpp_lib_math_constants >= 201907L

namespace std { namespace number {

inline constexpr DDouble e_v{2.718281828459045, 1.4456468917292502e-16};
inline constexpr DDouble log2e_v{1.4426950408889634, 2.0355273740931033e-17};
inline constexpr DDouble log10e_v{0.4342944819032518, 1.098319650216765e-17};
inline constexpr DDouble pi_v{3.141592653589793, 1.2246467991473532e-16};
inline constexpr DDouble inv_pi_v{0.3183098861837907, -1.9678676675182486e-17};
inline constexpr DDouble inv_sqrtpi_v{0.5641895835477563, 7.66772980658294e-18};
inline constexpr DDouble ln2_v{0.6931471805599453, 2.3190468138462996e-17};
inline constexpr DDouble ln10_v{2.302585092994046, -2.1707562233822494e-16};
inline constexpr DDouble sqrt2_v{1.4142135623730951, -9.667293313452913e-17};
inline constexpr DDouble sqrt3_v{1.7320508075688772, 1.0035084221806903e-16};
inline constexpr DDouble inv_sqrt3_v{0.5773502691896257, 3.3450280739356345e-17};
inline constexpr DDouble egamma_v{0.5772156649015329, -4.942915152430645e-18};
inline constexpr DDouble phi_v{1.618033988749895, -5.432115203682506e-17};

}} /* namespace std::number */

#endif

#include "internal/arith.h"
#include "internal/checks.h"
#include "internal/functions.h"
#include "internal/limits.h"
