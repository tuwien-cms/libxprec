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

enable_testing()

add_executable(tests
    arith.cxx
    circular.cxx
    convert.cxx
    exp.cxx
    funcs.cxx
    gauss.cxx
    hyperbolic.cxx
    inline.cxx
    mpfloat.cxx
    )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE MPFR::MPFR)
target_link_libraries(tests PRIVATE xprec)
target_include_directories(tests PRIVATE ../include)
target_compile_options(tests PRIVATE -Wall -Wextra)

if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.12")
    set_property(TARGET tests PROPERTY CXX_STANDARD 20)
else()
    set_property(TARGET tests PROPERTY CXX_STANDARD 17)
endif()
add_test(tests tests)
