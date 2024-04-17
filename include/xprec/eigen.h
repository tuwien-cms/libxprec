/* Adapter for using Eigen
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include "ddouble.h"
#include <Eigen/Core>

namespace Eigen {

template <>
struct NumTraits<xprec::DDouble> : GenericNumTraits<xprec::DDouble>
{
    typedef xprec::DDouble Real;
    typedef xprec::DDouble NonInteger;
    typedef xprec::DDouble Nested;

    enum {
        IsComplex = 0,
        IsInteger = 0,
        IsSigned = 1,
        RequireInitialization = 1,

        ReadCost = 1,
        AddCost = 20,
        MulCost = 9
    };
};

} /* namespace Eigen */
