/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "xprec/ddouble.h"
#include "mpfloat.h"
#include "catch2-addons.h"

TEST_CASE("greater_in_magnitude", "")
{
    REQUIRE(greater_in_magnitude(2.0, 1.0));
    REQUIRE(greater_in_magnitude(-1e100, 1e50));
    REQUIRE(greater_in_magnitude(-0.1, 0.1));
    REQUIRE(!greater_in_magnitude(1.0, 2.0));
    REQUIRE(!greater_in_magnitude(1e200, -INFINITY));
    REQUIRE(greater_in_magnitude(NAN, INFINITY));
    REQUIRE(!greater_in_magnitude(1e200, NAN));
}

TEST_CASE("is_small", "")
{
    REQUIRE(ExDouble(-1.2).is_small(0.5));
    REQUIRE(ExDouble(1.2).is_small(1.3));
    REQUIRE(ExDouble(9.0).is_small(0.0));
    REQUIRE(ExDouble(-1e200).is_small(1e-200));
}

TEST_CASE("arith", "[arith]")
{
    for (double x = 10.0; x > 5.0; x *= .9933) {
        for (double y = x; y > 1e-35; y *= .9383) {
            CMP_BINARY(operator+, x, y, 2.5e-32);
            CMP_BINARY(operator-, x, y, 2.5e-32);
            CMP_BINARY(operator+, y, x, 2.5e-32);
            CMP_BINARY(operator-, y, x, 2.5e-32);
            CMP_BINARY(operator*, x, y, 2.5e-32);
            CMP_BINARY(operator/, x, y, 5.0e-32);
        }
    }
}

TEST_CASE("Relational", "[rel]")
{
    REQUIRE(DDouble(1.0) > 0);
    REQUIRE(DDouble(1.0) - ldexp(1.0, -80) < 1);
    REQUIRE(DDouble(3.0) <= +DDouble(3.0));
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
    REQUIRE_THAT(reciprocal(ExDouble(137)),
                 WithinRel(MPFloat(1) / 137, 1e-21));
}

TEST_CASE("trunc", "[round]")
{
    DDouble x(ldexp(1, 61), 0.5);
    REQUIRE(round(x) == DDouble(ldexp(1, 61), 1));
    REQUIRE(round(-x) == DDouble(-ldexp(1, 61), -1));
}
