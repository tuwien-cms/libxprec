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
    XPrec
    LANGUAGES CXX
    VERSION "${XPREC_VERSION}"
    DESCRIPTION "library for emulated extended-precision arithmetic"
    )

# Library
add_library(xprec SHARED
    src/circular.cxx
    src/exp.cxx
    src/gauss.cxx
    src/hyperbolic.cxx
    src/io.cxx
    src/sqrt.cxx
    )
if(NOT MSVC)
    target_compile_options(xprec PRIVATE -Wall -Wextra -pedantic)
endif()
target_include_directories(xprec PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
    )
set_target_properties(xprec PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
    CXX_STANDARD 11
    CXX_STANDARD_REQUIRED ON
    )

# Use library convention.
add_library(XPrec::xprec ALIAS xprec)

install(
    TARGETS xprec
    EXPORT xprec_exports
    )
install(
    DIRECTORY include/
    DESTINATION include
    PATTERN "ddouble-header-only.h" EXCLUDE
    )
install(
    EXPORT xprec_exports
    NAMESPACE XPrec::
    DESTINATION lib/cmake/XPrec
    )

set(XPREC_TEST OFF
    CACHE BOOL "Build xprec tests.")

# Tests
if (XPREC_TEST)
    enable_testing()
    add_subdirectory("test")
endif()
