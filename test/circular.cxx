/* Tests
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"
#include "mpfloat.h"
#include "catch2-addons.h"

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


TEST_CASE("tan", "[trig]")
{
    CMP_UNARY(tan, 0.0, 1e-31);

    // small values must be very accurate
    DDouble x = M_PI/4;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(tan, x, 5e-32);
        CMP_UNARY(tan, -x, 5e-32);
    }

    // XXX: larger values are relatively inaccurate
}
