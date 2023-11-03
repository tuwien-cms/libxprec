#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"

TEST_CASE("Divide", "[div]" ) {
    REQUIRE((DDouble(3) / DDouble(2)).as<double>() == 1.5);
}
