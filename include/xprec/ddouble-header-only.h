/* Small double-double arithmetic library.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once

//Visual studio won't define M_PI unless _USE_MATH_DEFINES is defined, which we can't ensure with header only inclusion.
#ifndef M_PI
    #define M_PI 3.14159265358979323846
    #define CLEAN_M_PI
#endif

//The strategy for header only use of ddouble is pretty simple:
//conditionally inline globally defined functions and include the cxx files directly.
#define INLINE_IF_HEADER_ONLY inline

#include "ddouble.h"
#include "xprec/../../src/circular.cxx"
#include "xprec/../../src/ddouble.cxx"
#include "xprec/../../src/exp.cxx"
#include "xprec/../../src/gauss.cxx"
#include "xprec/../../src/hyperbolic.cxx"
#include "xprec/../../src/io.cxx"

//Clean up if needed.
#ifndef CLEAN_M_PI
    #undef M_PI
#endif

