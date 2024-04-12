/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "xprec/ddouble.h"
#include "mpfloat.h"
#include "catch2-addons.h"

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

    REQUIRE(exp(DDouble(-1000)) == 0);
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
