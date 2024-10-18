/* Small double-double arithmetic library - header-only version.
 *
 * To use this, simply drop the full libxprec directory into your project
 * and include this header.  Please note that this will likely lead to
 * considerably longer compile times.
 *
 * Please also note that this header WILL NOT be installed by `make install`.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once

// The strategy for header only use of ddouble is pretty simple:
// conditionally inline globally defined functions and include the cxx files
// directly.
#define XPREC_API_EXPORT inline

#include "../../src/circular.cpp"
#include "../../src/exp.cpp"
#include "../../src/floats.cpp"
#include "../../src/gauss.cpp"
#include "../../src/hyperbolic.cpp"
#include "../../src/io.cpp"
#include "../../src/sqrt.cpp"
#include "ddouble.hpp"
