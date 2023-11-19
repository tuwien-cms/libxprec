/* Tests for MPFloat
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "catch2-addons.h"
#include "mpfloat.h"

TEST_CASE("basic", "[mpfr]")
{
    MPFloat x, y;

    x = 3;
    REQUIRE(x == 3);

    x = -5;
    REQUIRE(x == -5);
    REQUIRE(x + x == x * 2);
    REQUIRE(x - x == 0);

    y = x;
    REQUIRE(x == y);

    x = 3.5;
    REQUIRE(x != y);
}

TEST_CASE("test_that", "[mpfr]")
{
    MPFloat x = 1;
    MPFloat y = x + 1e-30;

    REQUIRE(x != y);
    REQUIRE_THAT(x, WithinAbs(y, 1e-25));
    REQUIRE_THAT(x, WithinRel(y, 1e-30));
}
