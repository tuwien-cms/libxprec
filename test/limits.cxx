/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "catch2-addons.h"
#include "xprec/ddouble.h"
#include <catch2/catch_test_macros.hpp>

using xprec::DDouble;
using xprec::PowerOfTwo;
using xprec::ExDouble;
using dd_limits = std::numeric_limits<DDouble>;

static const double eps_d = std::numeric_limits<double>::epsilon();

TEST_CASE("epsilon", "[limits]")
{
    const DDouble almost_one(1.0, eps_d/2);
    REQUIRE(almost_one + dd_limits::epsilon() != almost_one);
    REQUIRE(almost_one - dd_limits::epsilon() != almost_one);
    REQUIRE(almost_one + PowerOfTwo(0.25) * dd_limits::epsilon() == almost_one);
    REQUIRE(almost_one - PowerOfTwo(0.25) * dd_limits::epsilon() == almost_one);
}

TEST_CASE("lohi", "[limits]")
{
    const DDouble smaller(1.0, -2*eps_d*eps_d);
    const DDouble bigger(1.0, 2*eps_d*eps_d);

    REQUIRE(ExDouble(dd_limits::max().hi()) + dd_limits::max().lo() ==
            dd_limits::max());
    REQUIRE(dd_limits::max() > 0);
    REQUIRE(isfinite(dd_limits::max() * smaller));
    REQUIRE(!isfinite(dd_limits::max() * bigger));
    REQUIRE(dd_limits::max() + 0.0 == dd_limits::max());

    REQUIRE(ExDouble(dd_limits::lowest().hi()) + dd_limits::lowest().lo() ==
            dd_limits::lowest());
    REQUIRE(dd_limits::lowest() < 0);
    REQUIRE(isfinite(dd_limits::lowest() * smaller));
    REQUIRE(!isfinite(dd_limits::lowest() * bigger));
    REQUIRE(dd_limits::lowest() + 0.0 == dd_limits::lowest());

    REQUIRE(dd_limits::min() > 0);
    REQUIRE(isnormal(dd_limits::min()));
    REQUIRE(isnormal(bigger * dd_limits::min()));
    REQUIRE(!isnormal((1.0 - eps_d) * dd_limits::min()));
}
