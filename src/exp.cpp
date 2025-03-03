/* Mathematical functions to quad precision.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 *
 * Most strategies are adapted from DoubleFloats.jl which is
 * Copyright (C) 2018-2023 Julia Math
 * and also licensed MIT
 */
#include "taylor.hpp"
#include "xprec/ddouble.hpp"
#include <cassert>

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

inline DDouble expm1_kernel_taylor(DDouble x, int nquad, int n)
{
    assert(std::fabs(x.hi()) < 1.0);
    DDouble xpow = x * x;
    DDouble r = x.add_small(PowerOfTwo(0.5) * xpow);
    int k = 3;
    for (; k <= nquad; ++k) {
        xpow *= x;
        r = r.add_small(reciprocal_factorial(k) * xpow);
    }

    // Here the terms are so small that they only affect the lo part, so
    // we can get away with double arithmetic.
    double xpow_d = xpow.hi();
    double r_d = 0;
    for (; k <= n; ++k) {
        xpow_d *= x.hi();
        r_d += reciprocal_factorial(k).hi() * xpow_d;
    }
    r = r.add_small(r_d);
    return r;
}

static DDouble expm1_alphas(int n)
{
    // For multiples of alpha = log(2)/128, precompute and store the
    // exponential function in a table, from -64*alpha until 63*alpha
    static const DDouble EXPM1_ALPHAS[128] = {
        {-0.2928932188134525, 7.174684663993261e-18},
        {-0.2890536989154172, -8.038914457945122e-18},
        {-0.285193330804015, -6.0158212445268276e-18},
        {-0.2813120012755088, -2.1020170082337783e-17},
        {-0.2774095965114767, -1.5118790674969937e-17},
        {-0.27348600207547374, 2.66114081842773e-17},
        {-0.26954110290967653, 2.7509265300881745e-17},
        {-0.265574783331509, -1.318173744858969e-17},
        {-0.2615869270302503, -1.741997278446398e-17},
        {-0.25757741706362375, -1.6107174092204261e-18},
        {-0.2535461358543676, 7.096460077142018e-18},
        {-0.24949296518678724, -4.31326076332226e-18},
        {-0.24541778620328863, 4.688384843543075e-18},
        {-0.24132047940089266, 6.212078255412209e-18},
        {-0.23720092462773085, 3.8644266954502085e-19},
        {-0.233059001079522, -1.1135017009065593e-17},
        {-0.2288945872960296, 1.199359843285919e-17},
        {-0.2247075611575, -7.300353295344693e-18},
        {-0.2204977998810815, -8.849540348841276e-18},
        {-0.21626518001722356, 3.750842387009219e-18},
        {-0.21200957744605675, -5.068458235639152e-18},
        {-0.20773086737375313, -9.668858517292851e-18},
        {-0.20342892432886656, 5.039118519698011e-18},
        {-0.19910362215865332, -2.5190116520100086e-18},
        {-0.19475483402537286, 1.2353596284898944e-17},
        {-0.19038243240256814, 1.0470667077114546e-17},
        {-0.1859862890713261, -5.809199807906506e-18},
        {-0.18156627511651777, 1.0736049740970466e-17},
        {-0.17712226092301758, 4.882751662883964e-18},
        {-0.1726541161719028, -7.294679715277685e-18},
        {-0.16816170983663178, 1.699387867936586e-18},
        {-0.1636449101792017, 3.719957926310978e-19},
        {-0.15910358474628547, 1.3239474487278572e-17},
        {-0.15453760036534742, 7.162793859283428e-18},
        {-0.14994682314073826, -4.01185968519885e-18},
        {-0.1453311184497686, 6.167253948093172e-18},
        {-0.14069035093876103, -9.256902091315555e-18},
        {-0.13602438451908122, 1.7562419252346148e-18},
        {-0.13133308236314686, -1.1933629119164127e-17},
        {-0.12661630690041553, 1.749698813720255e-18},
        {-0.12187391981335026, 9.229156694299104e-19},
        {-0.1171057820333636, 5.67321166697297e-18},
        {-0.11231175373673938, 4.393083367153945e-18},
        {-0.1074916943405325, -6.2125877472988e-18},
        {-0.1026454624984464, -4.7640585938584126e-18},
        {-0.09777291609668806, 1.869463571662324e-18},
        {-0.09287391224980063, 5.66349353665608e-18},
        {-0.08794830729647335, 4.713011919872412e-18},
        {-0.08299595679532877, 2.537748313413679e-18},
        {-0.07801671552068704, -1.94313451912091e-18},
        {-0.07301043745830721, -6.701713777619857e-18},
        {-0.06797697580110548, 4.948987787473942e-18},
        {-0.06291618294485005, -2.8582414493917966e-18},
        {-0.057827910483832776, 5.00397795774813e-19},
        {-0.05271200920651718, 3.1392298682681924e-18},
        {-0.047568329091162896, -2.025181945944751e-18},
        {-0.042396719301426355, 2.4114209502780123e-18},
        {-0.037197028181937535, -1.0025615211181075e-18},
        {-0.03196910325385278, 3.089672476031033e-18},
        {-0.026712791210383356, -6.393577718667539e-19},
        {-0.021427937912299865, -2.989714202136461e-19},
        {-0.01611438838341211, 4.670642216485574e-19},
        {-0.010771986806024515, -6.223051570826017e-19},
        {-0.005400576516366824, -2.342423707574178e-19},
        {0.0, 0.0},
        {0.005429901112802822, -4.1792582417406993e-19},
        {0.01088928605170046, 3.7773268042268547e-19},
        {0.016378314910953037, 1.2588974512148405e-18},
        {0.02189714865411668, -9.494539895697731e-19},
        {0.027445949118763698, -9.884844191031042e-19},
        {0.03302487902122842, 6.619449701198605e-19},
        {0.03863410196137879, -2.487307246639953e-18},
        {0.04427378242741384, 2.252170208492904e-18},
        {0.049944085800687266, 4.182272500122047e-19},
        {0.05564517836055716, 1.759325738772092e-18},
        {0.06137722728926208, 1.9042507224487988e-18},
        {0.06714040067682361, 4.268187178470922e-18},
        {0.07293486752597556, -3.839668843358824e-18},
        {0.07876079775711979, 2.8223346785063543e-18},
        {0.08461836221330923, 3.905952842534547e-18},
        {0.09050773266525766, -2.712245182495796e-18},
        {0.09642908181637683, -3.6881836132353304e-18},
        {0.10238258330784095, -2.8507825155508824e-18},
        {0.10836841172367864, -4.601411604918528e-18},
        {0.11438674259589254, -6.919517894059943e-18},
        {0.12043775240960669, -6.499707834283954e-18},
        {0.1265216186082419, -3.8525836433032604e-18},
        {0.13263851959871922, 4.617986051751087e-18},
        {0.13878863475669165, 5.861399913367335e-18},
        {0.14497214443180423, -9.09825230955772e-18},
        {0.1511892299529827, 4.751526573009359e-18},
        {0.15744007363375104, -7.971985464457258e-18},
        {0.1637248587775775, 1.0536472753612021e-17},
        {0.1700437696832502, -1.8477442017900047e-18},
        {0.17639699165028128, 3.088131092296112e-20},
        {0.18278471098434104, -1.2325821314838153e-17},
        {0.18920711500272105, 1.2064576699027549e-17},
        {0.19566439203982738, -9.345114526443012e-18},
        {0.20215673145270313, 1.0938663761265181e-17},
        {0.20868432362658157, 8.043891778967983e-18},
        {0.21524735998046887, 6.140419920071864e-18},
        {0.2218460329727575, 4.912090348488744e-18},
        {0.22848053610687, 8.767759302603614e-18},
        {0.2351510639369333, 3.469859019437239e-18},
        {0.24185781207348406, -8.930875312888462e-18},
        {0.24860097718920474, 6.4861685666710185e-19},
        {0.2553807570246911, -6.7113898212968784e-18},
        {0.2621973503942507, 2.4666502356519365e-17},
        {0.2690509571917332, 2.667932131342186e-18},
        {0.2759417783963921, -1.1868000020372746e-17},
        {0.28287001607877826, 1.713594918243561e-17},
        {0.28983587340666583, -2.1529727153539737e-17},
        {0.29683955465100964, 2.5382502794888315e-17},
        {0.3038812651919359, -2.4545546479836942e-17},
        {0.31096121152476436, -1.6304210123936712e-17},
        {0.318079601266064, 9.315929597662924e-19},
        {0.32523664315974127, 2.6923839130869213e-17},
        {0.33243254708316144, 4.495284922090389e-18},
        {0.339667524053303, -2.1749476514198334e-17},
        {0.34694178623294586, -2.3270500218711038e-17},
        {0.3542555469368927, 2.1498332566772065e-17},
        {0.36160902063822475, 1.533787661270668e-18},
        {0.3690024229745906, -1.5084323271327172e-17},
        {0.3764359707545301, -1.3474738127460185e-17},
        {0.38390988196383197, -1.2193965356690036e-17},
        {0.3914243757719262, 6.4494025783679345e-18},
        {0.3989796725383111, 1.4880170372002426e-17},
        {0.40657599381901544, 7.034914812136422e-18},
        };

    assert(n >= -64 && n < 64);
    return EXPM1_ALPHAS[n + 64];
}

