/* Small double-double arithmetic library.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#pragma once

//The strategy for header only use of ddouble is pretty simple:
//conditionally inline globally defined functions and include the cxx files directly.
#define _XPREC_INLINE_IF_HEADER_ONLY inline

#include "ddouble.h"
#include "xprec/../../src/circular.cxx"
#include "xprec/../../src/ddouble.cxx"
#include "xprec/../../src/exp.cxx"
#include "xprec/../../src/gauss.cxx"
#include "xprec/../../src/hyperbolic.cxx"
#include "xprec/../../src/io.cxx"
