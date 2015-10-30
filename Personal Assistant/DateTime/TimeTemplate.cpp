/*
 * TimeTemplate.cpp
 *
 *  Created on: 2015Äê6ÔÂ6ÈÕ
 *      Author: Sine
 */

#ifndef TIMETEMPLATE_CPP_
#define TIMETEMPLATE_CPP_

#include "Time.h"
#include <sstream>

template<class stream>
stream &operator>>(stream & s, Time & ref) {
    int hour = 0, minute = 0, second = 0;
    s >> hour;
    if (s.fail()) {
        hour = 0;
        s.clear();
    }
    s.ignore();
    s >> minute;
    if (s.fail()) {
        minute = 0;
        s.clear();
    }
    if (Time::_use_second) {
        s.ignore();
        s >> second;
        if (s.fail()) {
            second = 0;
            s.clear();
        }
    }
    ref.settime(hour, minute, second);
    return s;
}

// transform to one string first, for the outer to use such as setwidth()...
template<class stream>
stream &operator<<(stream & s, const Time & ref) {
    std::stringstream stream_;
    stream_ << (ref._hour < 10 ? "0" : "") << ref._hour << ':'
            << (ref._minute < 10 ? "0" : "") << ref._minute;
    if (Time::_use_second)
        stream_ << ':' << (ref._second < 10 ? "0" : "") << ref._second;
    std::string output;
    stream_ >> output;
    s << output;
    return s;
}

#endif
