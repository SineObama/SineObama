/*
 * Calculator_error.h
 *
 *  Created on: 2015Äê10ÔÂ24ÈÕ
 *      Author: Sine
 */

#ifndef CALCULATORERROR_H_
#define CALCULATORERROR_H_

#include <stdexcept>
#include <string>

namespace Sine {

class CalculatorError : public std::runtime_error {
 public:
    CalculatorError(const std::string &s, const std::string &f = "")
            : std::runtime_error(s),
              from(f) {
    }
    virtual const char * function() {
        return from.c_str();
    }
    virtual ~CalculatorError() throw() {}
 protected:
    std::string from;
};

class SyntaxError : public CalculatorError {
 public:
    SyntaxError(const std::string &s, const std::string &f = "")
            : CalculatorError("syntax error: " + s, f) {
    }
    virtual ~SyntaxError() throw() {}
};

}

#endif /* CALCULATORERROR_H_ */
