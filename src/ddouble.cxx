#include "ddouble.h"
#include <cstring>

union double_pattern {
    double number;
    uint64_t pattern;
};

/**
 * Return true if x is greater or equal in magnitude as y.
 *
 * Return true if the xponent of y does not exceed the exponent of x.  NaN
 * and Inf are considered maximum magnitude, 0 is considered minimum magnitude.
 */
static bool greater_in_magnitude(double x, double y)
{
    if(std::numeric_limits<double>::is_iec559) {
        static const uint64_t exp_mask = 0x7ff0000000000000UL;
        double_pattern x_u = {x}, y_u = {y};
        return (x_u.pattern & exp_mask) >= (y_u.pattern & exp_mask);
    } else {
        int x_exp, y_exp;
        std::frexp(x, &x_exp);
        std::frexp(y, &y_exp);
        return x_exp >= y_exp;
    }
}

static bool greater_in_magnitude(DDouble x, DDouble y)
{
    return greater_in_magnitude(x.hi(), y.hi());
}

DDouble sqrt(DDouble a)
{
    // From: Karp, High Precision Division and Square Root, 1993
    double sqrt_hi = sqrt(a.hi());
    if (a.hi() <= 0)
        return sqrt_hi;

    double x = 1.0 / sqrt_hi;
    double ax = a.hi() * x;
    DDouble ax_squared = DDouble::product(ax, ax);
    double diff = (a - ax_squared).hi() * x * 0.5;
    return DDouble::sum(ax, diff);
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
        return sqrt(x * x + y * y) * LARGE;
    } else if (greater_in_magnitude(SMALL, x.hi())) {
        // For small values, scale up to avoid underflow
        x *= LARGE;
        y *= LARGE;
        return sqrt(x * x + y * y) * SMALL;
    } else {
        // We're fine
        return sqrt(x * x + y * y);
    }
}

DDouble exp(DDouble x)
{
    // X
    return 0;
}
