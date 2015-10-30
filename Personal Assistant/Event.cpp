/*
 * Event.cpp
 *
 *  Created on: 2015��5��29��
 *      Author: Sine
 */

#include "Event.h"
#include <iostream>

Event::Event(const Date & date, const std::string & theme,
             const std::string & detail)
        : Affair(date, theme, detail) {
}

Event::Event(const Affair & affair)
        : Affair(affair) {
}

void Event::print() const {
    std::cout << "Event ";
    Affair::print();
}

inline const std::string Event::type() const {
    return "Event";
}
