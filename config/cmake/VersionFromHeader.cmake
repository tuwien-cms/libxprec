# Utility for extracting version from C header
#
# Copyright (C) 2022 Markus Wallerberger and others
# SPDX-License-Identifier: MIT

# Function to extract version string from header file
function(version_from_header version filename
         macro_major macro_minor macro_patch)
    file(READ "${filename}" header)

    string(REGEX MATCH
        "define[ \t]+${macro_major}[ \t]+([0-9]+)"
        match "${header}")
    set(_version "${CMAKE_MATCH_1}")

    string(REGEX MATCH
        "define[ \t]+${macro_minor}[ \t]+([0-9]+)"
        match "${header}")
    set(_version "${_version}.${CMAKE_MATCH_1}")

    string(REGEX MATCH
        "define[ \t]+${macro_patch}[ \t]+([0-9]+)"
        match "${header}")
    set(_version "${_version}.${CMAKE_MATCH_1}")

    set("${version}" "${_version}" PARENT_SCOPE)
endfunction()
