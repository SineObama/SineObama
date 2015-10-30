/*
 * Event.h
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#include "Affair.h"
#include <string>

#ifndef EVENT_H_
#define EVENT_H_

class Event : public Affair {

 public:

    Event(const Date & date, const std::string & theme,
          const std::string & detail);
    Event(const Affair & affair);
    virtual void print() const;
    virtual const std::string type() const;

};

#endif /* EVENT_H_ */
