#include <catch2/catch_test_macros.hpp>
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
