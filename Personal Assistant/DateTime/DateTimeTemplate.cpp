/*
 * DateTimeTemplate.cpp
 *
 *  Created on: 2015Äê6ÔÂ6ÈÕ
 *      Author: Sine
 */

#ifndef DATETIMETEMPLATE_CPP_
#define DATETIMETEMPLATE_CPP_

#include "DateTime.h"
#include <sstream>

template<class stream>
stream &operator>>(stream & s, DateTime & ref) {
    // fixme I don't know whether this is the right way...
    s >> static_cast<Date &>(ref);
    s.ignore();
    s >> static_cast<Time &>(ref);
    return s;
}

// transform to one string first, for the outer to use such as setwidth()...
template<class stream>
stream &operator<<(stream & s, const DateTime & ref) {
    std::stringstream stream_;
    stream_ << static_cast<Date>(ref) << static_cast<Time>(ref);
    std::string output;
    stream_ >> output;
    s << output;
    return s;
}

#endif
