#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"
#include "use-mpfr.h"

TEST_CASE("int", "[convert]" )
{
    int64_t large = 1L << 63 - 1;
    REQUIRE(DDouble(large).as<int64_t>() == large);
    REQUIRE(DDouble(-large).as<int64_t>() == -large);

    REQUIRE(DDouble(1.0f) + DDouble(3) == 4.0);
}

TEST_CASE("mpfr", "[convert]")
{
    DDouble x(1., ldexp(1, -60));
    mpfr_t y;

    mpfr_init2(y, 120);
    ddouble_set_mpfr(y, x);
    DDouble xrec = ddouble_from_mpfr(y);
    REQUIRE(x == xrec);
}