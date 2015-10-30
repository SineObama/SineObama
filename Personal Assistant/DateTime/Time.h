/*
 * Time.h
 *
 *  Created on: 2015Äê6ÔÂ6ÈÕ
 *      Author: Sine
 */

// the second can be hidden

// very simple test passed
// todo set fail information

#ifndef TIME_H_
#define TIME_H_

#include <string>

class Time {

 public:

    explicit Time(int hour = 0, int minute = 0, int second = 0);
//    explicit Time(const std::string &);
    Time(const Time &);

    void settime(int hour, int minute, int second = 0);
    void sethour(int );
    void setminute(int);
    void setsecond(int );
//    static void set_use_second(bool);

    int gethour();
    int getminute();
    int getsecond();
//    static bool set_use_second();

    template<class stream>
    stream &operator>>(stream &, Time &);
    // todo any problems ? after delete the "friend" above
    template<class stream>
    // print in the format: 01:02:03
    friend stream &operator<<(stream &, const Time &);

    friend bool operator==(const Time &, const Time &);
    friend bool operator<(const Time &, const Time &);
    friend bool operator<=(const Time &, const Time &);
    friend bool operator>(const Time &, const Time &);
    friend bool operator>=(const Time &, const Time &);

    static bool _use_second;

 private:

    int _hour;
    int _minute;
    int _second;

};

#include "TimeTemplate.cpp"

#endif /* TIME_H_ */
