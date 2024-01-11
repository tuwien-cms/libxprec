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

TEST_CASE("leg-cmp", "[gauss]")
{
    const static DDouble x_ref[7] = {
        {-0.9491079123427585, -3.82579658786657e-17},
        {-0.7415311855993945, 2.0220134774069897e-17},
        {-0.4058451513773972, 1.72492754475471e-17},
        {0.0, 0.0},
        {0.4058451513773972, -1.72492754475471e-17},
        {0.7415311855993945, -2.0220134774069897e-17},
        {0.9491079123427585, 3.82579658786657e-17}
        };
    const static DDouble w_ref[7] = {
        {0.1294849661688697, -9.625448970284404e-18},
        {0.27970539148927664, 2.3267180221717138e-17},
        {0.3818300505051189, 2.1862747923824822e-17},
        {0.4179591836734694, -1.5497807119257288e-17},
        {0.3818300505051189, 2.1862747923824822e-17},
        {0.27970539148927664, 2.3267180221717138e-17},
        {0.1294849661688697, -9.625448970284404e-18}
        };

    std::vector<DDouble> x(7), w(7);
    gauss_legendre(7, x.data(), w.data());
    for (int i = 0; i < 7; ++i) {
        REQUIRE_THAT(x[i], WithinAbs(x_ref[i], 5e-32));
        REQUIRE_THAT(w[i], WithinAbs(w_ref[i], 5e-32));
    }
}