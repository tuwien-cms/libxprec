Small double-double library
===========================

Emulates quadruple precision with a pair of doubles.  This roughly doubles
the mantissa bits (and thus squares the precision of double).  The range
is almost the same as double, with a larger area of denormalized numbers.

The rough cost in floating point operations (flops) and relative error as
multiples of u² = 1.32e-32 (round-off error or half the machine epsilon) is
as follows:

    double d;
    xprec::ExDouble xd;        // double "cast" to quad precsion
    xprec::DDouble dd;         // emulated quad precision number

  | (op)       | xd (op) d | error | dd (op) d | error | dd (op) dd | error |
  |------------|----------:|------:|----------:|------:|-----------:|------:|
  | add_small  |   3 flops |   0u² |   7 flops |   2u² |   17 flops |   3u² |
  | + -        |   6 flops |   0u² |  10 flops |   2u² |   20 flops |   3u² |
  | *          |   2 flops |   0u² |   6 flops |   2u² |    9 flops |   4u² |
  | /          |  3* flops |   1u² |  10 flops |   3u² |  31* flops |   6u² |
  | reciprocal |  3* flops |   1u² |  14 flops | 2.3u² |  22* flops | 2.3u² |

The error bounds are tight analytical bounds [^2], except in the case of
double-double division, where the bound is 10u² but largest observed error
is 6u², and double by double division, where we expect u².  We report the
largest observed error here [^1]. * indicates the need for one or two double
divisions, which are about an order of magnitude more expensive than regular
flops on a modern CPU.

The table can be distilled into two rules of thumb: double-double arithmetic
roughly doubles the number of significant digits at the cost of a roughly 
15x slowdown compared to double arithmetic.

[^1]: M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
[^2]: J.-M. Muller and L. Rideau, ACM Trans. Math. Softw. 48, 1, 9 (2022)

Usage
-----
Simple example:

    #include <iostream>
    #include <xprec/ddouble.h>

    int main()
    {
      xprec::DDouble x = 1.0;                // emulated quad precision
      x = (4 - x) / (x + 6);                 // arithmetic operators work
      std::cout << x << std::endl;           // output to full precision
      std::cout << x.hi() << std::endl;      // output truncated to double
      std::cout << exp(x) << std::endl;      // higher-precision exp
    }

Installation
------------
libxprec has no mandatory dependencies other than a C++11-compliant compiler.

    mkdir build
    cd build
    cmake .. [EXTRA_CMAKE_FLAGS_GO_HERE]
    make
    ./test/tests      # requires -DBUILD_TESTING=ON
    make install      # set -DCMAKE_INSTALL_PREFIX to customize install dir

Useful CMake flags:

 - `-DBUILD_TESTING=ON`: builds unit tests. You need to have the [GNU MPFR]
   library installed for this to work.

 - `-DCMAKE_CXX_FLAGS=-mfma`: the double-double arithmetic in libxprec is much
   faster when using the fused-multiply add (FMA) instruction, which should
   be available on most modern CPUs. We recommend adding this flag unless you
   require portable binaries.

 - `-DCMAKE_INSTALL_PREFIX=/path/to/usr`: sets the base directory below which
   to install include files and the shared object.

#### Header-only mode ####
libxprec can also be used in header-only mode, which does not require
installation. For this, simply drop the full libxprec directory into your project
and use the following header:

    #include "libxprec/include/xprec/ddouble-header-only.h"

Please note that this will likely lead to considerably longer compile times.

[GNU MPFR]: https://www.mpfr.org/

#### Import in other CMake project ####
In order to use the library in CMake projects, we recommend using [FetchContent]:

    include(FetchContent)
    FetchContent_Declare(XPrec
        GIT_REPOSITORY https://github.com/tuwien-cms/libxprec
        GIT_TAG v0.5.0
        )
    FetchContent_MakeAvailable(XPrec)

You then should be able to simply link against the `XPrec::xprec` target.

[FetchContent]: https://cmake.org/cmake/help/latest/module/FetchContent.html

License and Copying
-------------------
Copyright (C) 2023 Markus Wallerberger and others.

Released under the MIT license (see LICENSE for details).
