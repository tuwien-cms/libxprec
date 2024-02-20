/* Small double-double arithmetic library.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once

// The strategy for header only use of ddouble is pretty simple:
// conditionally inline globally defined functions and include the cxx files
// directly.
#define XPREC_API_EXPORT inline

#include "ddouble.h"
#include "../../src/circular.cxx"
#include "../../src/ddouble.cxx"
#include "../../src/exp.cxx"
#include "../../src/gauss.cxx"
#include "../../src/hyperbolic.cxx"
#include "../../src/io.cxx"
