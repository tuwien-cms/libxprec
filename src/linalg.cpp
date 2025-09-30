/* Linear algebra utilities.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.hpp"

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

static bool _jacobi2x2(DDouble x, DDouble y, DDouble z, DDouble *c, DDouble *s)
{
    DDouble deno = PowerOfTwo(2) * abs(y);
    if (deno.hi() < std::numeric_limits<DDouble>::min().hi()){
        *c = 1.0;
        *s = 0.0;
        return false;
    }

    DDouble tau = (x - z) / deno;
    DDouble w = sqrt(square(tau) + 1.0);
    DDouble ww = copysign(w, tau);
    DDouble t = reciprocal(ww.add_small(tau));
    DDouble n = invsqrt(square(t) + 1.0);

    *s = (y / abs(y)) * abs(t) * n;
    if (t.hi() > 0)
        *s = -*s;
    *c = n;
    return true;
}

extern "C" XPREC_API_EXPORT
bool xprec_jacobi2x2(xprec_ddouble x, xprec_ddouble y, xprec_ddouble z,
                     xprec_ddouble *c, xprec_ddouble *s)
{
    return _jacobi2x2(x, y, z, (xprec::DDouble *)c, (xprec::DDouble *)s);
}

}