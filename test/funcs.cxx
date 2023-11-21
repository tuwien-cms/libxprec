/* Tests
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"
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
    CMP_UNARY(exp, 0.0, 1e-31);
    CMP_UNARY(exp, 1.0, 1e-31);
    CMP_UNARY(exp, 1e-4, 1e-31);

    // Small values shall be very accurate
    DDouble x = 0.25;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(exp, x, 5e-32);
        CMP_UNARY(exp, -x, 5e-32);
    }

    // Larger, less so, but let's still strive for 1 ulps
    x = 0.125;
    while ((x *= 1.0041) < 708.0) {
        CMP_UNARY(exp, x, 5e-32);
        if (x < 670)
            CMP_UNARY(exp, -x, 5e-32);
    }

    REQUIRE(exp(DDouble(-1000)) == 0);
}

TEST_CASE("expm1", "[exp]")
{
    CMP_UNARY(expm1, 0.0, 1e-31);
    CMP_UNARY(expm1, 1.0, 1e-31);
    CMP_UNARY(expm1, -1000, 1e-31);

    // Small values shall be very accurate
    DDouble x = 0.25;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(expm1, x, 5e-32);
        CMP_UNARY(expm1, -x, 5e-32);
    }

    // Larger, less so, but let's still strive for 1 ulps
    x = 0.125;
    while ((x *= 1.02) < 708.0) {
        CMP_UNARY(expm1, x, 8e-32);
        if (x < 670)
            CMP_UNARY(expm1, -x, 8e-32);
    }
}

TEST_CASE("log", "[exp]")
{
    CMP_UNARY(log, 1.0, 1e-31);
    CMP_UNARY(log, 3.0, 1e-31);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e300) {
        CMP_UNARY(log, x, 1e-31);
    }

    x = 1.;
    while ((x *= 0.95) > 1e-290) {
        CMP_UNARY(log, x, 1e-31);
    }
}

TEST_CASE("log1p", "[exp]")
{
    CMP_UNARY(log, 1.0, 1e-31);
    CMP_UNARY(log, 3.0, 1e-31);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e300) {
        CMP_UNARY(log1p, x, 1e-31);
    }

    x = 1.;
    while ((x *= 0.92) > 1e-290) {
        CMP_UNARY(log1p, x, 1e-31);
    }
    x = -.9999999;
    while ((x *= 0.92) > 1e-290) {
        CMP_UNARY(log1p, x, 1e-31);
    }
}

TEST_CASE("cosh", "[hyp]")
{
    CMP_UNARY(cosh, 0.0, 1e-31);
    CMP_UNARY(cosh, 1.0, 1e-31);
    CMP_UNARY(cosh, 1e-4, 1e-31);

    DDouble x = 0.25;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(cosh, x, 5e-32);
        CMP_UNARY(cosh, -x, 5e-32);
    }

    x = 0.125;
    while ((x *= 1.0041) < 708.0) {
        CMP_UNARY(cosh, x, 5e-32);
        CMP_UNARY(cosh, -x, 5e-32);
    }
}
