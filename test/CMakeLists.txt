# Copyright (C) 2023 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
include(FetchContent)

# Require Catch2 for tests
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
    FIND_PACKAGE_ARGS 3
    )

# Goes through find_package first
FetchContent_MakeAvailable(Catch2)

# MPFR is required for tests
find_package(MPFR REQUIRED)
find_package(Eigen3 3.3)

add_executable(tests
    arith.cpp
    circular.cpp
    convert.cpp
    exp.cpp
    gauss.cpp
    hyperbolic.cpp
    inline.cpp
    limits.cpp
    mpfloat.cpp
    random.cpp
    round.cpp
    sqrt.cpp
    )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE MPFR::MPFR)
target_link_libraries(tests PRIVATE XPrec::xprec)
target_compile_options(tests PRIVATE -Wall -Wextra)

if (TARGET Eigen3::Eigen)
    target_sources(tests PRIVATE eigen.cpp)
    target_link_libraries(tests PRIVATE Eigen3::Eigen)
endif()

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.12")
    set_property(TARGET tests PROPERTY CXX_STANDARD 20)
else()
    set_property(TARGET tests PROPERTY CXX_STANDARD 17)
endif()
add_test(tests tests)
