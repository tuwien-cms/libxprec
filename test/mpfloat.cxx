#include <catch2/catch_test_macros.hpp>
#include "catch2-addons.h"
#include "mpfloat.h"

TEST_CASE("basic", "[mpfr]")
{
    MPFloat x, y;

    x = 3.0;
    REQUIRE(x == 3.0);
    REQUIRE(x + x == 2.0 * x);
    REQUIRE(x - x == 0.0);

    y = x;
    REQUIRE(x == y);

    x = 3.5;
    REQUIRE(x != y);
}

TEST_CASE("test_that", "[mpfr]")
{
    MPFloat x = 1.0;
    MPFloat y = x + 1e-30;

    REQUIRE(x != y);
    REQUIRE_THAT(x, WithinAbs(y, 1e-25));
    REQUIRE_THAT(x, WithinRel(y, 1e-30));
}
