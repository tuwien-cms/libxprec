# Copyright (C) 2023 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/config/cmake")

# extract version information from header file
if (NOT XPREC_VERSION)
    include("VersionFromHeader")
    version_from_header(XPREC_VERSION
        HEADER "include/xprec/version.h"
        MACROS XPREC_VERSION_MAJOR XPREC_VERSION_MINOR XPREC_VERSION_PATCH
        )
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

# Library
add_library(xprec SHARED
    src/circular.cxx
    src/ddouble.cxx
    src/exp.cxx
    src/gauss.cxx
    src/hyperbolic.cxx
    src/io.cxx
    )
if(NOT MSVC)
    target_compile_options(xprec PRIVATE -Wall -Wextra -pedantic)
endif()
target_include_directories(xprec PUBLIC include)
set_property(TARGET xprec PROPERTY
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    )

install(TARGETS xprec)
install(
    DIRECTORY include/
    DESTINATION include
    PATTERN "ddouble-header-only.h" EXCLUDE
    )

# Tests
if (BUILD_TESTING)
    enable_testing()
    add_subdirectory("test")
endif()
