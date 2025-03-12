/* Tests
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>

#include "catch2-addons.hpp"
#include "compare-mpfloat.hpp"
#include "xprec/ddouble.hpp"

using xprec::DDouble;

TEST_CASE("cosh", "[hyp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(cosh, 0.0, 1.0 * ulp);
    CMP_UNARY(cosh, 1.0, 1.0 * ulp);
    CMP_UNARY(cosh, 1e-4, 1.0 * ulp);

    DDouble x = 0.25;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(cosh, x, 1.0 * ulp);
        CMP_UNARY(cosh, -x, 1.0 * ulp);
    }

    x = 0.125;
    while ((x *= 1.0041) < 708.0) {
        CMP_UNARY(cosh, x, 1.0 * ulp);
        CMP_UNARY(cosh, -x, 1.0 * ulp);
    }
}

TEST_CASE("sinh", "[hyp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(sinh, 0.01, 1.0 * ulp);

    DDouble x = 0.15;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(sinh, x, 1.0 * ulp);
        CMP_UNARY(sinh, -x, 1.0 * ulp);
    }

    // XXX improve precision here a little
    x = 0.15;
    while ((x *= 1.0041) < 1.0) {
        CMP_UNARY(sinh, x, 10.0 * ulp);
        CMP_UNARY(sinh, -x, 10.0 * ulp);
    }

    // This is fine.
    while ((x *= 1.0041) < 708.0) {
        CMP_UNARY(sinh, x, 1.0 * ulp);
        CMP_UNARY(sinh, -x, 1.0 * ulp);
    }
}

TEST_CASE("tanh", "[hyp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(tanh, INFINITY, 1.0 * ulp);
    CMP_UNARY(tanh, -INFINITY, 1.0 * ulp);

    DDouble x = 0.2;
    while ((x *= 0.9) > 1e-290) {
        CMP_UNARY(tanh, x, 2.0 * ulp);
        CMP_UNARY(tanh, -x, 2.0 * ulp);
    }

    x = 0.2;
    while ((x *= 1.05) < 1e300) {
        CMP_UNARY(tanh, x, 4.0 * ulp);
        CMP_UNARY(tanh, -x, 4.0 * ulp);
    }
}

TEST_CASE("acosh", "[hyp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(acosh, 1.0, 2.0 * ulp);
    CMP_UNARY(acosh, 4.0, 2.0 * ulp);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e306) {
        CMP_UNARY(acosh, x, 2.0 * ulp);
    }

    // XXX accuracy
    CMP_UNARY(acosh, 1.00001, 10.0 * ulp);
}

TEST_CASE("asinh", "[hyp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(asinh, 0.0, 2.0 * ulp);

    DDouble x = 1.;
    while ((x *= 1.13) < 1e306) {
        CMP_UNARY(asinh, x, 2.0 * ulp);
        CMP_UNARY(asinh, -x, 2.0 * ulp);
    }

    x = 1.0;
    while ((x *= 0.91) > 1e-300) {
        CMP_UNARY(asinh, x, 5.0 * ulp);
        CMP_UNARY(asinh, -x, 5.0 * ulp);
    }
}

TEST_CASE("atanh", "[hyp]")
{
    const double ulp = 2.4651903288156619e-32;
    CMP_UNARY(atanh, 0.0, 2.0 * ulp);
    CMP_UNARY(atanh, 0.5, 2.0 * ulp);
    CMP_UNARY(atanh, -0.5, 2.0 * ulp);

    DDouble x = 0.9;
    while ((x *= 0.95) > 1e-300) {
        CMP_UNARY(atanh, x, 2.0 * ulp);
        CMP_UNARY(atanh, -x, 2.0 * ulp);
    }

    // Check values close to one.
    x = 0.1;
    while ((x *= 0.95) > 1e-20) {
        CMP_UNARY(atanh, 1.0 - x, 2.0 * ulp);
        CMP_UNARY(atanh, x - 1.0, 2.0 * ulp);
    }
}
