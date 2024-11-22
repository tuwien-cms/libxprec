/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.hpp"
#include <catch2/catch_test_macros.hpp>
#include "catch2-addons.hpp"

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

    REQUIRE(isinf(trunc(DDouble(INFINITY))));
    REQUIRE(isinf(trunc(DDouble(-INFINITY))));
    REQUIRE(isnan(trunc(DDouble(NAN))));
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

    REQUIRE(isinf(ceil(DDouble(INFINITY))));
    REQUIRE(isinf(ceil(DDouble(-INFINITY))));
    REQUIRE(isnan(ceil(DDouble(NAN))));
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

    REQUIRE(isinf(floor(DDouble(INFINITY))));
    REQUIRE(isinf(floor(DDouble(-INFINITY))));
    REQUIRE(isnan(floor(DDouble(NAN))));
}
