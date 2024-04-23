/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "catch2-addons.h"
#include "mpfloat.h"
#include "xprec/ddouble.h"
#include <catch2/catch_test_macros.hpp>

using xprec::DDouble;

TEST_CASE("hypot", "[fn]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_BINARY(hypot, 1.0, 1.0, 1.5 * ulp);
    CMP_BINARY(hypot, 3.0, -10000.0, 1e-31);
    CMP_BINARY(hypot, ldexp(3.0, 600), ldexp(1.0, 570), 1.5 * ulp);
    CMP_BINARY(hypot, ldexp(-3.0, 600), ldexp(1.0, 640), 1.5 * ulp);
    CMP_BINARY(hypot, ldexp(3.0, -600), ldexp(1.0, -570), 1.5 * ulp);
    CMP_BINARY(hypot, ldexp(3.14, -600), ldexp(9.4, -640), 1.5 * ulp);

    for (double x = 10.0; x > 5.0; x *= .9333) {
        for (double y = x; y > 1e-20 * x; y *= .9983) {
            CMP_BINARY(hypot, x, y, 2.0 * ulp);
            CMP_BINARY(hypot, y, x, 2.0 * ulp);
        }
    }
}

TEST_CASE("sqrt", "[fn]")
{
    const double ulp = 2.4651903288156619e-32;
    REQUIRE(isnan(sqrt(DDouble(-1.0))));

    CMP_UNARY(sqrt, 0.0, 1.0);
    CMP_UNARY(sqrt, 1.0, 1.0 * ulp);
    CMP_UNARY(sqrt, 0.25, 1.0 * ulp);
    CMP_UNARY(sqrt, 4.0, 1.0 * ulp);

    DDouble x = 1.0;
    while ((x *= 0.99) > 1e-290) {
        CMP_UNARY(sqrt, x, 1.5 * ulp);
    }

    x = 1.0;
    while ((x /= 0.99) <= 1e290) {
        CMP_UNARY(sqrt, x, 1.5 * ulp);
    }
}
