/*
 * Time.cpp
 *
 *  Created on: 2015Äê6ÔÂ6ÈÕ
 *      Author: Sine
 */

#include "Time.h"
#include <sstream>

bool Time::_use_second = false;

Time::Time(int hour, int minute, int second) {
    settime(hour, minute, second);
}

//Time::Time(const std::string & s) {
//    set(0, 0, 0);
//    std::stringstream stream_;
//    stream_ << s;
//    stream_ >> (*this);
//}

Time::Time(const Time & other) {
    *this = other;
}

void Time::settime(int hour, int minute, int second) {
    sethour(hour);
    setminute(minute);
    setsecond(second);
}

void Time::sethour(int hour) {
    if (hour < 0 || hour > 23)
        _hour = 0;
    else
        _hour = hour;
}

void Time::setminute(int minute) {
    if (minute < 0 || minute > 59)
        _minute = 0;
    else
        _minute = minute;
}

void Time::setsecond(int second) {
    if (second < 0 || second > 59)
        _second = 0;
    else
        _second = second;
}

//void Time::set_use_second(bool i) {
//    _use_second = i;
//}

int Time::gethour() {
    return _hour;
}

int Time::getminute() {
    return _minute;
}

int Time::getsecond() {
    return _second;
}

//bool Time::set_use_second() {
//    return _use_second;
//}

bool operator==(const Time & ref1, const Time & ref2) {
    if (ref1._hour != ref2._hour)
        return false;
    else if (ref1._minute != ref2._minute)
        return false;
    else if (Time::_use_second)
        if (ref1._second != ref2._second)
            return false;
        else
            return true;
    else
        return true;
}

bool operator<(const Time & ref1, const Time & ref2) {
    if (ref1._hour < ref2._hour)
        return true;
    if (ref1._hour > ref2._hour)
        return false;
    else if (ref1._minute < ref2._minute)
        return true;
    else if (ref1._minute > ref2._minute)
        return false;
    else if (Time::_use_second)
        if (ref1._second < ref2._second)
            return true;
        else
            return false;
    else
        return false;
}

bool operator<=(const Time & ref1, const Time & ref2) {
    return ref1 < ref2 || ref1 == ref2;
}

bool operator>(const Time & ref1, const Time & ref2) {
    return !(ref1 <= ref2);
}

bool operator>=(const Time & ref1, const Time & ref2) {
    return !(ref1 < ref2);
}

