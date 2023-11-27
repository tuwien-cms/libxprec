Small double-double library
===========================

Emulates quadruple precision with a pair of doubles.  This roughly doubles
the mantissa bits (and thus squares the precision of double).  The range
is almost the same as double, with a larger area of denormalized numbers.

The rough cost in floating point operations (flops) and relative error as
multiples of u² = 1.32e-32 (round-off error or half the machine epsilon) is
as follows:

  | (op)       | (op)double | error | (op)DDouble | error |
  |------------|-----------:|------:|------------:|------:|
  | add_small  |    3 flops |   2u² |    17 flops |   3u² |
  | + -        |   10 flops |   2u² |    20 flops |   3u² |
  | *          |    6 flops |   2u² |     9 flops |   4u² |
  | /          |   10 flops |   3u² |    31 flops |   6u² |
  | reciprocal |   14 flops | 2.3u² |    22 flops | 2.3u² |

The error bounds are tight analytical bounds [^2], except in the case of
double-double division, where the bound is 10u² but largest observed error
is 6u². We report the largest observed error here [^1].

[^1]: M. Joldes, et al., ACM Trans. Math. Softw. 44, 1-27 (2018)
[^2]: J.-M. Muller and L. Rideau, ACM Trans. Math. Softw. 48, 1, 9 (2022)


Installation
------------
libxprec has no mandatory dependencies other than a C++11-compliant compiler.
If you want to run the tests, you need to have GNU MPFR installed and add
`-DBUILD_TESTING=ON` to the cmake flags.

    mkdir build
    cd build
    cmake .. [-DBUILD_TESTING=ON] [EXTRA_CMAKE_FLAGS_GO_HERE]
    make
    ./test/tests

xprec is faster when using the fused-multiply add (FMA) instruction, which should
be available on most modern CPUs.  We recommend adding `CMAKE_CXX_FLAGS=-mfma`
to the cmake command.

License and Copying
-------------------
Copyright (C) 2023 Markus Wallerberger and others.

Released under the MIT license (see LICENSE for details).
