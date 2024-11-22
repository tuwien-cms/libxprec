/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "catch2-addons.hpp"
#include "mpfloat.hpp"
#include "xprec/ddouble.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("pow", "[fn]")
{
    CMP_BINARY_1(pow, 3.0, 5, 1e-31);
    CMP_BINARY_1(pow, -5.0, 12, 1e-31);
    CMP_BINARY_1(pow, 2.0, 0, 1e-31);
    CMP_BINARY_1(pow, -2.75, 27, 1e-31);

    // Something is very weird with powers ...
    CMP_BINARY_1(pow, 2., -17, 1e-30);
    CMP_BINARY_1(pow, -1.5, -8, 1e-30);
    CMP_BINARY_1(pow, 1.5, 17, 1e-30);
    CMP_BINARY_1(pow, -2.25, -10, 1e-30);

    REQUIRE_THAT(pow(pow(DDouble(-2.25), -10), -10),
                 WithinRel(pow(DDouble(-2.25), 100), 1e-30));
}

TEST_CASE("pow special", "[pow]")
{
    // pow(anything, +-0) == 1
    REQUIRE_THAT(pow(DDouble(+0.0), -0.0), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(DDouble(+14.0), +0.0), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(DDouble(-77.0), -0.0), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(DDouble(INFINITY), -0.0), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(DDouble(-INFINITY), 0.0), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(DDouble(NAN), 0.0), Equals<DDouble>(1.0));

    // pow(1, anything) == 1
    REQUIRE_THAT(pow(1.0, DDouble(0.0)), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(1.0, DDouble(-0.0)), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(1.0, DDouble(-119139.0)), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(1.0, DDouble(INFINITY)), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(1.0, DDouble(-INFINITY)), Equals<DDouble>(1.0));
    REQUIRE_THAT(pow(1.0, DDouble(NAN)), Equals<DDouble>(1.0));

    // pow(+-0, -odd) == +-inf
    REQUIRE_THAT(pow(DDouble(+0.0), -5.0), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(+0.0), -17.0), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -3.0), IsMinusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -1e15 + 1), IsMinusInf);

    // pow(+-0, -[non-odd]) == +inf
    REQUIRE_THAT(pow(DDouble(+0.0), -4.0), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(+0.0), -1e200), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -1e15 + 2), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -1e100), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -2.1), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -2.95), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -3.1), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -3.8), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(+0.0), -INFINITY), IsPlusInf);
    REQUIRE_THAT(pow(DDouble(-0.0), -INFINITY), IsPlusInf);
}

TEST_CASE("exp", "[exp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(exp, 0.0, 1 * ulp);
    CMP_UNARY(exp, 1.0, 1 * ulp);
    CMP_UNARY(exp, 1e-4, 1.0 * ulp);

    // Small values shall be very accurate
    DDouble x = 0.25;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(exp, x, 1.0 * ulp);
        CMP_UNARY(exp, -x, 1.0 * ulp);
    }

    // Larger, less so, but let's still strive for 1 ulps
    x = 0.125;
    while ((x *= 1.0041) < 708.0) {
        CMP_UNARY(exp, x, 2.0 * ulp);
        if (x < 670)
            CMP_UNARY(exp, -x, 2.0 * ulp);
    }

    REQUIRE_THAT(exp(DDouble(-1000)), Equals<DDouble>(0));
}

TEST_CASE("expm1", "[exp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(expm1, 0.0, 1.0 * ulp);
    CMP_UNARY(expm1, 1.0, 1.0 * ulp);
    CMP_UNARY(expm1, -1000, 1.0 * ulp);

    // Small values shall be very accurate
    DDouble x = 0.25;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(expm1, x, 1.5 * ulp);
        CMP_UNARY(expm1, -x, 1.5 * ulp);
    }

    // Larger, less so, but let's still strive for 1 ulps
    x = 0.125;
    while ((x *= 1.02) < 708.0) {
        CMP_UNARY(expm1, x, 2.0 * ulp);
        if (x < 670)
            CMP_UNARY(expm1, -x, 2.0 * ulp);
    }
}

TEST_CASE("log", "[exp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(log, 1.0, 1.0 * ulp);
    CMP_UNARY(log, 3.0, 1.0 * ulp);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e300) {
        CMP_UNARY(log, x, 1.0 * ulp);
    }

    x = 1.;
    while ((x *= 0.95) > 1e-290) {
        CMP_UNARY(log, x, 1.0 * ulp);
    }
}

TEST_CASE("log1p", "[exp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(log, 1.0, 1.0 * ulp);
    CMP_UNARY(log, 3.0, 1.0 * ulp);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e300) {
        CMP_UNARY(log1p, x, 1.0 * ulp);
    }

    x = 1.;
    while ((x *= 0.92) > 1e-290) {
        CMP_UNARY(log1p, x, 2.5 * ulp);
    }
    x = -.9999999;
    while ((x *= 0.92) > 1e-290) {
        CMP_UNARY(log1p, x, 1.0 * ulp);
    }
}
