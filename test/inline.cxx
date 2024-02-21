/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble-header-only.h"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("check")
{
    DDouble x = 1.0;
    REQUIRE(x.hi() == 1.0);
}
