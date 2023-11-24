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

TEST_CASE("sinh", "[hyp]")
{
    CMP_UNARY(sinh, 0.01, 1e-31);

    DDouble x = 0.15;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(sinh, x, 5e-32);
        CMP_UNARY(sinh, -x, 5e-32);
    }

    // XXX improve precision here a little
    x = 0.15;
    while ((x *= 1.0041) < 708.0) {
        CMP_UNARY(sinh, x, 1e-31);
        CMP_UNARY(sinh, -x, 1e-31);
    }
}

TEST_CASE("tanh", "[hyp]")
{
    CMP_UNARY(tanh, INFINITY, 1e-31);
    CMP_UNARY(tanh, -INFINITY, 1e-31);

    DDouble x = 0.2;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(tanh, x, 5e-32);
        CMP_UNARY(tanh, -x, 5e-32);
    }

    x = 0.2;
    while ((x *= 1.05) < 1e300) {
        CMP_UNARY(tanh, x, 8e-32);
        CMP_UNARY(tanh, -x, 8e-32);
    }
}

TEST_CASE("acosh", "[hyp]")
{
    CMP_UNARY(acosh, 1.0, 1e-31);
    CMP_UNARY(acosh, 4.0, 1e-31);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e306) {
        CMP_UNARY(acosh, x, 1e-31);
    }

    CMP_UNARY(acosh, 1.00001, 1e-31);
}

TEST_CASE("asinh", "[hyp]")
{
    CMP_UNARY(asinh, 0.0, 1e-31);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e306) {
        CMP_UNARY(asinh, x, 1e-31);
        CMP_UNARY(asinh, -x, 1e-31);
    }

    x = 1.0;
    while ((x *= 0.91) > 1e-300) {
        CMP_UNARY(asinh, x, 1e-31);
        CMP_UNARY(asinh, -x, 1e-31);
    }
}

TEST_CASE("atanh", "[hyp]")
{
    CMP_UNARY(atanh, 0.0, 1e-31);
    CMP_UNARY(atanh, 0.5, 5e-32);
    CMP_UNARY(atanh, -0.5, 5e-32);

    DDouble x = 0.9;
    while ((x *= 0.95) > 1e-300) {
        CMP_UNARY(atanh, x, 1e-31);
        CMP_UNARY(atanh, -x, 1e-31);
    }

    // Check values close to one.
    x = 0.1;
    while ((x *= 0.95) > 1e-20) {
        CMP_UNARY(atanh, 1.0 - x, 1e-31);
        CMP_UNARY(atanh, x - 1.0, 1e-31);
    }
}

TEST_CASE("sin", "[trig]")
{
    CMP_UNARY(sin, 0.0, 5e-32);

    // small values must be very accurate
    DDouble x = M_PI/4;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(sin, x, 5e-32);
        CMP_UNARY(sin, -x, 5e-32);
    }

    // larger values can only be accurate to the magnitude of x
    x = M_PI/4;
    while ((x *= 1.0009) < 10.0) {
        CMP_UNARY_ABS(sin, x, 5e-32 * fabs(x.hi()));
        CMP_UNARY_ABS(sin, -x, 5e-32 * fabs(x.hi()));
    }
    while ((x *= 1.07) < 1e6) {
        CMP_UNARY_ABS(sin, x, 5e-32 * fabs(x.hi()));
        CMP_UNARY_ABS(sin, -x, 5e-32 * fabs(x.hi()));
    }
}

TEST_CASE("cos", "[trig]")
{
    CMP_UNARY(cos, 0.0, 1e-31);

    // small values must be very accurate
    DDouble x = M_PI/4;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(cos, x, 5e-32);
        CMP_UNARY(cos, -x, 5e-32);
    }

    // larger values can only be accurate to the magnitude of x
    x = M_PI/4;
    while ((x *= 1.0009) < 10.0) {
        CMP_UNARY_ABS(cos, x, 5e-32 * fabs(x.hi()));
        CMP_UNARY_ABS(cos, -x, 5e-32 * fabs(x.hi()));
    }
    while ((x *= 1.07) < 1e6) {
        CMP_UNARY_ABS(cos, x, 5e-32 * fabs(x.hi()));
        CMP_UNARY_ABS(cos, -x, 5e-32 * fabs(x.hi()));
    }
}
