/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include <catch2/catch_test_macros.hpp>
#include <complex>
#include <iostream>

using xprec::DDouble;

TEST_CASE("simple", "[complex]")
{
    std::complex<DDouble> a = {DDouble(0.0), DDouble(3.0)};
    std::complex<DDouble> x = (a + DDouble(1.0)) / (a + DDouble(2.0));

    DDouble y = std::abs(x);
    REQUIRE(y < 10.0);
}
