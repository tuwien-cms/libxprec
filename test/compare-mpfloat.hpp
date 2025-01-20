#pragma once
#include "catch2-addons.hpp"
#include "mpfloat.hpp"

#define CMP_UNARY(fn, x, eps)                                                  \
    do {                                                                       \
        xprec::DDouble r_d = fn(xprec::DDouble(x));                            \
        MPFloat r_f = fn(MPFloat(x));                                          \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)

#define CMP_UNARY_ABS(fn, x, eps)                                              \
    do {                                                                       \
        xprec::DDouble r_d = fn(xprec::DDouble(x));                            \
        MPFloat r_f = fn(MPFloat(x));                                          \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinAbs(r_f, eps_d));                              \
    } while (false)

#define CMP_BINARY(fn, x, y, eps)                                              \
    do {                                                                       \
        xprec::DDouble r_d = fn(xprec::DDouble(x), xprec::DDouble(y));         \
        MPFloat r_f = fn(MPFloat(x), MPFloat(y));                              \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)

#define CMP_BINARY_1(fn, x, y, eps)                                            \
    do {                                                                       \
        xprec::DDouble r_d = fn(xprec::DDouble(x), y);                         \
        MPFloat r_f = fn(MPFloat(x), y);                                       \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)

#define CMP_BINARY_EX(fn, x, y, eps)                                           \
    do {                                                                       \
        xprec::DDouble r_d = fn(xprec::ExDouble(x), xprec::ExDouble(y));       \
        MPFloat r_f = fn(MPFloat(x), MPFloat(y));                              \
        double eps_d = eps;                                                    \
        REQUIRE_THAT(r_d, WithinRel(r_f, eps_d));                              \
    } while (false)
