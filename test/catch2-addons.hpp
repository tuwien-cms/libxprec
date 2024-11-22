/* Convenience functions for Catch2
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <catch2/matchers/catch_matchers_predicate.hpp>
#include <iostream>

#include "mpfloat.hpp"

#define CMP_UNARY(fn, x, eps)                                                  \
    do {                                                                       \
        DDouble r_d = fn(DDouble(x));                                          \
        MPFloat r_f = fn(MPFloat(x));                                          \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)

#define CMP_UNARY_ABS(fn, x, eps)                                              \
    do {                                                                       \
        DDouble r_d = fn(DDouble(x));                                          \
        MPFloat r_f = fn(MPFloat(x));                                          \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinAbs(r_f, eps_d));                              \
    } while (false)

#define CMP_BINARY(fn, x, y, eps)                                              \
    do {                                                                       \
        DDouble r_d = fn(DDouble(x), DDouble(y));                              \
        MPFloat r_f = fn(MPFloat(x), MPFloat(y));                              \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)

#define CMP_BINARY_1(fn, x, y, eps)                                            \
    do {                                                                       \
        DDouble r_d = fn(DDouble(x), y);                                       \
        MPFloat r_f = fn(MPFloat(x), y);                                       \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)

#define CMP_BINARY_EX(fn, x, y, eps)                                           \
    do {                                                                       \
        DDouble r_d = fn(ExDouble(x), ExDouble(y));                            \
        MPFloat r_f = fn(MPFloat(x), MPFloat(y));                              \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)

using std::abs;

template <typename T, typename R>
class WithinAbsMatcher : public Catch::Matchers::MatcherBase<T> {
public:
    WithinAbsMatcher(const T &target, const R &margin)
        : _target(target), _margin(margin)
    {
        if (!(_margin >= 0.0f))
            throw std::runtime_error("margin must be non-negative");
    }

    bool match(const T &matchee) const override
    {
        T diff = matchee - _target;
        return diff <= _margin && diff >= -_margin;
    }

    std::string describe() const override
    {
        return "is within " + ::Catch::Detail::stringify(_margin) + " of " +
               ::Catch::Detail::stringify(_target);
    }

private:
    T _target;
    R _margin;
};

template <typename M, typename T>
class EqualsMatcher : public Catch::Matchers::MatcherBase<M> {
public:
    EqualsMatcher(const T &target) : _target(target) { }

    bool match(const M &matchee) const override
    {
        return matchee == _target;
    }

    std::string describe() const override
    {
        return "is equal to " + ::Catch::Detail::stringify(_target);
    }

private:
    T _target;
};

template <typename T, typename R>
WithinAbsMatcher<T, R> WithinAbs(const T &target, const R &margin)
{
    return WithinAbsMatcher<T, R>(target, margin);
}

template <typename T, typename R>
WithinAbsMatcher<T, decltype(R() * abs(T()))> WithinRel(const T &target,
                                                        const R &eps)
{
    using std::abs;
    auto margin = eps * abs(target);
    return WithinAbsMatcher<T, decltype(margin)>(target, margin);
}

template <typename M, typename T>
EqualsMatcher<M,T> Equals(const T &target)
{
    return EqualsMatcher<M,T>(target);
}

static auto IsPlusInf = Catch::Matchers::Predicate<DDouble>(
        [] (DDouble x) -> bool { return x.hi() == INFINITY; },
        "Must be +Infinity");
static auto IsMinusInf = Catch::Matchers::Predicate<DDouble>(
        [] (DDouble x) -> bool { return x.hi() == -INFINITY; },
        "Must be -Infinity");
static auto IsPlusZero = Catch::Matchers::Predicate<DDouble>(
        [] (DDouble x) -> bool { return x.hi() == 0 && !std::signbit(x.hi()); },
        "Must be +0.0");
static auto IsMinusZero = Catch::Matchers::Predicate<DDouble>(
        [] (DDouble x) -> bool { return x.hi() == 0 && std::signbit(x.hi()); },
        "Must be -0.0");
static auto IsNaN = Catch::Matchers::Predicate<DDouble>(
        [] (DDouble x) -> bool { return isnan(x); },
        "Must be NAN");
