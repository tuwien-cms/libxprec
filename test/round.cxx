/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include <catch2/catch_test_macros.hpp>

using xprec::DDouble;

TEST_CASE("trunc", "[round]")
{
    double u = std::numeric_limits<double>::epsilon()/2;

    REQUIRE(trunc(DDouble(0, 0)) == 0);
    REQUIRE(trunc(DDouble(2.5, u/4)) == 2);
    REQUIRE(trunc(DDouble(-2.3, u/4)) == -2);

    REQUIRE(trunc(DDouble(2, +u/2)) == 2);
    REQUIRE(trunc(DDouble(2, -u/2)) == 1);
    REQUIRE(trunc(DDouble(-2, +u/2)) == -1);
    REQUIRE(trunc(DDouble(-2, -u/2)) == -2);

    REQUIRE(isinf(trunc(DDouble(INFINITY))));
    REQUIRE(isinf(trunc(DDouble(-INFINITY))));
    REQUIRE(isnan(trunc(DDouble(NAN))));
}

TEST_CASE("ceil", "[round]")
{
    double u = std::numeric_limits<double>::epsilon()/2;

    REQUIRE(ceil(DDouble(0, 0)) == 0);
    REQUIRE(ceil(DDouble(2.5, u/4)) == 3);
    REQUIRE(ceil(DDouble(-2.3, u/4)) == -2);

    REQUIRE(ceil(DDouble(2, +u/2)) == 3);
    REQUIRE(ceil(DDouble(2, -u/2)) == 2);
    REQUIRE(ceil(DDouble(-2, +u/2)) == -1);
    REQUIRE(ceil(DDouble(-2, -u/2)) == -2);

    REQUIRE(isinf(ceil(DDouble(INFINITY))));
    REQUIRE(isinf(ceil(DDouble(-INFINITY))));
    REQUIRE(isnan(ceil(DDouble(NAN))));
}

TEST_CASE("floor", "[round]")
{
    double u = std::numeric_limits<double>::epsilon()/2;

    REQUIRE(floor(DDouble(0, 0)) == 0);
    REQUIRE(floor(DDouble(2.5, u/4)) == 2);
    REQUIRE(floor(DDouble(-2.3, u/4)) == -3);

    REQUIRE(floor(DDouble(2, +u/2)) == 2);
    REQUIRE(floor(DDouble(2, -u/2)) == 1);
    REQUIRE(floor(DDouble(-2, +u/2)) == -2);
    REQUIRE(floor(DDouble(-2, -u/2)) == -3);

    REQUIRE(isinf(floor(DDouble(INFINITY))));
    REQUIRE(isinf(floor(DDouble(-INFINITY))));
    REQUIRE(isnan(floor(DDouble(NAN))));
}
