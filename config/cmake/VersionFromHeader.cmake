# Utility for extracting version from C header
#
# Copyright (C) 2023 Markus Wallerberger and others
# SPDX-License-Identifier: MIT

# Function to extract version string from header file
function(version_from_header)
    # parse arguments
    set(options)
    set(one_value_args VERSION HEADER)
    set(multi_value_args MACROS)
    cmake_parse_arguments(_ARG "${options}" "${one_value_args}"
                          "${multi_value_args}" ${ARGN})

    # These are required
    if (NOT _ARG_VERSION)
        message(FATAL_ERROR "VERSION is required")
    endif()
    if (NOT _ARG_HEADER)
        message(FATAL_ERROR "HEADER is required")
    endif()
    if (NOT _ARG_MACROS)
        message(FATAL_ERROR "MACROS is required")
    endif()

    # read in the file
    file(READ "${_ARG_HEADER}" header)

    # build up version string from macros
    set(version "")
    foreach(macro_name IN LISTS _ARG_MACROS)
        if (header MATCHES "define[ \t]+${macro_name}[ \t]+([0-9]+)")
            set(chunk "${CMAKE_MATCH_1}")
            if (version STREQUAL "")
                set(version "${chunk}")
            else()
                set(version "${version}.${chunk}")
            endif()
        else()
            message(SEND_ERROR "${macro_name} not #defined in ${_ARG_HEADER}")
            return()
        endif()
    endforeach()

    # export back to parent scope ("pass by reference")
    set("${_ARG_VERSION}" "${version}" PARENT_SCOPE)
endfunction()