static DDouble reduce_mod_alpha(DDouble x, double &n)
{
    // ALPHA_T is an approximation of log(2)/128 to 90 significant bits -- 17
    // bits fewer than full double-double precision.  Observe then that
    // 128*log(DBL_MAX) is around 91000, which fit comfortably into 17 bits.
    // That means that the reduction of x modulo ALPHA_T:
    //
    //     x = n * ALPHA_T + z
    //
    // is *exact* for any x in the range of the exponential funcion.  We have
    // to correct this expression to at least 124 digits. The correction term
    // only needs to be in double precision
    //
    //     z = n * ALPHA_CORR + y
    //
    static const double INV_ALPHA = 184.6649652337873;
    static const DDouble ALPHA_T(0.0054152123481245725, 1.8117553232937405e-19);
    static const double ALPHA_CORR = 2.3681038446414578e-30;

    n = std::rint(x.hi() * INV_ALPHA);
    DDouble z = x.add_small(n * -ALPHA_T);
    return z.add_small(n * -ALPHA_CORR);
}

static int reduce_mod_128(int k, int &m)
{
    m = k >> 7;
    int n = k & 0x7F;
    if (k & 0x40) {
        n -= 0x80;
        m += 1;
    }
    return n;
}

XPREC_API_EXPORT
DDouble exp(DDouble x)
{
    // Here is the main strategy. Let α be log(2)/128. Then we first reduce the
    // argument x modulo α, i.e.:
    //
    //     x = k * α + y
    //
    double k;
    DDouble y = reduce_mod_alpha(x, k);

    // Now we perform checks for special values. Using not <= instead of >
    // also catches NaNs.
    const double MAX_ALPHA = 128 * 1024;
    if (!(std::fabs(k) <= MAX_ALPHA)) {
        if (isnan(x))
            return x;
        else if (x.hi() > 0)
            return INFINITY;
        else
            return 0;
    }

    // We further split k = 128 * m + n, where `n` is between {0, ..., 127}
    // Then we have that:
    //
    //     exp(x) = ldexp(1, m) * exp(n * ALPHA) * exp(y)
    //
    int m;
    int n = reduce_mod_128((int) k, m);

    PowerOfTwo exp_m = std::ldexp(1.0, m);
    DDouble exp_n = ExDouble(1.0).add_small(expm1_alphas(n));
    DDouble exp_y = ExDouble(1.0).add_small(expm1_kernel_taylor(y, 6, 10));
    return exp_m * exp_n * exp_y;
}

