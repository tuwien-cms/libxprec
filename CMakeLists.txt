# Copyright (C) 2023 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.9)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

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

# ---------------------------------
# Building

add_library(xprec SHARED
    src/cinterface.cpp
    src/circular.cpp
    src/exp.cpp
    src/floats.cpp
    src/gauss.cpp
    src/hyperbolic.cpp
    src/io.cpp
    src/sqrt.cpp
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
export(TARGETS xprec
    NAMESPACE XPrec::
    FILE XPrecTargets.cmake)

# ------------------------------
# Installation

# Provides CMAKE_INSTALL_* directories
include(GNUInstallDirs)
set(XPREC_INSTALL_INCLUDEDIR "${CMAKE_INSTALL_INCLUDEDIR}"
    CACHE PATH "directory into which to install xprec include files")
set(XPREC_INSTALL_LIBDIR "${CMAKE_INSTALL_LIBDIR}"
    CACHE PATH "directory into which to install xprec library files")
set(XPREC_INSTALL_CMAKEDIR "${CMAKE_INSTALL_DATADIR}/cmake/XPrec"
    CACHE PATH "directory into which to install xprec cmake files")
option(XPREC_INSTALL_CMAKE_PACKAGE "Installs CMake configuration files" ON)

install(TARGETS xprec
    DESTINATION "${XPREC_INSTALL_LIBDIR}"
    EXPORT XPrecTargets
    )
install(DIRECTORY include/
    DESTINATION "${XPREC_INSTALL_INCLUDEDIR}"
    PATTERN "ddouble-header-only.h" EXCLUDE
    )

if (XPREC_INSTALL_CMAKE_PACKAGE)
    # These are essentially magic incantantions copied from the CMake tutorial:
    include(CMakePackageConfigHelpers)
    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/package/XPrecConfig.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/XPrecConfig.cmake"
        INSTALL_DESTINATION "${XPREC_INSTALL_CMAKEDIR}"
        NO_SET_AND_CHECK_MACRO
        NO_CHECK_REQUIRED_COMPONENTS_MACRO
        )
    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/XPrecConfigVersion.cmake"
        VERSION "${XPREC_VERSION}"
        COMPATIBILITY SameMajorVersion
        )
    install(FILES
        "${CMAKE_CURRENT_BINARY_DIR}/XPrecConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/XPrecConfigVersion.cmake"
        DESTINATION "${XPREC_INSTALL_CMAKEDIR}"
        )
    install(EXPORT XPrecTargets
        NAMESPACE XPrec::
        DESTINATION "${XPREC_INSTALL_CMAKEDIR}"
        )
endif()

# -------------------------------------
# Testing

option(XPREC_BUILD_TESTING "Enable creation of xprec tests." ${BUILD_TESTING})

if (XPREC_BUILD_TESTING)
    enable_testing()
    add_subdirectory("test")
endif()
