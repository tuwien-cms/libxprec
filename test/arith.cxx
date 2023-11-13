#include <catch2/catch_test_macros.hpp>
#include "ddouble.h"
#include "mpfloat.h"
#include "catch2-addons.h"

#define CMP_UNARY(fn, x, eps)                                              \
    do {                                                                   \
        DDouble r_d = fn(DDouble(x));                                      \
        MPFloat r_f = fn(MPFloat(x));                                      \
        double eps_d = eps;                                                \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                          \
    } while(false)

#define CMP_BINARY(fn, x, y, eps)                                          \
    do {                                                                   \
        DDouble r_d = fn(DDouble(x), DDouble(y));                          \
        MPFloat r_f = fn(MPFloat(x), MPFloat(y));                          \
        double eps_d = eps;                                                \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                          \
    } while(false)


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

TEST_CASE("Relational", "[rel]")
{
    REQUIRE(DDouble(1.0) > 0);
    REQUIRE(DDouble(1.0) - ldexp(1.0, -80) < 1);
    REQUIRE(DDouble(3.0) <= DDouble(3.0));
}

TEST_CASE("Divide", "[arith]")
{
    REQUIRE((DDouble(3) / DDouble(2)).as<double>() == 1.5);
}

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
    CMP_BINARY(hypot, ldexp(3.0, -600), ldexp(9.0, -640), 1e-31);
}