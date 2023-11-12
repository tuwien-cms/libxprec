#pragma once
#include <catch2/matchers/catch_matchers_floating_point.hpp>

template <typename T, typename R>
class WithinAbsMatcher
    : public Catch::Matchers::MatcherBase<T>
{
public:
    WithinAbsMatcher(const T &target, const R &margin)
        : _target(target)
        , _margin(margin)
    {
        if (!(_margin >= 0.0f))
            throw std::runtime_error("margin must be non-negative");
    }

    bool match(const T &matchee) const override
    {
        return abs(matchee - _target) <= _margin;
    }

    std::string describe() const override
    {
        return "is within " + ::Catch::Detail::stringify(_margin)
            + " of " + ::Catch::Detail::stringify(_target);
    }

private:
    T _target;
    R _margin;
};

template <typename T, typename R>
WithinAbsMatcher<T,R> WithinAbs(const T &target, const R &margin)
{
    return WithinAbsMatcher<T,R>(target, margin);
}

template <typename T, typename R>
WithinAbsMatcher<T, decltype(R() * abs(T()))>
WithinRel(const T &target, const R &eps)
{
    auto margin = eps * abs(target);
    return WithinAbsMatcher<T,decltype(margin)>(target, margin);
}
