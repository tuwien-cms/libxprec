# Copyright (C) 2022 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config/cmake")

project(
    ddouble
    LANGUAGES CXX
    VERSION 0.1.0
    DESCRIPTION "library for emulated extended-precision arithmetic"
    )

# Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(ddouble SHARED
    src/ddouble.cxx
    src/io.cxx
    )
target_compile_options(ddouble PRIVATE -Wall -Wextra)


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
    test/mpfloat.cxx
    test/arith.cxx
    test/convert.cxx
    )
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain)
target_link_libraries(tests PRIVATE MPFR::MPFR)
target_link_libraries(tests PRIVATE ddouble)
target_include_directories(tests PRIVATE src)
target_compile_options(tests PRIVATE -Wall -Wextra)
