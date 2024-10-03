/* Tests
 *
 * Copyright (C) 2024Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/eigen.h"
#include "catch2-addons.h"
#include "xprec/ddouble.h"
#include <catch2/catch_test_macros.hpp>

#include <Eigen/Dense>

TEST_CASE("eigenvalues", "[eigen]")
{
    using Eigen::Dynamic;

    Eigen::Matrix<DDouble, Dynamic, Dynamic> A(21, 21);
    for (int j = 0; j != A.cols(); ++j)
        for (int i = 0; i != A.rows(); ++i)
            A(i, j) = xprec::reciprocal(xprec::ExDouble(i + j + 1));

    Eigen::SelfAdjointEigenSolver<decltype(A)> A_eig(A);
    // TODO
}


TEST_CASE("Singular values", "[svd]")
{
    using Eigen::Dynamic;

    Eigen::Matrix<DDouble, Dynamic, Dynamic> A(21, 21);
    for (int j = 0; j != A.cols(); ++j)
        for (int i = 0; i != A.rows(); ++i)
            A(i, j) = xprec::reciprocal(xprec::ExDouble(i + j + 1));

    Eigen::JacobiSVD<decltype(A)> svd;
    svd.compute(A, Eigen::ComputeThinU | Eigen::ComputeThinV);

    auto U = svd.matrixU();
    auto V = svd.matrixV();
    auto S = svd.singularValues().asDiagonal();

    auto A_reconstructed = U * S * V.transpose();

    auto diff = A - A_reconstructed;

    auto reconstruction_error = diff.norm();
    REQUIRE(reconstruction_error < 1e-29);
}
