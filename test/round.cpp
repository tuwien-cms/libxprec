/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>

#include "catch2-addons.hpp"
#include "xprec/ddouble.hpp"

using xprec::DDouble;

TEST_CASE("trunc", "[round]")
{
    double u = std::numeric_limits<double>::epsilon()/2;

    REQUIRE_THAT(trunc(DDouble(0, 0)), Equals<DDouble>(0));
    REQUIRE_THAT(trunc(DDouble(2.5, u/4)), Equals<DDouble>(2));
    REQUIRE_THAT(trunc(DDouble(-2.3, u/4)), Equals<DDouble>(-2));

    REQUIRE_THAT(trunc(DDouble(2, +u/2)), Equals<DDouble>(2));
    REQUIRE_THAT(trunc(DDouble(2, -u/2)), Equals<DDouble>(1));
    REQUIRE_THAT(trunc(DDouble(-2, +u/2)), Equals<DDouble>(-1));
    REQUIRE_THAT(trunc(DDouble(-2, -u/2)), Equals<DDouble>(-2));

    REQUIRE_THAT(trunc(DDouble(INFINITY)), IsPlusInf);
    REQUIRE_THAT(trunc(DDouble(-INFINITY)), IsMinusInf);
    REQUIRE_THAT(trunc(DDouble(NAN)), IsNaN);
}

TEST_CASE("ceil", "[round]")
{
    double u = std::numeric_limits<double>::epsilon()/2;

    REQUIRE_THAT(ceil(DDouble(0, 0)), Equals<DDouble>(0));
    REQUIRE_THAT(ceil(DDouble(2.5, u/4)), Equals<DDouble>(3));
    REQUIRE_THAT(ceil(DDouble(-2.3, u/4)), Equals<DDouble>(-2));

    REQUIRE_THAT(ceil(DDouble(2, +u/2)), Equals<DDouble>(3));
    REQUIRE_THAT(ceil(DDouble(2, -u/2)), Equals<DDouble>(2));
    REQUIRE_THAT(ceil(DDouble(-2, +u/2)), Equals<DDouble>(-1));
    REQUIRE_THAT(ceil(DDouble(-2, -u/2)), Equals<DDouble>(-2));

    REQUIRE_THAT(ceil(DDouble(INFINITY)), IsPlusInf);
    REQUIRE_THAT(ceil(DDouble(-INFINITY)), IsMinusInf);
    REQUIRE_THAT(ceil(DDouble(NAN)), IsNaN);
}

TEST_CASE("floor", "[round]")
{
    double u = std::numeric_limits<double>::epsilon()/2;

    REQUIRE_THAT(floor(DDouble(0, 0)), Equals<DDouble>(0));
    REQUIRE_THAT(floor(DDouble(2.5, u/4)), Equals<DDouble>(2));
    REQUIRE_THAT(floor(DDouble(-2.3, u/4)), Equals<DDouble>(-3));

    REQUIRE_THAT(floor(DDouble(2, +u/2)), Equals<DDouble>(2));
    REQUIRE_THAT(floor(DDouble(2, -u/2)), Equals<DDouble>(1));
    REQUIRE_THAT(floor(DDouble(-2, +u/2)), Equals<DDouble>(-2));
    REQUIRE_THAT(floor(DDouble(-2, -u/2)), Equals<DDouble>(-3));

    REQUIRE_THAT(floor(DDouble(INFINITY)), IsPlusInf);
    REQUIRE_THAT(floor(DDouble(-INFINITY)), IsMinusInf);
    REQUIRE_THAT(floor(DDouble(NAN)), IsNaN);
}
