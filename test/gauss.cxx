/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <numeric>
#include <vector>
#include <catch2/catch_test_macros.hpp>
#include "xprec/ddouble.h"
#include "catch2-addons.h"


TEST_CASE("leg-weights", "[gauss]")
{
    std::vector<DDouble> x(5), w(5);
    gauss_legendre(5, x.data(), w.data());

    REQUIRE_THAT(std::accumulate(w.begin(), w.end(), DDouble(0.0)),
                 WithinRel(DDouble(2.0), 5e-32));
}