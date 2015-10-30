/*
 * Affair.h
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#ifndef AFFAIR_H_
#define AFFAIR_H_

#include "Date.h"
#include <string>

class Affair {

 public:

    Affair();
    Affair(const Date & date, const std::string & theme,
           const std::string & detail);

    void set_date(const Date & ref);
    void set_date(int year, int month, int day);
    void set_theme(const std::string & theme);
    void set_detail(const std::string & detail);

    const Date &get_date() const;
    const std::string &get_theme() const;
    const std::string &get_detail() const;

    void mark_done();
    void mark_todo();
    bool done() const;

    virtual void print() const;
    virtual const std::string type() const;
    virtual ~Affair();

 private:

    Date _date;
    std::string _theme;
    std::string _detail;
    bool _done;

};

#endif /* AFFAIR_H_ */
