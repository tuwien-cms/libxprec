/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "catch2-addons.hpp"
#include "mpfloat.hpp"
#include "xprec/ddouble.hpp"
#include "xprec/internal/utils.hpp"
#include <catch2/catch_test_macros.hpp>

using xprec::ExDouble;
using xprec::PowerOfTwo;

TEST_CASE("greater_in_magnitude", "")
{
    using xprec::_internal::greater_in_magnitude;
    REQUIRE(greater_in_magnitude(2.0, 1.0));
    REQUIRE(greater_in_magnitude(-1e100, 1e50));
    REQUIRE(greater_in_magnitude(-0.1, 0.1));
    REQUIRE(!greater_in_magnitude(1.0, 2.0));
    REQUIRE(!greater_in_magnitude(1e200, -INFINITY));
    REQUIRE(greater_in_magnitude(NAN, INFINITY));
    REQUIRE(!greater_in_magnitude(1e200, NAN));
}

TEST_CASE("arith dbl", "[arith]")
{
    const double ulp = 2.4651903288156619e-32;
    for (double x = 10.0; x > 5.0; x *= .9933) {
        for (double y = x; y > 1e-35; y *= .9383) {
            CMP_BINARY(operator+, x, y, 1.6*ulp);
            CMP_BINARY(operator-, x, y, 1.6*ulp);
            CMP_BINARY(operator+, y, x, 1.6*ulp);
            CMP_BINARY(operator-, y, x, 1.6*ulp);
            CMP_BINARY(operator*, x, y, 2*ulp);
            CMP_BINARY(operator/, x, y, 3*ulp);
            CMP_BINARY(operator/, -x, y, 3*ulp);
            CMP_BINARY(operator/, y, x, 3*ulp);
        }
    }
}

TEST_CASE("arith ex", "[arith]")
{
    const double ulp = 2.4651903288156619e-32;
    for (double x = 10.0; x > 5.0; x *= .9933) {
        for (double y = x; y > 1e-35; y *= .9383) {
            CMP_BINARY_EX(operator+, x, y, ulp/4);
            CMP_BINARY_EX(operator-, x, y, ulp/4);
            CMP_BINARY_EX(operator+, y, x, ulp/4);
            CMP_BINARY_EX(operator-, y, x, ulp/4);
            CMP_BINARY_EX(operator*, x, y, ulp/4);
            CMP_BINARY_EX(operator/, x, y, ulp);
            CMP_BINARY_EX(operator/, y, x, ulp);
            CMP_BINARY_EX(operator/, x, -y, ulp);
            CMP_BINARY_EX(operator/, -y, x, ulp);
        }
    }
}

TEST_CASE("arith dbl-small", "[arith]")
{
    const double ulp = 2.4651903288156619e-32;
    for (double x = 10.0; x > 5.0; x *= .9933) {
        for (double y = x; y > 1e-35; y *= .9883) {
            MPFloat x_plus_y = MPFloat(x) + y;
            REQUIRE_THAT(ExDouble(x).add_small(y), WithinRel(x_plus_y, ulp));

            MPFloat x_minus_y = MPFloat(x) - y;
            REQUIRE_THAT(ExDouble(x).add_small(-y), WithinRel(x_minus_y, ulp));
        }
    }
}

TEST_CASE("arith dbl-small-ddbl", "[arith]")
{
    const double ulp = 2.4651903288156619e-32;
    for (double x = 10.0; x > 5.0; x *= .9933) {
        for (DDouble y = x; y > 1e-35; y *= .9883) {
            MPFloat x_plus_y = MPFloat(x) + y;
            REQUIRE_THAT(ExDouble(x).add_small(y), WithinRel(x_plus_y, ulp));

            MPFloat x_minus_y = MPFloat(x) - y;
            REQUIRE_THAT(ExDouble(x).add_small(-y), WithinRel(x_minus_y, ulp));
        }
    }
}

TEST_CASE("sum stress test", "[arith]")
{
    const double u = 0.5 * std::numeric_limits<double>::epsilon();

    // Propery 2.2 in doi:10.1145/3484514
    DDouble x(1.0, u - u*u);
    DDouble y(0.5 * (-1 + u), u * u * (-0.5 + u));
    MPFloat r_ex = MPFloat(x) + y;

    // The lower tolerances can be relaxed if we find a better algorithm
    DDouble r = x + y;
    REQUIRE_THAT(r, WithinRel(r_ex, 3*u*u));
    REQUIRE_THAT(r, !WithinRel(r_ex, 2.5*u*u));

    DDouble r2 = x.add_small(y);
    REQUIRE_THAT(r2, WithinRel(r_ex, 3*u*u));
    REQUIRE_THAT(r2, !WithinRel(r_ex, 2.5*u*u));
}

