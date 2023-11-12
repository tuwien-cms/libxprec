#pragma once
#include <new>
#include <utility>
#include <cstdlib>

#include <mpfr.h>

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

    MPFloat(double x) : MPFloat() { mpfr_set_d(_x, x, round); }

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
        }

    _DECLARE_INPLACE_OP(operator+=, add)
    _DECLARE_INPLACE_OP(operator-=, sub)
    _DECLARE_INPLACE_OP(operator*=, mul)
    _DECLARE_INPLACE_OP(operator/=, div)


    #define _DECLARE_RELATIONAL_OP(op, func)                                 \
        friend bool op(const MPFloat &left, const MPFloat &right) {          \
            return mpfr_ ## func ## _p(left._x, right._x);                   \
        }

    _DECLARE_RELATIONAL_OP(operator<,  less)
    _DECLARE_RELATIONAL_OP(operator>,  greater)
    _DECLARE_RELATIONAL_OP(operator<=, lessequal)
    _DECLARE_RELATIONAL_OP(operator>=, greaterequal)
    _DECLARE_RELATIONAL_OP(operator==, equal)

private:
    mpfr_ptr _x;
    
    static const mpfr_prec_t precision = 140;
    static const mpfr_rnd_t round = MPFR_RNDN;

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
};
