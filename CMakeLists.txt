# Copyright (C) 2023 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config/cmake")

# extract version information from header file
if (NOT XPREC_VERSION)
    include("VersionFromHeader")
    version_from_header(
        XPREC_VERSION "include/xprec/version.h"
        XPREC_VERSION_MAJOR XPREC_VERSION_MINOR XPREC_VERSION_PATCH)
    message(STATUS "Extracted package version: ${XPREC_VERSION}")

    set(XPREC_VERSION "${XPREC_VERSION}"
        CACHE STRING "version of the xprec package")
    mark_as_advanced(XPREC_VERSION)
endif()

# Configure project
project(
    libxprec
    LANGUAGES CXX
    VERSION "${XPREC_VERSION}"
    DESCRIPTION "library for emulated extended-precision arithmetic"
    )

# Require C++11
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library
add_library(xprec SHARED
    src/ddouble.cxx
    src/exp.cxx
    src/circular.cxx
    src/hyperbolic.cxx
    src/io.cxx
    )
target_compile_options(xprec PRIVATE -Wall -Wextra -pedantic)
target_include_directories(xprec PUBLIC include)

install(TARGETS xprec)
install(
    DIRECTORY include/
    DESTINATION include
    )

# Tests
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory("test")
endif()
