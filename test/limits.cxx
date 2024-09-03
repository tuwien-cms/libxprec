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
    REQUIRE(dd_limits::max() + 0.0 == dd_limits::max());

    REQUIRE(ExDouble(dd_limits::lowest().hi()) + dd_limits::lowest().lo() ==
            dd_limits::lowest());
    REQUIRE(dd_limits::lowest() < 0);
    REQUIRE(isfinite(dd_limits::lowest() * smaller));
    REQUIRE(dd_limits::lowest() + 0.0 == dd_limits::lowest());

    REQUIRE(dd_limits::min() > 0);
    REQUIRE(isnormal(dd_limits::min()));
    REQUIRE(isnormal(bigger * dd_limits::min()));
    REQUIRE(!isnormal((1.0 - eps_d) * dd_limits::min()));

    REQUIRE(dd_limits::denorm_min() > 0);
    REQUIRE(!isnormal(dd_limits::denorm_min()));
    REQUIRE(0.49 * dd_limits::denorm_min() == 0);
}

TEST_CASE("infs", "[limits]")
{
    REQUIRE(isinf(dd_limits::infinity()));
    REQUIRE(!isnan(dd_limits::infinity()));
    REQUIRE(!isfinite(dd_limits::infinity()));
    REQUIRE(!isnormal(dd_limits::infinity()));
    REQUIRE(dd_limits::infinity() > dd_limits::max());

    REQUIRE(isinf(-dd_limits::infinity()));
    REQUIRE(!isnan(-dd_limits::infinity()));
    REQUIRE(!isfinite(-dd_limits::infinity()));
    REQUIRE(!isnormal(-dd_limits::infinity()));
    REQUIRE(-dd_limits::infinity() < dd_limits::lowest());
}

TEST_CASE("nans", "[limits]")
{
    REQUIRE((dd_limits::has_quiet_NaN || dd_limits::has_signaling_NaN));

    if (dd_limits::has_quiet_NaN) {
        REQUIRE(isnan(dd_limits::quiet_NaN()));
        REQUIRE(!isinf(dd_limits::quiet_NaN()));
        REQUIRE(!isfinite(dd_limits::quiet_NaN()));
        REQUIRE(!isnormal(dd_limits::quiet_NaN()));
        REQUIRE(dd_limits::quiet_NaN() != dd_limits::quiet_NaN());
    }
    if (dd_limits::has_signaling_NaN) {
        REQUIRE(isnan(dd_limits::signaling_NaN()));
        REQUIRE(!isinf(dd_limits::signaling_NaN()));
        REQUIRE(!isfinite(dd_limits::signaling_NaN()));
        REQUIRE(!isnormal(dd_limits::signaling_NaN()));
        REQUIRE(dd_limits::signaling_NaN() != dd_limits::signaling_NaN());
    }
}

TEST_CASE("nextafter", "[limits]")
{
    REQUIRE(isinf(nextafter(+dd_limits::infinity(), +dd_limits::infinity())));
    REQUIRE(isinf(nextafter(-dd_limits::infinity(), -dd_limits::infinity())));
    REQUIRE(nextafter(+dd_limits::infinity(), -dd_limits::infinity())
            == dd_limits::max());
    REQUIRE(nextafter(-dd_limits::infinity(), +dd_limits::infinity())
            == -dd_limits::max());

    REQUIRE(nextafter(DDouble(1.0), DDouble(2.0)) > 1.0);
    REQUIRE(nextafter(DDouble(1.0), DDouble(0.0)) < 1.0);

    REQUIRE(nextafter(DDouble(1.0), DDouble(1.0, 1e-20)) > 1.0);
    REQUIRE(nextafter(DDouble(1.0), DDouble(1.0, -1e-20)) < 1.0);
}
