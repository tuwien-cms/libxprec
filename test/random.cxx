/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "xprec/ddouble.h"
#include "xprec/random.h"
#include "catch2-addons.h"

using xprec::DDouble;

TEST_CASE("simple rng 1", "[rng]")
{
    std::mt19937 rng;
    std::uniform_real_distribution<DDouble> dist01(0.0, 1.0);

    for (size_t i = 0; i != 1000; ++i) {
        DDouble x = dist01(rng);
        REQUIRE(x >= 0.0);
        REQUIRE(x < 1.0);
    }
}

TEST_CASE("simple rng 64", "[rng]")
{
    std::mt19937_64 rng;
    std::uniform_real_distribution<DDouble> dist01(-1.0, 1.0);

    for (size_t i = 0; i != 1000; ++i) {
        DDouble x = dist01(rng);
        REQUIRE(x >= -1.0);
        REQUIRE(x < 1.0);
    }
}

TEST_CASE("simple rng 2", "[rng]")
{
    std::minstd_rand0 rng;
    std::uniform_real_distribution<DDouble> dist01(1.5, 4.0);

    for (size_t i = 0; i != 1000; ++i) {
        DDouble x = dist01(rng);
        REQUIRE(x >= 1.5);
        REQUIRE(x < 4.0);
    }
}
