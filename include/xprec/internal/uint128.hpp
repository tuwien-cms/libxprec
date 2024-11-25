/* Portable unsigned 128-bit integer type
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Copyright (C) 2024 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once

#include <cstdint>
#include <functional>
#ifdef _MSC_VER
    #include <intrin.h>
#endif

namespace xprec {
namespace _internal {

/**
 * Portable 128-bit unsigned integer type.
 */
struct UInt128 {
public:
    // constructors
    constexpr UInt128(uint64_t x) : lo(x), hi() { }
    constexpr UInt128(uint64_t hi, uint64_t lo) : lo(lo), hi(hi)  { }

    // conversions
    constexpr explicit operator unsigned short() const { return lo; }
    constexpr explicit operator unsigned int() const { return lo; }
    constexpr explicit operator unsigned long() const { return lo; }

    // operations
    friend UInt128 operator+(UInt128 x, UInt128 y);
    friend UInt128 operator-(UInt128 x, UInt128 y);
    friend UInt128 operator*(UInt128 x, UInt128 y);
    friend UInt128 operator<<(UInt128 x, int n);

    UInt128 &operator+=(UInt128 x) { return *this = *this + x; }
    UInt128 &operator-=(UInt128 x) { return *this = *this - x; }
    UInt128 &operator*=(UInt128 x) { return *this = *this * x; }
    UInt128 &operator<<=(int n) { return *this = *this << n; }

    // relations
    friend constexpr bool operator==(UInt128 x, UInt128 y);
    friend constexpr bool operator!=(UInt128 x, UInt128 y);
    friend constexpr bool operator<(UInt128 x, UInt128 y);
    friend constexpr bool operator<=(UInt128 x, UInt128 y);
    friend constexpr bool operator>(UInt128 x, UInt128 y);
    friend constexpr bool operator>=(UInt128 x, UInt128 y);

private:
    uint64_t lo, hi;

    template <typename F>
    static constexpr UInt128 binary(F op, UInt128 x, UInt128 y)
    {
        return UInt128(op(x.hi, y.hi), op(x.lo, y.lo));
    }

    template <typename F>
    static constexpr bool compare(F op, UInt128 x, UInt128 y)
    {
        return x.hi != y.hi ? op(x.hi, y.hi) : op(x.lo, y.lo);
    }
};

inline UInt128 operator+(UInt128 x, UInt128 y)
{
    UInt128 r = UInt128::binary(std::plus<uint64_t>(), x, y);
    if (r.lo < x.lo)
        r.hi++;
    return r;
}

inline UInt128 operator-(UInt128 x, UInt128 y)
{
    UInt128 r = UInt128::binary(std::minus<uint64_t>(), x, y);
    if (x.lo < y.lo)
        r.hi--;
    return r;
}

#ifdef _M_X64

inline UInt128 _mul_widen(uint64_t x, uint64_t y)
{
    // for MSVC, this is fastest
    uint64_t hi, lo;
    lo = _umul128(x, y, &hi);
    return UInt128(hi, lo);
}

#else

inline UInt128 _mul_widen(uint64_t x, uint64_t y)
{
    // split numbers into high and low 32 bits
    const uint64_t lowest32 = 0xFFFFFFFF;
    uint64_t x_hi = x >> 32, x_lo = x & lowest32;
    uint64_t y_hi = y >> 32, y_lo = y & lowest32;

    // parts
    UInt128 r(x_hi * y_hi, x_lo * y_lo);

    // cross terms
    uint64_t u = x_hi * y_lo;
    uint64_t v = x_lo * y_hi;
    r += UInt128(u >> 32, u << 32);
    r += UInt128(v >> 32, v << 32);
    return r;
}

#endif

UInt128 operator*(UInt128 x, UInt128 y)
{
    UInt128 r = _mul_widen(x.lo, y.lo);
    r.hi += x.hi * y.lo;
    r.hi += x.lo * y.hi;
    return r;
}

inline UInt128 operator<<(UInt128 x, int n)
{
    if (n == 0) {
        return x;
    } else if (n >= 64) {
        return UInt128(x.lo << (n - 64), 0);
    } else {
        return UInt128(x.hi << n | x.lo >> (64 - n), x.lo << n);
    }
}

inline constexpr bool operator==(UInt128 x, UInt128 y)
{
    return x.hi == y.hi && x.lo == y.lo;
}

inline constexpr bool operator!=(UInt128 x, UInt128 y)
{
    return x.hi != y.hi || x.lo != y.lo;
}

inline constexpr bool operator<(UInt128 x, UInt128 y)
{
    return UInt128::compare(std::less<UInt128>(), x, y);
}

inline constexpr bool operator<=(UInt128 x, UInt128 y)
{
    return UInt128::compare(std::less_equal<UInt128>(), x, y);
}

inline constexpr bool operator>(UInt128 x, UInt128 y)
{
    return UInt128::compare(std::greater<UInt128>(), x, y);
}

inline constexpr bool operator>=(UInt128 x, UInt128 y)
{
    return UInt128::compare(std::greater_equal<UInt128>(), x, y);
}
/**
 * Fast type for 128-bit unsigned integer arithmetic
 */
#ifdef __SIZEOF_INT128__
    using uint128 = unsigned __int128;
#else
    using uint128 = UInt128;
#endif

} // namespace _internal
} // namespace xprec
