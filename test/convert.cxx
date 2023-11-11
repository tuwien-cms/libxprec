#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"

TEST_CASE("int", "[convert]" )
{
    int64_t large = 1L << 63 - 1;
    REQUIRE(DDouble(large).as<int64_t>() == large);
    REQUIRE(DDouble(-large).as<int64_t>() == -large);

    REQUIRE(DDouble(1.0f) + DDouble(3) == 4.0);
}

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
