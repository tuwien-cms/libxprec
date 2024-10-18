/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble-header-only.hpp"
#include <catch2/catch_test_macros.hpp>

using xprec::DDouble;

TEST_CASE("check")
{
    DDouble x = 1.0;
    REQUIRE(x.hi() == 1.0);
}