TEST_CASE("mul stress test", "[arith]")
{
    const double u = 0.5 * std::numeric_limits<double>::epsilon();

    DDouble x(ldexp(2251799825991851, -51), ldexp(9007199203085987, -106));
    DDouble y(ldexp(4503599627471459, -52), ldexp(4503599627284651, -105));

    DDouble r = x * y;
    MPFloat r_ex = MPFloat(x) * y;
    REQUIRE_THAT(r, WithinRel(r_ex, 4*u*u));
    REQUIRE_THAT(r, !WithinRel(r_ex, 3.5*u*u));
}

TEST_CASE("divqd stress test", "[arith]")
{
    const double u = 0.5 * std::numeric_limits<double>::epsilon();

    DDouble x(4588860379563012., ldexp(-4474949195791253, -53));
    double y = 4578284000230917.;

    DDouble r = x / y;
    MPFloat r_ex = MPFloat(x) / y;
    REQUIRE_THAT(r, WithinRel(r_ex, 3*u*u));
    REQUIRE_THAT(r, !WithinRel(r_ex, 2.5*u*u));
}

TEST_CASE("divqq stress test", "[arith]")
{
    const double u = 0.5 * std::numeric_limits<double>::epsilon();

    DDouble x(4528288502329187., ldexp(1125391118633487, -51));
    DDouble y(4522593432466394., ldexp(-9006008290016505, -54));

    DDouble r = x / y;
    MPFloat r_ex = MPFloat(x) / y;
    REQUIRE_THAT(r, WithinRel(r_ex, 6*u*u));
    REQUIRE_THAT(r, !WithinRel(r_ex, 5.5*u*u));
}

TEST_CASE("pow2", "[arith]")
{
    REQUIRE(PowerOfTwo(4.0) * PowerOfTwo(-8.0) == PowerOfTwo(-32.0));
    REQUIRE(PowerOfTwo(16.0) / PowerOfTwo(64.0) == PowerOfTwo(0.25));
    REQUIRE(reciprocal(PowerOfTwo(8.0)) == PowerOfTwo(0.125));
}

TEST_CASE("Relational", "[rel]")
{
    REQUIRE(DDouble(1.0) > 0);
    REQUIRE(DDouble(1.0) - ldexp(1.0, -80) < 1);
    REQUIRE(DDouble(3.0) <= +DDouble(3.0));
}

TEST_CASE("MinMax", "[rel]")
{
    REQUIRE(fmin(DDouble(1.0), DDouble(2.5)) == 1.0);
    REQUIRE(fmin(DDouble(2.0), DDouble(-4.0)) == -4.0);
    REQUIRE(fmin(DDouble(NAN), DDouble(100.0)) == 100);
    REQUIRE(fmin(DDouble(50.0), DDouble(NAN)) == 50);

    REQUIRE(fmax(DDouble(1.0), DDouble(2.5)) == 2.5);
    REQUIRE(fmax(DDouble(2.0), DDouble(-4.0)) == 2.0);
    REQUIRE(fmax(DDouble(NAN), DDouble(100.0)) == 100);
    REQUIRE(fmax(DDouble(50.0), DDouble(NAN)) == 50);
}

TEST_CASE("Mul", "[arith]")
{
    DDouble x = DDouble(1.25, -ldexp(1.125, -85));
    CMP_BINARY(operator*, x, x, 1e-31);
    CMP_BINARY(operator*, -3.25, x, 1e-31);

    CMP_BINARY_1(operator*, x, -139818.125, 1e-31);
}

TEST_CASE("Divide", "[arith]")
{
    REQUIRE((DDouble(3) / DDouble(2)).as<double>() == 1.5);
    CMP_BINARY(operator/, 3, 4, 1e-31);
    CMP_BINARY(operator/, -149, 53.25, 1e-30);
}

TEST_CASE("Divide pow2", "[arith]")
{
    CMP_BINARY(operator/, 1, 137, 1e-31);
    REQUIRE_THAT(reciprocal(ExDouble(137)), WithinRel(MPFloat(1) / 137, 1e-21));
}
