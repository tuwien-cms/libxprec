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
    src/exp.cxx
    src/circular.cxx
    src/hyperbolic.cxx
    src/io.cxx
    )
target_compile_options(ddouble PRIVATE -Wall -Wextra -pedantic)
set_target_properties(ddouble PROPERTIES
    PUBLIC_HEADER "src/ddouble.h;src/ddouble-impl.h"
    )

install(TARGETS ddouble)

# TESTS
if (BUILD_TESTING)
    add_subdirectory("test")
endif()
