#include "use-mpfr.h"

DDouble ddouble_from_mpfr(mpfr_srcptr x)
{
    mpfr_prec_t x_prec = mpfr_get_prec(x);
    assert(x_prec >= 110);

    double hi = mpfr_get_d(x, MPFR_RNDN);
    mpfr_t x_lo;
    mpfr_init2(x_lo, x_prec);
    mpfr_sub_d(x_lo, x, hi, MPFR_RNDD);
    double lo = mpfr_get_d(x_lo, MPFR_RNDN);
    return DDouble(hi, lo);
}

void ddouble_set_mpfr(mpfr_ptr x, DDouble y)
{
    mpfr_prec_t x_prec = mpfr_get_prec(x);
    assert(x_prec >= 110);

    mpfr_set_d(x, y.hi(), MPFR_RNDD);
    mpfr_add_d(x, x, y.lo(), MPFR_RNDD);
}
