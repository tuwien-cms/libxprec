/* Mathematical functions to quad precision.
 *
 * Copyright (C) 2022 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "ddouble.h"
#include <cstring>

DDouble sqrt(DDouble a)
{
    // From: Karp, High Precision Division and Square Root, 1993
    double sqrt_hi = sqrt(a.hi());
    if (a.hi() <= 0)
        return sqrt_hi;

    double x = 1.0 / sqrt_hi;
    ExDouble ax = a.hi() * x;
    DDouble ax_squared = ax * ax;
    double diff = (a - ax_squared).hi() * x * 0.5;
    return ax + diff;
}

DDouble hypot(DDouble x, DDouble y)
{
    // Make sure that the values are ordered by magnitude
    if (!greater_in_magnitude(x, y)) {
        swap(x, y);
    }

    // Check for infinities
    if (!std::isfinite(x.hi())) {
        return std::isnan(y.hi()) ? y : x;
    }

    // Splits the range in half
    static const PowerOfTwo LARGE(std::numeric_limits<double>::max_exponent / 2);
    static const PowerOfTwo SMALL  = PowerOfTwo(0) / LARGE;

    if (greater_in_magnitude(x.hi(), LARGE)) {
        // For large values, scale down to avoid overflow
        x *= SMALL;
        y *= SMALL;
        return sqrt((x * x).add_small(y * y)) * LARGE;
    } else if (greater_in_magnitude(SMALL, x.hi())) {
        // For small values, scale up to avoid underflow
        x *= LARGE;
        y *= LARGE;
        return sqrt((x * x).add_small(y * y)) * SMALL;
    } else {
        // We're fine
        return sqrt((x * x).add_small(y * y));
    }
}

DDouble pow(DDouble x, int n)
{
    if (n < 0) {
        DDouble res = pow(x, -n);
        return reciprocal(res);
    }
    if (n == 0) {
        // XXX handle nan's etc.
        return DDouble(1.0);
    }

    // Get first non-zero power
    while ((n & 1) == 0) {
        n >>= 1;
        x *= x;
    }

    // Multiply and square
    DDouble res = x;
    while (n >>= 1) {
        x *= x;
        if ((n & 1) == 1)
            res *= x;
    }
    return res;
}

static DDouble expm1_kernel(DDouble x)
{
    // We need to make sure that (1 + x) does not lose possible significant
    // digits, so no matter what strategy we choose here, the convergence
    // needs to go out to x = log(1.5) = 0.22

    // Convergence of the CF approx to 2e-32
    assert(greater_in_magnitude(0.3, x.hi()));

    // Continued fraction expansion of the exponential function
    //  6*div + div_d + 6*add_d + add_sm + mul_p = 253 flops
    DDouble xsq = x * x;
    DDouble r;
    r = xsq / 34.0 + 30.0;
    r = xsq / r + 26.0;
    r = xsq / r + 22.0;
    r = xsq / r + 18.0;
    r = xsq / r + 14.0;
    r = xsq / r + 10.0;
    r = xsq / r + 6.0;
    r = (-x).add_small(xsq / r) + 2.0;
    r = x / r;
    r *= PowerOfTwo(1);
    return r;
}

static DDouble exp_halves(int x)
{
    const static DDouble EXP_HALVES[31] = {
        {1.6487212707001282,-4.731568479435833e-17},
        {2.718281828459045,1.4456468917292502e-16},
        {4.4816890703380645,3.0481759556536343e-16},
        {7.38905609893065,-1.7971139497839148e-16},
        {12.182493960703473,2.0334002173348147e-16},
        {20.085536923187668,-1.8275625525512858e-16},
        {33.11545195869231,2.2435601403927554e-15},
        {54.598150033144236,2.8741578015844115e-15},
        {90.01713130052181,2.550844346114049e-15},
        {148.4131591025766,3.4863514900464198e-15},
        {244.69193226422038,4.129320187450839e-15},
        {403.4287934927351,1.2359628024450387e-14},
        {665.1416330443618,2.990469256473133e-14},
        {1096.6331584284585,9.869752640434095e-14},
        {1808.0424144560632,3.6612201665204784e-14},
        {2980.9579870417283,-2.7103295816873633e-14},
        {4914.768840299134,2.17317454126359e-14},
        {8103.083927575384,-2.1530877621067177e-13},
        {13359.726829661873,-8.496858340658619e-13},
        {22026.465794806718,-1.3780134700517372e-12},
        {36315.502674246636,1.577797006387782e-12},
        {59874.14171519782,1.7895764888916994e-12},
        {98715.7710107605,3.036676373480473e-12},
        {162754.79141900392,5.30065881322063e-12},
        {268337.2865208745,-2.0035114163950887e-11},
        {442413.3920089205,1.2118711752313224e-11},
        {729416.3698477013,5.1483277361034595e-11},
        {1.2026042841647768e6,-1.5000525764327354e-11},
        {1.9827592635375687e6,2.845770459793355e-11},
        {3.2690173724721107e6,-3.075806431120808e-11},
        {5.389698476283012e6,4.098121666636582e-10}
        };
    const static DDouble EXP_SIXTEENS[44] = {
        {8.886110520507872e6,5.321182483501564e-10},
        {7.896296018268069e13,0.007660978022635108},
        {7.016735912097631e20,30185.471599886117},
        {6.235149080811617e27,1.3899738872492847e11},
        {5.54062238439351e34,2.1811937023229343e18},
        {4.923458286012058e41,1.3869835129739753e25},
        {4.375039447261341e48,1.035824156236645e32},
        {3.887708405994595e55,2.707966110366217e39},
        {3.454660656717546e62,1.8553902103629043e46},
        {3.0698496406442424e69,4.375620509828095e52},
        {2.7279023188106115e76,6.6492459414351406e59},
        {2.4240441494100796e83,-3.8332753349400205e66},
        {2.1540324218248465e90,6.568050851363196e73},
        {1.9140970165092822e97,-1.497464557916617e81},
        {1.700887763567586e104,1.4773861394382237e88},
        {1.5114276650041035e111,1.4805989167614457e94},
        {1.3430713274979614e118,-6.561438244448466e101},
        {1.1934680253072109e125,-3.301231394418859e108},
        {1.0605288775572162e132,5.4744408887427266e115},
        {9.423976816163585e138,-2.7555072985830676e122},
        {8.374249953113352e145,-3.529195534423469e129},
        {7.441451060972311e152,4.251237045552673e136},
        {6.612555656075053e159,-3.4828210031110127e143},
        {5.875990038289236e166,7.682543674132907e149},
        {5.221469689764144e173,-3.041154182825333e157},
        {4.639855674272614e180,-3.3453058659461497e164},
        {4.123027032079202e187,1.8602059512155307e171},
        {3.663767388609735e194,-1.8555200045340274e178},
        {3.255664193661862e201,5.148254191579011e184},
        {2.8930191842539453e208,-2.8880381060655904e191},
        {2.5707688209230085e215,1.1853726094570251e199},
        {2.2844135865397565e222,1.3549224944023444e206},
        {2.0299551604542052e229,1.2942147572086164e213},
        {1.803840590747136e236,1.820681001928355e218},
        {1.6029126850757262e243,-2.463627227554342e226},
        {1.4243659274306933e250,-5.204358467973364e233},
        {1.2657073052794837e257,-3.983584155610672e240},
        {1.124721500132769e264,-8.843155706148207e247},
        {9.994399554971195e270,8.925025806205413e253},
        {8.881133903158874e277,-4.948247489077345e261},
        {7.891873741089921e284,2.4630459641303726e268},
        {7.012806227721897e291,-1.1759583274063904e275},
        {6.231657119844268e298,1.1619020533730335e281},
        {5.5375193892845935e305,1.5239358093004245e289}
        };

    if (x < 0) {
        return reciprocal(exp_halves(-x));
    }
    assert(x <= 1439);
    int x_halves = x % 32;
    int x_sixteens = x / 32;

    if (!x_halves && !x_sixteens)
        return 1;
    else if (x_halves && !x_sixteens)
        return EXP_HALVES[x_halves-1];
    else if (!x_halves && x_sixteens)
        return EXP_SIXTEENS[x_sixteens-1];
    else
        return EXP_HALVES[x_halves-1] * EXP_SIXTEENS[x_sixteens-1];
}

DDouble exp(DDouble x)
{
    // x = y/2 + z
    double y = round(2 * x.hi());
    DDouble z = x - y/2;

    // exp(z + y/4) = (1 + expm1(z)) exp(1/4)^y
    DDouble exp_z = 1.0 + expm1_kernel(z);
    DDouble exp_y = exp_halves(int(y));
    return exp_z * exp_y;
}
