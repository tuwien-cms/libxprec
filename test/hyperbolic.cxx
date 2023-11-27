/* Tests
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "xprec/ddouble.h"
#include "mpfloat.h"
#include "catch2-addons.h"


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
