/*
 * Data.h
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

// will check the validity and print the message

#include <string>
#include <iostream>

#ifndef DATA_H_
#define DATA_H_


#define DEFAULT_YEAR 2015
#define DEFAULT_MONTH 1
#define DEFAULT_DAY 1

// used when setting
#define _UNCHANGE -1

class Date {

 public:

    explicit Date(int year, int month, int day);
    Date(const Date &);
//    explicit Date(const std::string &);

    void setdate(int year, int month = _UNCHANGE, int day = _UNCHANGE);
    void setyear(int);
    void setmonth(int);
    void setday(int);

    int getyear() const;
    int getmonth() const;
    int getday() const;

    // set or get the separator and whether print the invalidity warning
    static void separator(char);
    static char separator() const;
    static void printwarning(bool);
    static bool printwarning() const;

    // input year, month, day with a separator between them
    // include printing the warning message
    template<class stream>
    stream &operator>>(stream &, Date &);
    // print in the format: 2015-06-01
    template<class stream>
    friend stream &operator<<(stream &, const Date &);

    friend bool operator==(const Date &, const Date &);
    friend bool operator<(const Date &, const Date &);
    friend bool operator<=(const Date &, const Date &);
    friend bool operator>(const Date &, const Date &);
    friend bool operator>=(const Date &, const Date &);

    // the max day in a month, determined by the year and the month
    static int maxday(int year, int month) throw (std::invalid_argument);
    static bool leapyear(int year);

 private:

    int _year;
    int _month;
    int _day;

    // int the printing format
    static char _separator;

    // date will be automatically set to default
    static bool _print_warning;

};

#include "DateTemplate.cpp"

#endif /* DATA_H_ */
