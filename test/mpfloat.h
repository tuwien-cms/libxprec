/* Convenience wrapper for MPFloat to allow nice object-oriented stuff.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <cstdlib>
#include <iostream>
#include <new>
#include <sstream>
#include <string>
#include <utility>

#include <mpfr.h>

#include "xprec/ddouble.h"

/**
 * Class wrapping a MPFR object at certain precision.
 */
class MPFloat
{
public:
    MPFloat()
    {
        _x = (mpfr_ptr) malloc(sizeof(mpfr_t));
        if (_x == nullptr)
            throw std::bad_alloc();
        mpfr_init2(_x, precision);
    }

    template <typename T>
    MPFloat(const T &x) : MPFloat() { *this = x; }

    MPFloat(const MPFloat &other) : MPFloat() { mpfr_set(_x, other._x, round); }

    MPFloat(MPFloat &&other) : _x(nullptr) { swap(*this, other); }

    MPFloat &operator=(MPFloat other) { swap(*this, other); return *this; }

    ~MPFloat()
    {
        if (_x != nullptr) {
            mpfr_clear(_x);
            free(_x);
        }
    }

    void swap(MPFloat &left, MPFloat &right) { std::swap(left._x, right._x); }


    #define _DECLARE_ASSIGN(type, func)                                      \
        MPFloat &operator=(type x) {                                         \
            func(_x, x, round);                                              \
            return *this;                                                    \
        }

    _DECLARE_ASSIGN(float, mpfr_set_flt)
    _DECLARE_ASSIGN(double, mpfr_set_d)
    _DECLARE_ASSIGN(long double, mpfr_set_ld)
    _DECLARE_ASSIGN(bool, mpfr_set_ui)
    _DECLARE_ASSIGN(unsigned char, mpfr_set_ui)
    _DECLARE_ASSIGN(unsigned short, mpfr_set_ui)
    _DECLARE_ASSIGN(unsigned int, mpfr_set_ui)
    _DECLARE_ASSIGN(unsigned long, mpfr_set_ui)
    _DECLARE_ASSIGN(unsigned long long, mpfr_set_uj)
    _DECLARE_ASSIGN(signed char, mpfr_set_si)
    _DECLARE_ASSIGN(short, mpfr_set_si)
    _DECLARE_ASSIGN(int, mpfr_set_si)
    _DECLARE_ASSIGN(long, mpfr_set_si)
    _DECLARE_ASSIGN(long long, mpfr_set_sj)

    // ddouble stuff
    MPFloat &operator=(const DDouble &x)
    {
        *this = x.hi();
        *this += x.lo();
        return *this;
    }
    DDouble as_ddouble() const
    {
        double hi = mpfr_get_d(_x, round);
        MPFloat lo_f = (*this) - hi;
        double lo = mpfr_get_d(lo_f._x, round);
        return DDouble(hi, lo);
    }

