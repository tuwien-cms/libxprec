/* Input/Output functions.
 *
 * Copyright (C) 2023 Markus Wallerberger and others
 * SPDX-License-Identifier: MIT
 */
#include "xprec/ddouble.h"
#include <array>
#include <iostream>
#include <sstream>

#ifndef XPREC_API_EXPORT
#define XPREC_API_EXPORT
#endif

namespace xprec {

class FormatSentry {
public:
    explicit FormatSentry(std::ostream &str) : stream_(str), saved_(nullptr)
    {
        saved_.copyfmt(stream_);
    }

    ~FormatSentry() { stream_.copyfmt(saved_); }

    operator bool() const { return stream_.good(); }

private:
    // make it non-copyable and -movable
    FormatSentry(const FormatSentry &) = delete;
    FormatSentry &operator=(const FormatSentry &) = delete;

    std::ostream &stream_;
    std::ios saved_;
};

XPREC_API_EXPORT
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

XPREC_API_EXPORT
std::ostream &operator<<(std::ostream &out, DDouble x)
{
    // XXX this needs some work
    dump(out, x);

    // Return stream
    return out;
}

XPREC_API_EXPORT
std::string to_string(DDouble d, size_t nDigits){
	using namespace std;

	//Get some basic cases out of the way.
	if(d.hi() == 0.0)
		return "0.0";

	if(isnan(d.hi()))
		return "NaN";

	if(isinf(d.hi()))
		if(d.hi() > 0.0)
			return "Inf";
		else
			return "-Inf";


	//Alright. Get a string started. Handle sign.
	string s;

	if(d.hi() < 0.0) s += "-";
	d = abs(d);


	//Get the initial tens place and reduce d by it. This way, d becomes digit.nextdigits.
	auto n = (int)floor(log10(d.hi()));
	if(n < 0) n++;

	constexpr DDouble Ten = 10.0;
	d /= pow(Ten, n);

	//Loop over digits, print them.
	nDigits = min(max(nDigits, (size_t)3), (size_t)34);

	for(size_t i = 0; i != nDigits; i++){
		if(i == 1) s += ".";

		//Current digit to print is just the leading one.
		auto m = (int)floor(d.hi());
		assert(m >= 0 && m < 10);

		s += std::to_string(m);

		//Remove leading digit, promote by ten to retain the digit.nextdigits form.
		d = (d - DDouble(m))*Ten;

		//Emptied it - nothing more to print.
		if(d.hi() == 0.0)
			break;

		assert(d.hi() > 0.0);
	}

	//Assuming you care more about the mantissa... always use exponential form. n == 0 imples e0.
	if(n != 0)
		s += "e" + to_string(n);

	return s;
}

} /* namespace xprec */
