/* Input/Output functions.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include <array>
#include <iostream>
#include <sstream>

class FormatSentry
{
public:
    explicit FormatSentry(std::ostream &str)
        : stream_(str)
        , saved_(nullptr)
    {
        saved_.copyfmt(stream_);
    }

    ~FormatSentry() { stream_.copyfmt(saved_); }

    _XPREC_INLINE_IF_HEADER_ONLY
    operator bool () const { return stream_.good(); }

private:
    // make it non-copyable and -movable
    FormatSentry(const FormatSentry &) = delete;
    FormatSentry &operator=(const FormatSentry &) = delete;

    std::ostream &stream_;
    std::ios saved_;
};

_XPREC_INLINE_IF_HEADER_ONLY
std::ostream &dump(std::ostream &out, DDouble x)
{
    FormatSentry s(out);
    if (!s)
        return out;

    out.width(0);
    out.precision(16);
    out.setf(std::ios_base::scientific);
    out << "DDouble(" << x.hi() << "," << x.lo() << ")";
    return out;
}

_XPREC_INLINE_IF_HEADER_ONLY
std::ostream &operator<<(std::ostream &out, DDouble x)
{
    // XXX this needs some work
    dump(out, x);

    // Return stream
    return out;
}
