/*
 * Task.cpp
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#include "Task.h"
#include <iostream>

Task::Task(const Date & date, const std::string & theme,
           const std::string & detail)
        : Affair(date, theme, detail) {
}

Task::Task(const Affair & affair)
        : Affair(affair) {
}

void Task::print() const {
    std::cout << "Task  ";
    Affair::print();
}

inline const std::string Task::type() const {
    return "Task";
}
