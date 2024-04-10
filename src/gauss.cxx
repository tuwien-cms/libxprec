/* Gauss-Legendre quadrature
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include "xprec/numbers.h"
#include <cassert>

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

XPREC_API_EXPORT
void gauss_chebyshev(int n, DDouble x[], DDouble w[])
{
    if (n < 1)
        return;

    DDouble fact = xprec::numbers::pi / (1.0 * n);
    for (int i = 0; i < n; ++i) {
        x[i] = cos((n - i - 0.5) * fact);
        if (w != nullptr)
            w[i] = fact;
    }
}

static void leg_deriv(int N, DDouble x, DDouble &Pn, DDouble &dPn)
{
    assert(N >= 1);
    assert(greater_in_magnitude(1.0, x));

    DDouble Pn_1 = 1.0;
    DDouble dPn_1 = 0.0;
    Pn = x;
    dPn = 1.0;
    for (int n = 1; n < N; ++n) {
        // compute next term by Bonnet's recursion formula
        DDouble Pnext = ((2*n + 1.0) * x * Pn - n * Pn_1) / (n + 1.0);
        DDouble dPnext = ((2*n + 1.0) * (x * dPn + Pn) - n * dPn_1) / (n + 1.0);

        // shift terms by one
        Pn_1 = Pn;
        Pn = Pnext;
        dPn_1 = dPn;
        dPn = dPnext;
    }
}

XPREC_API_EXPORT
void gauss_legendre(int n, DDouble x[], DDouble w[])
{
    if (n < 1)
        return;

    // Initial guess for x: Gauss-Chebyshev nodes
    gauss_chebyshev(n, x);

    // Perform Newton iteration to refine x
    // store derivatives in w for later use
    for (int iter = 0; iter < 10; ++iter) {
        bool converged = true;
        for (int i = 0; i < n; ++i) {
            DDouble Pn, dPn, dx;
            leg_deriv(n, x[i], Pn, dPn);
            dx = -Pn / dPn;
            x[i] += dx;
            if (w != nullptr)
                w[i] = dPn;

            if (converged && !greater_in_magnitude(2.5e-32, dx))
                converged = false;
        }
        if (converged)
            break;
    }

    // Compute weights if so desired
    if (w != nullptr) {
        for (int i = 0; i < n; ++i) {
            w[i] = 2.0 / ((1.0 - x[i] * x[i]) * w[i] * w[i]);
        }
    }
}

} /* namespace xprec */
