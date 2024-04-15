/* Trigonometric functions to quad precision.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include "xprec/numbers.h"
#include "taylor.h"

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

XPREC_API_EXPORT
DDouble trig_complement(DDouble x)
{
    if (fabs(x) > 1.0)
        return NAN;
    if (fabs(x.hi()) > 0.5)
        return sqrt(1 - x * x);

    // sqrt(1 - x*x) loses half its digits of precision for small x.  But this
    // means that half of the digits are accurate, so we compute the function
    // for the hi part first.
    ExDouble x0 = x.hi(), dx = x.lo();
    DDouble y0 = sqrt(1.0 - x0 * x0);

    // Now we have the Taylor expansion:
    //
    //     f(x) = sqrt(1 - x*x) = f(x0) - x0/f(x0) * (x - x0) + ...
    //
    // where it is sufficient to take just one term.
    DDouble dy = -x0 * dx / y0;
    return y0.add_small(dy);
}

static DDouble sin_kernel(DDouble x, int n=13)
{
    // We need this to go out to pi/4 ~= 0.785
    // Convergence of the Taylor approx to 2e-32
    assert(n >= 0 && n <= 13);

    // Taylor series of the sin around 0
    DDouble xsq = -x * x;
    DDouble r = x;
    DDouble xpow = x;
    for (int i = 3; i <= 2 * n + 1; i += 2) {
        xpow *= xsq;
        r = r.add_small(reciprocal_factorial(i) * xpow);
    }
    return r;
}

static DDouble cos_kernel(DDouble x, int n=13)
{
    // We need this to go out to pi/4 ~= 0.785
    // Convergence of the Taylor approx to 2e-32
    assert(n >= 0 && n <= 13);

    // Taylor series of the cos around 0
    DDouble xsq = -x * x;
    DDouble r = 1.0;
    DDouble xpow = xsq;
    r = r.add_small(PowerOfTwo(0.5) * xpow);
    for (int i = 4; i <= 2 * n; i += 2) {
        xpow *= xsq;
        r = r.add_small(reciprocal_factorial(i) * xpow);
    }
    return r;
}

static DDouble remainder_pi2(DDouble x, int &sector)
{
    // This reduction has to be done quite carefully, because of the
    // remainder.
    using xprec::numbers::pi_half;
    DDouble n = x / pi_half;
    if (fabs(n.hi()) < 0.5) {
        sector = 0;
        return x;
    }

    // This is a problem for very large revolution count, but there we
    // have a problem anyway.
    n = round(n);
    int64_t n_int = n.as<int64_t>();
    sector = n_int % 4;
    if (sector < 0)
        sector += 4;
    return x - pi_half * n;
}

static DDouble sin_sector(DDouble x, int sector)
{
    using xprec::numbers::pi_4;
    assert(sector >= 0 && sector < 4);
    assert(fabs(x.hi()) <= nextafter(pi_4.hi(), 1));

    switch (sector) {
    case 0:
        return sin_kernel(x);
    case 1:
        // use sin(x) = cos(x - pi/2)
        return cos_kernel(x);
    case 2:
        // use sin(x) = -sin(x - pi)
        return -sin_kernel(x);
    default:
        return -cos_kernel(x);
    }
}

XPREC_API_EXPORT
DDouble sin(DDouble x)
{
    int sector;
    x = remainder_pi2(x, sector);
    return sin_sector(x, sector);
}

XPREC_API_EXPORT
DDouble cos(DDouble x)
{
    // For small values, we shall use the cosine directly
    using xprec::numbers::pi_4;
    if (fabs(x.hi()) < pi_4.hi())
        return cos_kernel(x);

    // Otherwise, use common code.
    int sector;
    x = remainder_pi2(x, sector);
    return sin_sector(x, (sector + 1) % 4);
}

XPREC_API_EXPORT
void sincos(DDouble x, DDouble &s, DDouble &c)
{
    // XXX This should be improved
    s = sin(x);
    c = cos(x);
}

XPREC_API_EXPORT
DDouble tan(DDouble x)
{
    DDouble s, c;
    sincos(x, s, c);
    return s / c;
}

XPREC_API_EXPORT
DDouble asin(DDouble x)
{
    // Compute a approximation to double precision
    DDouble y0 = std::asin(x.hi());
    if (!isfinite(y0))
        return y0;

    // This is where Taylor fails
    if (fabs(x) == 1.0) {
        return copysign(xprec::numbers::pi_half, x);
    }

    // Perform Taylor expansion:
    //
    //    asin(x) = asin(x0) + (x - x0) / sqrt(1 - x0**2)
    //            = y0 + (x - sin(y0)) / cos(y0)
    //
    DDouble x0, w;
    sincos(y0, x0, w);

    DDouble y = y0 + (x - x0) / w;
    return y;
}

XPREC_API_EXPORT
DDouble acos(DDouble x)
{
    // Compute a approximation to double precision
    DDouble y0 = std::acos(x.hi());
    if (!isfinite(y0))
        return y0;

    // This is where Taylor fails
    if (x == 1.0)
        return 0.0;
    if (x == -1.0)
        return xprec::numbers::pi;

    // Perform Taylor expansion:
    //
    //    acos(x) = acos(x0) - (x - x0) / sqrt(1 - x0**2)
    //            = y0 - (x - cos(y0)) / sin(y0)
    //
    DDouble x0, w, diff;

    sincos(y0, w, x0);
    diff = (x0 - x) / w;
    y0 += diff;
    return y0;
}

XPREC_API_EXPORT
DDouble atan(DDouble x)
{
    // For large values, use reflection formula
    if (fabs(x.hi()) > 1.0) {
        DDouble y = copysign(xprec::numbers::pi_half, x);
        if (isfinite(x))
            y -= atan(reciprocal(x));
        return y;
    }

    // Again use Taylor expansion
    DDouble y0 = std::atan(x.hi());
    if (!isfinite(y0))
        return y0;

    DDouble s, c, x0;

    sincos(y0, s, c);
    x0 = s / c;
    y0 += (x - x0) * c * c;

    return y0;
}

XPREC_API_EXPORT
DDouble atan2(DDouble y, DDouble x)
{
    using xprec::numbers::pi_half;
    using xprec::numbers::pi;

    // Special values
    if (isnan(x) || isnan(y))
        return NAN;
    if (iszero(y))
        return x.hi() >= 0 ? 0.0 : pi;
    if (iszero(x))
        return copysign(pi_half, y);

    DDouble res = atan(y / x);
    if (x.hi() < 0)
        res = copysign(pi, y).add_small(res);
    return res;
}

}
