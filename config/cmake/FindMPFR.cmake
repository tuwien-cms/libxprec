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
# Copyright (C) 2023 Markus Wallerberger and others
# SPDX-License-Identifier: MIT
#
include(FindPackageHandleStandardArgs)

if (NOT MPFR_INCLUDE_DIR)
    find_path(MPFR_INCLUDE_DIR
        NAMES mpfr.h
        HINTS ENV MPFR_ROOT
        PATH_SUFFIXES include
        DOC "GNU MPFR - include directory"
        )
    mark_as_advanced(MPFR_INCLUDE_DIR)
endif()

if (NOT MPFR_LIBRARY)
    find_library(MPFR_LIBRARY
        NAMES mpfr
        HINTS ENV MPFR_ROOT
        PATH_SUFFIXES lib
        DOC "GNU MPFR - library"
        )
    mark_as_advanced(MPFR_LIBRARY)
endif()

if(MPFR_INCLUDE_DIR AND NOT MPFR_VERSION)
    include(VersionFromHeader)
    version_from_header(MPFR_VERSION
        HEADER "${MPFR_INCLUDE_DIR}/mpfr.h"
        MACROS MPFR_VERSION_MAJOR MPFR_VERSION_MINOR MPFR_VERSION_PATCHLEVEL
        )
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
