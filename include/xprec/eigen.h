/* Adapter for using Eigen
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <Eigen/Core>
#include "ddouble.h"

namespace Eigen {

template <>
struct NumTraits<DDouble>
    : GenericNumTraits<DDouble>
{
    typedef DDouble Real;
    typedef DDouble NonInteger;
    typedef DDouble Nested;

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
