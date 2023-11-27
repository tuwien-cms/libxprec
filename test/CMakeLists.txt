# Copyright (C) 2023 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
# Require Catch2 for tests
find_package(Catch2 3)
if (NOT Catch2_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        Catch2
        GIT_REPOSITORY https://github.com/catchorg/Catch2.git
        GIT_TAG v3.4.0
        )
    FetchContent_MakeAvailable(Catch2)
endif()

# MPFR is required for tests
find_package(MPFR REQUIRED)

add_executable(tests
    mpfloat.cxx
    convert.cxx
    arith.cxx
    funcs.cxx
    exp.cxx
    circular.cxx
    hyperbolic.cxx
    )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE MPFR::MPFR)
target_link_libraries(tests PRIVATE xprec)
target_include_directories(tests PRIVATE ../include)
target_compile_options(tests PRIVATE -Wall -Wextra)
