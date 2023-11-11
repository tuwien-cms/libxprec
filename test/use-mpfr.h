#pragma once
#include <mpfr.h>
#include "ddouble.h"

DDouble ddouble_from_mpfr(mpfr_srcptr x);

void ddouble_set_mpfr(mpfr_ptr x, DDouble y);

