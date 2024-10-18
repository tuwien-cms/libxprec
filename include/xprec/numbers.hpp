/* Small double-double arithmetic library - numbers module
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once

#if __cpp_lib_math_constants >= 201907L
#include <numbers>
#endif

#include "ddouble.hpp"

namespace xprec {

/**
 * Collection of mathematical constants accurate to quad precision
 *
 * The naming corresponds to std::numbers.
 */
namespace numbers {

/** Base of the natural logarithm (Euler's number) */
constexpr DDouble e{2.718281828459045, 1.4456468917292502e-16};

/** Binary logarithm of e */
constexpr DDouble log2e{1.4426950408889634, 2.0355273740931033e-17};

/** Logarithm base-10 of e */
constexpr DDouble log10e{0.4342944819032518, 1.098319650216765e-17};

/** Circle constant pi */
constexpr DDouble pi{3.141592653589793, 1.2246467991473532e-16};

/** Circle constant pi, divided by two */
constexpr DDouble pi_half{1.5707963267948966, 6.123233995736766e-17};

/** Circle constant pi, divided by four */
constexpr DDouble pi_4{0.7853981633974483, 3.061616997868383e-17};

/** Reciprocal of pi */
constexpr DDouble inv_pi{0.3183098861837907, -1.9678676675182486e-17};

/** Square root of the reciprocal of pi */
constexpr DDouble inv_sqrtpi{0.5641895835477563, 7.66772980658294e-18};

/** Natural logarithm of two */
constexpr DDouble ln2{0.6931471805599453, 2.3190468138462996e-17};

/** Natural logarithm of ten */
constexpr DDouble ln10{2.302585092994046, -2.1707562233822494e-16};

/** Square root of two */
constexpr DDouble sqrt2{1.4142135623730951, -9.667293313452913e-17};

/** Square root of three */
constexpr DDouble sqrt3{1.7320508075688772, 1.0035084221806903e-16};

/** Reciprocal of the square root of three */
constexpr DDouble inv_sqrt3{0.5773502691896257, 3.3450280739356345e-17};

/** Euler–Mascheroni constant */
constexpr DDouble egamma{0.5772156649015329, -4.942915152430645e-18};

/** Golden ratio */
constexpr DDouble phi{1.618033988749895, -5.432115203682506e-17};

} // namespace numbers
} // namespace xprec

#if __cpp_lib_math_constants >= 201907L

namespace std {
namespace numbers {

template <>
inline constexpr xprec::DDouble e_v<xprec::DDouble> = xprec::numbers::e;
template <>
inline constexpr xprec::DDouble log2e_v<xprec::DDouble> = xprec::numbers::log2e;
template <>
inline constexpr xprec::DDouble log10e_v<xprec::DDouble> =
    xprec::numbers::log10e;
template <>
inline constexpr xprec::DDouble pi_v<xprec::DDouble> = xprec::numbers::pi;
template <>
inline constexpr xprec::DDouble inv_pi_v<xprec::DDouble> =
    xprec::numbers::inv_pi;
template <>
inline constexpr xprec::DDouble inv_sqrtpi_v<xprec::DDouble> =
    xprec::numbers::inv_sqrtpi;
template <>
inline constexpr xprec::DDouble ln2_v<xprec::DDouble> = xprec::numbers::ln2;
template <>
inline constexpr xprec::DDouble ln10_v<xprec::DDouble> = xprec::numbers::ln10;
template <>
inline constexpr xprec::DDouble sqrt2_v<xprec::DDouble> = xprec::numbers::sqrt2;
template <>
inline constexpr xprec::DDouble sqrt3_v<xprec::DDouble> = xprec::numbers::sqrt3;
template <>
inline constexpr xprec::DDouble inv_sqrt3_v<xprec::DDouble> =
    xprec::numbers::inv_sqrt3;
template <>
inline constexpr xprec::DDouble egamma_v<xprec::DDouble> =
    xprec::numbers::egamma;
template <>
inline constexpr xprec::DDouble phi_v<xprec::DDouble> = xprec::numbers::phi;

} // namespace numbers
} // namespace std

#endif
