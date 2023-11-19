# Find MPFR library
#
# Understands the following envirnoment variables:
#
#  - MPFR_ROOT         : path to mpfr
#
# Allows the following cache variables (detected by default):
#
#  - MPFR_INCLUDE_DIR  : directory to include file
#  - MPFR_LIBRARY      : path to library
#
# Sets the following variables:
#
#  - MPFR_FOUND        : if MPFR has been found
#  - MPFR_INCLUDE_DIRS : header files
#  - MPFR_LIBRARIES    : libraries
#  - MPFR_VERSION      : version of the libarary
#
# Imported targets:
#
#  - MPFR::MPFR        : imported target if library has been found
#
# Copyright (C) 2022 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
include(FindPackageHandleStandardArgs)

find_path(MPFR_INCLUDE_DIR
    NAMES mpfr.h
    HINTS ENV MPFR_ROOT
    PATH_SUFFIXES include
    DOC "GNU MPFR - include directory"
    )
mark_as_advanced(MPFR_INCLUDE_DIR)

find_library(MPFR_LIBRARY
    NAMES mpfr
    HINTS ENV MPFR_ROOT
    PATH_SUFFIXES lib
    DOC "GNU MPFR - library"
    )
mark_as_advanced(MPFR_LIBRARY)

# Function to extract version string from header file
function(mpfr_find_version version filename)
    file(READ "${filename}" header)

    string(REGEX MATCH
        "define[ \t]+MPFR_VERSION_MAJOR[ \t]+([0-9]+)"
        match "${header}")
    set(_version "${CMAKE_MATCH_1}")

    string(REGEX MATCH
        "define[ \t]+MPFR_VERSION_MINOR[ \t]+([0-9]+)"
        match "${header}")
    set(_version "${_version}.${CMAKE_MATCH_1}")

    string(REGEX MATCH
        "define[ \t]+MPFR_VERSION_PATCHLEVEL[ \t]+([0-9]+)"
        match "${header}")
    set(_version "${_version}.${CMAKE_MATCH_1}")

    set("${version}" "${_version}" PARENT_SCOPE)
endfunction()

if(MPFR_INCLUDE_DIR)
    mpfr_find_version(MPFR_VERSION "${MPFR_INCLUDE_DIR}/mpfr.h")
endif()

find_package_handle_standard_args(MPFR
    REQUIRED_VARS MPFR_LIBRARY MPFR_INCLUDE_DIR
    VERSION_VAR MPFR_VERSION
    )

if(MPFR_FOUND)
    set(MPFR_INCLUDE_DIRS "${MPFR_INCLUDE_DIR}")
    set(MPFR_LIBRARIES "${MPFR_LIBRARY}")

    if(NOT TARGET MPFR::MPFR)
        add_library(MPFR::MPFR UNKNOWN IMPORTED)
        set_target_properties(MPFR::MPFR PROPERTIES
            IMPORTED_LOCATION "${MPFR_LIBRARY}"
            INTERFACE_INCLUDE_DIRECTORIES "${MPFR_INCLUDE_DIR}"
            )
    endif()
endif()
