/* Tests
 *
 * Copyright (C) 2024Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/eigen.hpp"
#include "catch2-addons.hpp"
#include "xprec/ddouble.hpp"
#include <catch2/catch_test_macros.hpp>

#include <Eigen/Dense>

using Eigen::Dynamic;

TEST_CASE("eigenvalues", "[eigen]")
{
    Eigen::Matrix<DDouble, Dynamic, Dynamic> A(21, 21);
    for (int j = 0; j != A.cols(); ++j)
        for (int i = 0; i != A.rows(); ++i)
            A(i, j) = xprec::reciprocal(xprec::ExDouble(i + j + 1));

    Eigen::SelfAdjointEigenSolver<decltype(A)> A_eig(A);
    // TODO
}

TEST_CASE("prod", "[eigen]")
{
    Eigen::Matrix<DDouble, Dynamic, Dynamic> A(21, 21);
    for (int j = 0; j != A.cols(); ++j)
        for (int i = 0; i != A.rows(); ++i)
            A(i, j) = xprec::reciprocal(xprec::ExDouble(i + j + 1));

    auto A2 = A * A;
    REQUIRE(A2.sum() > 0);
}

TEST_CASE("singular values", "[eigen]")
{
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
