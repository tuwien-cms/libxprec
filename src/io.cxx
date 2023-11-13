#include "ddouble.h"
#include <array>
#include <sstream>

void dump(std::ostream &out, DDouble x)
{
    std::ostream::sentry s(out);
    if (!s)
        return;

    out << "DDouble("
        << std::to_string(x.hi()) << "," << std::to_string(x.lo())
        << ")";
}

std::ostream &operator<<(std::ostream &out, DDouble x)
{
    std::ostream::sentry s(out);
    if (!s)
        return out;

    // XXX this needs some work
    dump(out, x);

    // Return stream
    return out;
}
