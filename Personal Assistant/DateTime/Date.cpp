/*
 * Data.cpp
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#include "Date.h"
#include <iostream>
#include <iomanip>
#include <cassert>
#include <stdexcept>
#include <sstream>

#define INVALID_WARNING(var, _default) if (_print_warning)\
    std::cout << "Date warning: "\
    << #var << " " << var << " invalid. Set to default: "\
    << _default << "." << std::endl

char Date::_separator = '-';
bool Date::_print_warning = true;

Date::Date(int year, int month, int day) {
    setdate(year, month, day);
}

// use the stream to read data, include printing the warning message
//Date::Date(const std::string & s) {
//    // for not warning me about the un-set data member
//    // though I guarnteen them to be set in operator>>
//    _year = DEFAULT_YEAR;
//    _month = DEFAULT_MONTH;
//    _day = DEFAULT_DAY;
//
//    std::stringstream stream_;
//    stream_ << s;
//    stream_ >> (*this);
//}

Date::Date(const Date & other) {
    *this = other;
}

void Date::setdate(int year, int month, int day) {
    setyear(year);
    setmonth(month);
    setday(day);
}

void Date::setyear(int year) {
    if (year == _UNCHANGE)
        return;
    if (year < 0 || year > 9999) {
        INVALID_WARNING(year, DEFAULT_YEAR);
        _year = DEFAULT_YEAR;
    } else {
        _year = year;
    }
}

void Date::setmonth(int month) {
    if (month == _UNCHANGE)
        return;
    if (month < 1 || month > 12) {
        INVALID_WARNING(month, DEFAULT_MONTH);
        _month = DEFAULT_MONTH;
    } else {
        _month = month;
    }
}

void Date::setday(int day) {
    if (day == _UNCHANGE)
        return;
    if (day < 1 || day > maxday(_year, _month)) {
        INVALID_WARNING(day, DEFAULT_DAY);
        _day = DEFAULT_DAY;
    } else {
        _day = day;
    }
}

int Date::getyear() const {
    return _year;
}

int Date::getmonth() const {
    return _month;
}

int Date::getday() const {
    return _day;
}

void Date::separator(char c) {
    _separator = c;
}

char Date::separator() const {
    return _separator;
}

void Date::printwarning(bool setting) {
    _print_warning = setting;
}

bool Date::printwarning() const {
    return _print_warning;
}

bool operator==(const Date & ref1, const Date & ref2) {
    return ref1._year == ref2._year && ref1._month == ref2._month
            && ref1._day == ref2._day;
}

bool operator<(const Date & ref1, const Date & ref2) {
    if (ref1._year < ref2._year)
        return true;
    if (ref1._year > ref2._year)
        return false;
    else if (ref1._month < ref2._month)
        return true;
    else if (ref1._month > ref2._month)
        return false;
    else if (ref1._day < ref2._day)
        return true;
    else
        return false;
}

bool operator<=(const Date & ref1, const Date & ref2) {
    return ref1 < ref2 || ref1 == ref2;
}

bool operator>(const Date & ref1, const Date & ref2) {
    return !(ref1 <= ref2);
}

bool operator>=(const Date & ref1, const Date & ref2) {
    return !(ref1 < ref2);
}

int Date::maxday(int year, int month) throw (std::invalid_argument) {
    switch (month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            return leapyear(year) ? 29 : 28;
        default:
            throw std::invalid_argument(
                    "Date::maxday(): The month is out of 1-12");
    }
    return 0;
}

bool Date::leapyear(int year) {
    if (year % 400 == 0)
        return true;
    else if (year % 100 == 0)
        return false;
    else if (year % 4 == 0)
        return true;
    else
        return false;
}

#undef INVALID_WARNING
