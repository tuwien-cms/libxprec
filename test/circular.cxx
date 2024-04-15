/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "xprec/ddouble.h"
#include "mpfloat.h"
#include "catch2-addons.h"

MPFloat trig_complement(MPFloat x)
{
    return sqrt(1 - x * x);
}

TEST_CASE("compl", "[trig]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(trig_complement, 0.0, 1.0 * ulp);
    CMP_UNARY(trig_complement, 1.0, 1.0 * ulp);

    DDouble x = 0.5;
    while ((x *= 0.98) > 1e-290) {
        CMP_UNARY(trig_complement, x, 3.0 * ulp);
        CMP_UNARY(trig_complement, -x, 3.0 * ulp);
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
    SKIP();
    x = M_PI/4;
    while ((x /= 0.98) < M_PI/2) {
        CMP_UNARY(tan, x, 5e-32);
        CMP_UNARY(tan, -x, 5e-32);
    }
}

TEST_CASE("asin", "[trig]")
{
    CMP_UNARY(asin, 0.0, 1e-31);
    CMP_UNARY(asin, 0.5, 1e-31);
    CMP_UNARY(asin, -0.5, 1e-31);
    CMP_UNARY(asin, 1.0, 1e-31);
    CMP_UNARY(asin, -1.0, 1e-31);

    // small values must be very accurate
    DDouble x = 1.0;
    while ((x *= 0.84) > 1e-300) {
        CMP_UNARY(asin, x, 1e-31);
        CMP_UNARY(asin, -x, 1e-31);
    }

    // XXX: inaccurate for large values
    SKIP();
    x = 0.9;
    while ((x /= 0.99) < 1.0) {
        CMP_UNARY(asin, x, 1e-31);
        CMP_UNARY(asin, -x, 1e-31);
    }
}

TEST_CASE("acos", "[trig]")
{
    CMP_UNARY(acos, 0.0, 1e-31);
    CMP_UNARY(acos, 0.5, 1e-31);
    CMP_UNARY(acos, -0.5, 1e-31);
    CMP_UNARY(acos, 1.0, 1e-31);
    CMP_UNARY(acos, -1.0, 1e-31);

    // small values must be very accurate
    DDouble x = 1.0;
    while ((x *= 0.84) > 1e-300) {
        CMP_UNARY(acos, x, 1e-31);
        CMP_UNARY(acos, -x, 1e-31);
    }

    // XXX: inaccurate for large values
    SKIP();
    x = 0.9;
    while ((x /= 0.99) < 1.0) {
         CMP_UNARY(acos, x, 2e-31);
         CMP_UNARY(acos, -x, 2e-31);
    }
}

TEST_CASE("atan", "[trig]")
{
    CMP_UNARY(atan, 0.0, 1e-31);
    CMP_UNARY(atan, 0.5, 1e-31);
    CMP_UNARY(atan, -0.5, 1e-31);

    // check infties
    const static DDouble PI_2(1.5707963267948966, 6.123233995736766e-17);
    REQUIRE_THAT(atan(DDouble(INFINITY)), WithinAbs(PI_2, 1e-31));
    REQUIRE_THAT(atan(DDouble(-INFINITY)), WithinAbs(-PI_2, 1e-31));
    REQUIRE(isnan(atan(DDouble(NAN))));

    // small values must be very accurate
    DDouble x = 1.0;
    while ((x *= 0.84) > 1e-300) {
        CMP_UNARY(atan, x, 1e-31);
        CMP_UNARY(atan, -x, 1e-31);
    }

    x = 1.0;
    while ((x /= 0.84) < 1e290) {
        CMP_UNARY(atan, x, 1e-31);
        CMP_UNARY(atan, -x, 1e-31);
    }
}

TEST_CASE("atan2", "[trig]")
{
    CMP_BINARY(atan2, 0.0, 0.0, 1e-31);
    CMP_BINARY(atan2, 0.3, 0.0, 1e-31);
    CMP_BINARY(atan2, 0.0, 1.0, 1e-31);
    CMP_BINARY(atan2, -0.5, 0.0, 1e-31);
    CMP_BINARY(atan2, 0.0, -0.1, 1e-31);

    CMP_BINARY(atan2,  0.5,  0.5, 1e-31);
    CMP_BINARY(atan2,  0.5, -0.5, 1e-31);
    CMP_BINARY(atan2, -0.5,  0.5, 1e-31);
    CMP_BINARY(atan2, -0.5, -0.5, 1e-31);
}
