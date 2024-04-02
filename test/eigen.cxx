/* Tests
 *
 * Copyright (C) 2024Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include <catch2/catch_test_macros.hpp>
#include "xprec/ddouble.h"
#include "xprec/eigen.h"
#include "catch2-addons.h"

#include <Eigen/Dense>

TEST_CASE("eigenvalues", "[eigen]" )
{
    using Eigen::Dynamic;

    Eigen::Matrix<DDouble, Dynamic, Dynamic> A(21, 21);
    for (int j = 0; j != A.cols(); ++j)
        for (int i = 0; i != A.rows(); ++i)
            A(i, j) = reciprocal(ExDouble(i + j + 1));

    Eigen::SelfAdjointEigenSolver<decltype(A)> A_eig(A);
    // TODO
}
