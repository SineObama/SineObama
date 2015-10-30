/*
 * Task.h
 *
 *  Created on: 2015Äê5ÔÂ29ÈÕ
 *      Author: Sine
 */

#include "Affair.h"
#include <string>

#ifndef TASK_H_
#define TASK_H_

class Task : public Affair {

 public:

    Task(const Date & date, const std::string & theme,
         const std::string & detail);
    Task(const Affair & affair);
    virtual void print() const;
    virtual const std::string type() const;

};

#endif /* TASK_H_ */
