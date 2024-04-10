/* Small double-double arithmetic library - random distributions
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <climits>
#include <random>

#include "ddouble.h"

namespace xprec {

template <typename Generator>
inline DDouble _rng_range(const Generator &rng)
{
    using T = typename Generator::result_type;
    static_assert(!std::numeric_limits<T>::is_signed, "must be unsigned type");

    // In case of an overflow, we get 0
    T range = rng.max() - rng.min() + 1;
    if (range == 0)
        return std::ldexp(1.0, CHAR_BIT * sizeof(T));
    else
        return range;
}

template <typename Generator, typename Range>
inline DDouble _generate_canonical(Generator &rng, Range range, size_t m)
{
    if (m == 0)
        return 0;

    DDouble sum = rng();
    sum /= range;

    for (size_t k = 1; k != m; ++k) {
        sum = DDouble(rng()).add_small(sum);
        sum /= range;
    }
    return sum;
}

template <size_t Bits, typename Generator>
DDouble generate_canonical(Generator &rng)
{
    size_t b = std::min<size_t>(std::numeric_limits<DDouble>::digits, Bits);
    DDouble r = _rng_range(rng);
    size_t m = std::ceil(b / std::log2(r.hi()));

    if (is_power_of_two(r))
        return _generate_canonical(rng, PowerOfTwo(r.hi()), m);
    else
        return _generate_canonical(rng, r, m);
}

} /* namespace xprec */


namespace std {

// TODO: figure out whether this is actually allowed by the standard
template <>
class uniform_real_distribution<xprec::DDouble>
{
public:
    /** Type of the result it gives */
    using result_type = xprec::DDouble;

    /** Type to store the parameters */
    struct param_type
    {
        using distribution_type = uniform_real_distribution<result_type>;

        constexpr param_type() : param_type(0.0) { }
        explicit constexpr param_type(result_type a, result_type b=1.0)
            : _a(a), _b(b) { }

        constexpr result_type a() const { return _a; }
        constexpr result_type b() const { return _b; }

        friend bool operator==(const param_type &lhs, const param_type &rhs);
        friend bool operator!=(const param_type &lhs, const param_type &rhs);

    private:
        result_type _a, _b;
    };

    constexpr uniform_real_distribution() : uniform_real_distribution(0.0) { }
    constexpr explicit uniform_real_distribution(result_type a, result_type b=1.0)
        : _param(a, b) { }
    constexpr explicit uniform_real_distribution(const param_type &p)
        : _param(p) { }

    constexpr result_type a() const { return _param.a(); }
    constexpr result_type b() const { return _param.b(); }
    constexpr param_type param() const { return _param; }

    void param(const param_type &p) { _param = p; }

    constexpr result_type min() const { return a(); }
    constexpr result_type max() const { return b(); }

    friend bool operator==(const uniform_real_distribution &lhs,
                           const uniform_real_distribution &rhs);
    friend bool operator!=(const uniform_real_distribution &lhs,
                           const uniform_real_distribution &rhs);

    template <typename Generator>
    result_type operator()(Generator &rng) { return operator()(rng, _param); }

    template <typename Generator>
    result_type operator()(Generator &rng, const param_type &param);

private:
    param_type _param;
};


// Implmenetations:

inline bool operator==(
        const uniform_real_distribution<xprec::DDouble>::param_type &lhs,
        const uniform_real_distribution<xprec::DDouble>::param_type &rhs)
{
    return lhs._a == rhs._a && lhs._b == rhs._b;
}

inline bool operator!=(
        const uniform_real_distribution<xprec::DDouble>::param_type &lhs,
        const uniform_real_distribution<xprec::DDouble>::param_type &rhs)
{
    return !(lhs == rhs);
}

inline bool operator==(
        const uniform_real_distribution<xprec::DDouble> &lhs,
        const uniform_real_distribution<xprec::DDouble> &rhs)
{
    return lhs.param() == rhs.param();
}

inline bool operator!=(
        const uniform_real_distribution<xprec::DDouble> &lhs,
        const uniform_real_distribution<xprec::DDouble> &rhs)
{
    return lhs.param() != rhs.param();
}

template <typename Generator>
xprec::DDouble uniform_real_distribution<xprec::DDouble>::operator()(
        Generator &rng, const param_type &param)
{
    using xprec::DDouble;
    constexpr size_t bits = std::numeric_limits<DDouble>::digits;
    DDouble u01 = xprec::generate_canonical<bits, Generator>(rng);
    return (param.b() - param.a()) * u01 + param.a();
}

} /* namespace std */
