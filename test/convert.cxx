/* Tests
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"

TEST_CASE("int", "[convert]" )
{
    int64_t large = (1UL << 63) - 1;
    REQUIRE(DDouble(large).as<int64_t>() == large);
    REQUIRE(DDouble(-large).as<int64_t>() == -large);

    REQUIRE(DDouble(1.0f) + DDouble(3) == 4.0);
}
