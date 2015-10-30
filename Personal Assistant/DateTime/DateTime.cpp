/*
 * DateTime.cpp
 *
 *  Created on: 2015Äê6ÔÂ6ÈÕ
 *      Author: Sine
 */

#include "DateTime.h"

DateTime::DateTime(int year, int month, int day, int hour, int minute,
                   int second)
        : Date(year, month, day),
          Time(hour, minute, second) {

}

DateTime::DateTime(const DateTime & other) {
    *this = other;
}
