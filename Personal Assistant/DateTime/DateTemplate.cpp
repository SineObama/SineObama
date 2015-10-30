/*
 * DateTemplate.cpp
 *
 *  Created on: 2015Äê6ÔÂ3ÈÕ
 *      Author: Sine
 */

// ...have nothing to do about the file type.
// It scared me that template function can be defined not only once
// among different files
// ...though their do can only be instantiated in file scope when needed
// so this is just included in the Date.h
// for not showing the definition ? encapsulation ?
#ifndef DATETEMPLATE_CPP_
#define DATETEMPLATE_CPP_
// above can be delete ? no !

#include <iostream>
#include <sstream>
#include <string>
#include "Date.h"  // this kind of include seem strange...

#define FAIL_WARNING(var, _default) if (Date::_print_warning)\
    std::cout << "Date warning: Read the "\
    << #var << " failed. Set to default: " << _default << "." << std::endl

template<class stream>
stream &operator>>(stream & s, Date & ref) {
    int year = DEFAULT_YEAR, month = DEFAULT_MONTH, day = DEFAULT_DAY;
    s >> year;
    if (s.fail()) {
        FAIL_WARNING(year, DEFAULT_YEAR);
        year = DEFAULT_YEAR;
        s.clear();
    }
    s.ignore();
    s >> month;
    if (s.fail()) {
        FAIL_WARNING(month, DEFAULT_MONTH);
        month = DEFAULT_MONTH;
        s.clear();
    }
    s.ignore();
    s >> day;
    if (s.fail()) {
        FAIL_WARNING(day, DEFAULT_DAY);
        day = DEFAULT_DAY;
        s.clear();
    }
    ref.setdate(year, month, day);
    return s;
}

// transform to one string first, for the outer to use such as setwidth()...
template<class stream>
stream &operator<<(stream & s, const Date & ref) {
    std::stringstream stream_;
    stream_ << ref._year << Date::_separator << (ref._month < 10 ? "0" : "")
            << ref._month << Date::_separator << (ref._day < 10 ? "0" : "")
            << ref._day;
    std::string output;
    stream_ >> output;
    s << output;
    return s;
}

#undef FAIL_WARNING

#endif
