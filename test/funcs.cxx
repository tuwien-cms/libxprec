/* Tests
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"
#include "mpfloat.h"
#include "catch2-addons.h"

TEST_CASE("sqrt", "[fn]")
{
    CMP_UNARY(sqrt, 2, 1e-31);
    CMP_UNARY(sqrt, 3, 1e-31);
    CMP_UNARY(sqrt, ldexp(67.0, -39), 1e-31);
    CMP_UNARY(sqrt, ldexp(23.0, 105), 1e-31);
}

TEST_CASE("hypot", "[fn]")
{
    CMP_BINARY(hypot, 1.0, 1.0, 1e-31);
    CMP_BINARY(hypot, 3.0, -10000.0, 1e-31);
    CMP_BINARY(hypot, ldexp(3.0, 600), ldexp(1.0, 570), 1e-31);
    CMP_BINARY(hypot, ldexp(-3.0, 600), ldexp(1.0, 640), 1e-31);
    CMP_BINARY(hypot, ldexp(3.0, -600), ldexp(1.0, -570), 1e-31);
    CMP_BINARY(hypot, ldexp(3.14, -600), ldexp(9.4, -640), 1e-31);
}
