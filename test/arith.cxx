#include <catch2/catch_test_macros.hpp>
#include <mpfr.h>

#include "ddouble.h"

TEST_CASE("greater_in_magnitude", "")
{
    REQUIRE(greater_in_magnitude(2.0, 1.0));
    REQUIRE(greater_in_magnitude(-1e100, 1e50));
    REQUIRE(greater_in_magnitude(-0.1, 0.1));
    REQUIRE(!greater_in_magnitude(1.0, 2.0));
    REQUIRE(!greater_in_magnitude(1e200, -INFINITY));
    REQUIRE(greater_in_magnitude(NAN, INFINITY));
    REQUIRE(!greater_in_magnitude(1e200, NAN));
}

TEST_CASE("Divide", "[arith]" )
{
    REQUIRE((DDouble(3) / DDouble(2)).as<double>() == 1.5);
}

TEST_CASE("mpfr", "")
{
    unsigned int i;
    mpfr_t s, t, u;

    mpfr_init2 (t, 200);
    mpfr_set_d (t, 1.0, MPFR_RNDD);
    mpfr_init2 (s, 200);
    mpfr_set_d (s, 1.0, MPFR_RNDD);
    mpfr_init2 (u, 200);
    for (i = 1; i <= 100; i++)
        {
        mpfr_mul_ui (t, t, i, MPFR_RNDU);
        mpfr_set_d (u, 1.0, MPFR_RNDD);
        mpfr_div (u, u, t, MPFR_RNDD);
        mpfr_add (s, s, u, MPFR_RNDD);
        }
    printf ("Sum is ");
    mpfr_out_str (stdout, 10, 0, s, MPFR_RNDD);
    putchar ('\n');
    mpfr_clear (s);
    mpfr_clear (t);
    mpfr_clear (u);
    mpfr_free_cache ();
}