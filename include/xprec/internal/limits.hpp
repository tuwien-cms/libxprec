/* Implementations.
 *
 * DO NOT INCLUDE THIS FILE DIRECTLY: Include ddouble.h instead.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "../ddouble.hpp"

constexpr xprec::DDouble std::numeric_limits<xprec::DDouble>::min() noexcept
{
    // Whereas the maximum exponent is the same for double and DDouble,
    // Denormalization in the low part means that the min exponent for
    // normalized values is lower.
    return DDouble(_double::min() / _double::epsilon());
}

constexpr xprec::DDouble std::numeric_limits<xprec::DDouble>::max() noexcept
{
    return DDouble(_double::max(),
                   _double::max() * _double::epsilon()
                        / _double::radix / _double::radix);
}

constexpr xprec::DDouble std::numeric_limits<xprec::DDouble>::lowest() noexcept
{
    return DDouble(_double::lowest(),
                   _double::lowest() * _double::epsilon()
                        / _double::radix / _double::radix);
}

constexpr xprec::DDouble std::numeric_limits<xprec::DDouble>::epsilon() noexcept
{
    return DDouble(_double::epsilon() * _double::epsilon() / _double::radix);
}

constexpr xprec::DDouble
std::numeric_limits<xprec::DDouble>::round_error() noexcept
{
    return DDouble(_double::round_error());
}

constexpr xprec::DDouble
std::numeric_limits<xprec::DDouble>::infinity() noexcept
{
    return DDouble(_double::infinity(), _double::infinity());
}

constexpr xprec::DDouble
std::numeric_limits<xprec::DDouble>::quiet_NaN() noexcept
{
    return DDouble(_double::quiet_NaN(), _double::quiet_NaN());
}

constexpr xprec::DDouble
std::numeric_limits<xprec::DDouble>::signaling_NaN() noexcept
{
    return DDouble(_double::signaling_NaN(), _double::signaling_NaN());
}

constexpr xprec::DDouble
std::numeric_limits<xprec::DDouble>::denorm_min() noexcept
{
    return DDouble(_double::denorm_min());
}
