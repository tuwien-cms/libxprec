/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.hpp"
#include "catch2-addons.hpp"
#include <catch2/catch_test_macros.hpp>

using xprec::DDouble;
using xprec::ExDouble;

TEST_CASE("int", "[convert]")
{
    // XXX BROKEN
    // int64_t large = (1UL << 63) - 1;
    // REQUIRE_THAT(DDouble(-large).as<int64_t>(), Equals<DDouble>(-large));

    REQUIRE_THAT(DDouble(1.0f) + DDouble(3), Equals<DDouble>(4.0));
}

TEST_CASE("ddbl-dbl", "[convert]")
{
    REQUIRE_THAT(ExDouble(1) + 2, Equals<DDouble>(3));
    REQUIRE_THAT(2 + DDouble(3), Equals<DDouble>(5));
}
