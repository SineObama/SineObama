/*
 * Affair.cpp
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#include "Affair.h"
#include <iostream>
#include <iomanip>
#include <string>

Affair::Affair()
        : _done(false) {
}

Affair::Affair(const Date & date, const std::string & theme,
               const std::string & detail)
        : _date(date),
          _theme(theme),
          _detail(detail),
          _done(false) {
}

void Affair::set_date(const Date & date) {
    _date = date;
}

void Affair::set_date(int year, int month, int day) {
    _date.setdate(year, month, day);
}

void Affair::set_theme(const std::string & theme) {
    _theme = theme;
}

void Affair::set_detail(const std::string & detail) {
    _detail = detail;
}

const Date &Affair::get_date() const {
    return _date;
}

const std::string &Affair::get_theme() const {
    return _theme;
}

const std::string &Affair::get_detail() const {
    return _detail;
}

void Affair::mark_done() {
    _done = true;
}

void Affair::mark_todo() {
    _done = false;
}

bool Affair::done() const {
    return _done;
}

void Affair::print() const {
    std::cout << std::setw(10) << _date << "  \004 " << _theme << "  \004 "
              << _detail;
}

const std::string Affair::type() const {
    return "Affair";
}

Affair::~Affair() {
}
