/*
 * DateTime.h
 *
 *  Created on: 2015Äê6ÔÂ6ÈÕ
 *      Author: Sine
 */

#ifndef DATETIME_H_
#define DATETIME_H_

#include "Date.h"
#include "Time.h"

class DateTime : public Date, public Time {

 public:

    explicit DateTime(int year = DEFAULT_YEAR, int month = DEFAULT_MONTH,
                      int day = DEFAULT_DAY, int hour = 0, int minute = 0,
                      int second = 0);
    DateTime(const DateTime &);

    // though not really necessary ?
    template<class stream>
    friend stream &operator>>(stream & s, DateTime & ref);
    template<class stream>
    friend stream &operator<<(stream & s, const DateTime & ref);

 private:

};

#include "DateTimeTemplate.cpp"

#endif /* DATETIME_H_ */