static DDouble expm1_small(DDouble x)
{
    // Reduce mod α = log(2)/128 similar to exp:
    //
    //     x = k * α + y = n * α + y
    //
    double k;
    DDouble y = reduce_mod_alpha(x, k);

    int m;
    int n = reduce_mod_128((int) k, m);
    assert(m == 0);

    // The idea is to use the identity
    //
    //     expm1(x) = expm1(n * α) + exp(n * α) * expm1(y)
    //
    // to reduce the expansion order.
    DDouble expm1_n = expm1_alphas(n);
    DDouble exp_n = ExDouble(1.0).add_small(expm1_n);
    DDouble expm1_y = expm1_kernel_taylor(y, 6, 10);
    return expm1_n.add_small(expm1_y * exp_n);
}

XPREC_API_EXPORT
DDouble expm1(DDouble x)
{
    // For small values, we call the expm1 kernel directly
    if (std::fabs(x.hi()) < 0.34657359)
        return expm1_small(x);

    // Otherwise, we do a naive computation
    DDouble res = exp(x);
    if (x.hi() < 75)
        res -= 1.0;
    return res;
}

XPREC_API_EXPORT
DDouble log(DDouble x)
{
    // Start with logarithm of hi part
    DDouble log_x = std::log(x.hi());
    if (!isfinite(log_x))
        return log_x;

    // Abramowitz and Stegun give the following series expansion (4.1.30):
    //
    //   log(x) = log(x0) + 2 (x - x0)/(x + x0) + O(x - x0)^3
    //
    DDouble x0 = exp(log_x);
    DDouble corr = PowerOfTwo(2.0) * x.add_small(-x0) / x.add_small(x0);
    log_x += corr;
    return log_x;
}

XPREC_API_EXPORT
DDouble log1p(DDouble x)
{
    // Start with logarithm of hi part
    DDouble log_x = std::log1p(x.hi());
    if (!isfinite(log_x))
        return log_x;

    // Again, we can use the same correction, but log1p <-> expm1
    //
    //   log(1 + x) = log(1 + x0) + 2 (x - x0)/(2 + x + x0) + O(x - x0)^3
    //
    // One need not worry about cancellation in the denominator for
    // x close to -1, since that is where we have an intrinsic loss of
    // precision anyway
    DDouble x0 = expm1(log_x);
    DDouble corr = PowerOfTwo(2.0) * x.add_small(-x0) / (2.0 + x).add_small(x0);
    log_x += corr;
    return log_x;
}

XPREC_API_EXPORT
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

XPREC_API_EXPORT
DDouble pow(DDouble x, DDouble y) { return exp(log(x) * y); }

} // namespace xprec
