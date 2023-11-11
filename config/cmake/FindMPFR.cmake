# Find MPFR library
#
# Sets the following variables:
#
#  - MPFR_INCLUDE_DIRS : header files
#  - MPFR_LIBRARIES    : libraries
#  - MPFR_VERSION      : version of the libarary
#
include(FindPackageHandleStandardArgs)

find_path(
    MPFR_INCLUDE_DIRS
    NAMES mpfr.h
    PATHS ${INCLUDE_INSTALL_DIR}
    DOC "GNU MPFR - include directories")

find_library(
    MPFR_LIBRARIES
    NAMES mpfr
    PATHS ${LIB_INSTALL_DIR}
    DOC "GNU MPFR - libraries")

function(mpfr_find_version version)
    foreach(incdir "${MPFR_INCLUDE_DIRS}")
        set(filename "${incdir}/mpfr.h")
        if (EXISTS "${filename}")
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
            break()
        endif()
    endforeach()
endfunction()

mpfr_find_version(MPFR_VERSION)

find_package_handle_standard_args(MPFR
    REQUIRED_VARS MPFR_INCLUDE_DIRS MPFR_LIBRARIES
    VERSION_VAR MPFR_VERSION
    )