    #define _DECLARE_BINARY_OP_RIGHT(op, func)                               \
        friend MPFloat op(const MPFloat &left, const MPFloat &right) {       \
            return MPFloat::binary_op(                                       \
                mpfr_ ## func, (mpfr_srcptr)left._x, (mpfr_srcptr)right._x); \
        }                                                                    \
        friend MPFloat op(const MPFloat &left, double right) {               \
            return MPFloat::binary_op(                                       \
                mpfr_ ## func ## _d, (mpfr_srcptr)left._x, right);           \
        }                                                                    \
        friend MPFloat op(const MPFloat &left, unsigned long right) {        \
            return MPFloat::binary_op(                                       \
                mpfr_ ## func ## _ui, (mpfr_srcptr)left._x, right);          \
        }                                                                    \
        friend MPFloat op(const MPFloat &left, long right) {                 \
            return MPFloat::binary_op(                                       \
                mpfr_ ## func ## _si, (mpfr_srcptr)left._x, right);          \
        }                                                                    \
        template <typename T>                                                \
        friend MPFloat op(const MPFloat &left, T right) {                    \
            return op(left, MPFloat(right));                                 \
        }

    _DECLARE_BINARY_OP_RIGHT(operator+, add)
    _DECLARE_BINARY_OP_RIGHT(operator-, sub)
    _DECLARE_BINARY_OP_RIGHT(operator*, mul)
    _DECLARE_BINARY_OP_RIGHT(operator/, div)


    #define _DECLARE_BINARY_OP_COMM(op, func)                                \
        friend MPFloat op(double right, const MPFloat &left) {               \
            return MPFloat::binary_op(                                       \
                mpfr_ ## func ## _d, (mpfr_srcptr)left._x, right);           \
        }                                                                    \
        friend MPFloat op(unsigned long right, const MPFloat &left) {        \
            return MPFloat::binary_op(                                       \
                mpfr_ ## func ## _ui, (mpfr_srcptr)left._x, right);          \
        }                                                                    \
        friend MPFloat op(long right, const MPFloat &left) {                 \
            return MPFloat::binary_op(                                       \
                mpfr_ ## func ## _si, (mpfr_srcptr)left._x, right);          \
        }                                                                    \
        template <typename T>                                                \
        friend MPFloat op(T left, const MPFloat &right) {                    \
            return op(MPFloat(left), right);                                 \
        }

    #define _DECLARE_BINARY_OP_LEFT(op, func)                                \
        friend MPFloat op(double right, const MPFloat &left) {               \
            return MPFloat::binary_op(                                       \
                mpfr_d_ ## func, right, (mpfr_srcptr)left._x);               \
        }                                                                    \
        friend MPFloat op(unsigned long right, const MPFloat &left) {        \
            return MPFloat::binary_op(                                       \
                mpfr_ui_ ## func, right, (mpfr_srcptr)left._x);              \
        }                                                                    \
        friend MPFloat op(long right, const MPFloat &left) {                 \
            return MPFloat::binary_op(                                       \
                mpfr_si_ ## func, right, (mpfr_srcptr)left._x);              \
        }                                                                    \
        template <typename T>                                                \
        friend MPFloat op(T left, const MPFloat &right) {                    \
            return op(MPFloat(left), right);                                 \
        }

    _DECLARE_BINARY_OP_COMM(operator+, add)
    _DECLARE_BINARY_OP_LEFT(operator-, sub)
    _DECLARE_BINARY_OP_COMM(operator*, mul)
    _DECLARE_BINARY_OP_LEFT(operator/, div)


    #define _DECLARE_INPLACE_OP(op, func)                                    \
        MPFloat &op(const MPFloat &right) {                                  \
            return inplace_op(mpfr_ ## func, (mpfr_srcptr)right._x);         \
        }                                                                    \
        MPFloat &op(double right) {                                          \
            return inplace_op(mpfr_ ## func ## _d, right);                   \
        }                                                                    \
        template <typename T>                                                \
        MPFloat &op(const T &right) {                                        \
            return op(MPFloat(right));                                       \
        }

    _DECLARE_INPLACE_OP(operator+=, add)
    _DECLARE_INPLACE_OP(operator-=, sub)
    _DECLARE_INPLACE_OP(operator*=, mul)
    _DECLARE_INPLACE_OP(operator/=, div)


    #define _DECLARE_RELATIONAL_OP(op, func)                                 \
        friend bool op(const MPFloat &left, const MPFloat &right) {          \
            return func(left._x, right._x);                                  \
        }                                                                    \
        template <typename T>                                                \
        friend bool op(const MPFloat &left, T right) {                       \
            return func(left._x, MPFloat(right)._x);                         \
        }                                                                    \
        template <typename T>                                                \
        friend bool op(T left, const MPFloat &right) {                       \
            return func(MPFloat(left)._x, right._x);                         \
        }

    _DECLARE_RELATIONAL_OP(operator<,  mpfr_less_p)
    _DECLARE_RELATIONAL_OP(operator>,  mpfr_greater_p)
    _DECLARE_RELATIONAL_OP(operator<=, mpfr_lessequal_p)
    _DECLARE_RELATIONAL_OP(operator>=, mpfr_greaterequal_p)
    _DECLARE_RELATIONAL_OP(operator==, mpfr_equal_p)
    _DECLARE_RELATIONAL_OP(operator!=, mpfr_notequal_p)


    #define _DECLARE_UNARY_OP(op, func)                                      \
        friend MPFloat op(const MPFloat &x)                                  \
        {                                                                    \
            MPFloat res;                                                     \
            func(res._x, x._x, round);                                       \
            return res;                                                      \
        }

    _DECLARE_UNARY_OP(operator-, mpfr_neg)
    _DECLARE_UNARY_OP(abs,   mpfr_abs)
    _DECLARE_UNARY_OP(sqrt,  mpfr_sqrt)

    _DECLARE_UNARY_OP(log,   mpfr_log)
    _DECLARE_UNARY_OP(log2,  mpfr_log2)
    _DECLARE_UNARY_OP(log10, mpfr_log10)
    _DECLARE_UNARY_OP(log1p, mpfr_log1p)
    _DECLARE_UNARY_OP(exp,   mpfr_exp)
    _DECLARE_UNARY_OP(exp2,  mpfr_exp2)
    _DECLARE_UNARY_OP(exp10, mpfr_exp10)
    _DECLARE_UNARY_OP(expm1, mpfr_expm1)

    _DECLARE_UNARY_OP(cos,   mpfr_cos)
    _DECLARE_UNARY_OP(sin,   mpfr_sin)
    _DECLARE_UNARY_OP(tan,   mpfr_tan)
    _DECLARE_UNARY_OP(acos,  mpfr_acos)
    _DECLARE_UNARY_OP(asin,  mpfr_asin)
    _DECLARE_UNARY_OP(atan,  mpfr_atan)

    _DECLARE_UNARY_OP(cosh,  mpfr_cosh)
    _DECLARE_UNARY_OP(sinh,  mpfr_sinh)
    _DECLARE_UNARY_OP(tanh,  mpfr_tanh)
    _DECLARE_UNARY_OP(acosh, mpfr_acosh)
    _DECLARE_UNARY_OP(asinh, mpfr_asinh)
    _DECLARE_UNARY_OP(atanh, mpfr_atanh)


    #define _DECLARE_BINARY_FUNC(op, func)                                   \
        friend MPFloat op(const MPFloat &left, const MPFloat &right) {       \
            return MPFloat::binary_op(                                       \
                func, (mpfr_srcptr)left._x, (mpfr_srcptr)right._x);          \
        }

    _DECLARE_BINARY_FUNC(atan2, mpfr_atan2)
    _DECLARE_BINARY_FUNC(hypot, mpfr_hypot)
    _DECLARE_BINARY_FUNC(pow, mpfr_pow)


    friend std::ostream &operator<<(std::ostream &out, const MPFloat &x)
    {
        std::string fmt = formatcode(out);

        char *str = nullptr;
        if (mpfr_asprintf(&str, fmt.c_str(), round, x._x) >= 0) {
            out << str;
            mpfr_free_str(str);
        } else {
            out.setstate(std::ios::failbit);
        }
        return out;
    }

    static const mpfr_prec_t precision = 200;
    static const mpfr_rnd_t round = MPFR_RNDN;

private:
    mpfr_ptr _x;

    template <typename L, typename R>
    static MPFloat binary_op(int (*op)(mpfr_ptr, L, R, mpfr_rnd_t),
                             L left, R right)
    {
        MPFloat res;
        op(res._x, left, right, round);
        return res;
    }

    template <typename R>
    MPFloat &inplace_op(int (*op)(mpfr_ptr, mpfr_srcptr, R, mpfr_rnd_t),
                        R right)
    {
        op(_x, _x, right, round);
        return *this;
    }

    static int mpfr_notequal_p(mpfr_srcptr left, mpfr_srcptr right)
    {
        return !mpfr_equal_p(left, right);
    }

    static std::string formatcode(const std::ostream &out)
    {
        std::ostringstream res;
        const std::ios::fmtflags ioflags = out.flags();

        res << '%';
        if (ioflags & std::ios::showpos)
            res << '+';
        if (out.width() > 0)
            res << out.width();
        // this is a bit hacky
        //res << '.' << out.precision();
        res << ".35";
        res << "R*";
        if ((ioflags & std::ios::floatfield) == std::ios::fixed)
            res << (out.flags() & std::ios_base::uppercase ? 'F' : 'f');
        else if ((ioflags & std::ios::floatfield) == std::ios::scientific)
            res << (out.flags() & std::ios_base::uppercase ? 'E' : 'e');
        else
            res << (out.flags() & std::ios_base::uppercase ? 'G' : 'g');;
        return res.str();
    }
};
