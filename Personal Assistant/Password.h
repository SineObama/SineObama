/*
 * Password.h
 *
 *  Created on: 2015Äê6ÔÂ7ÈÕ
 *      Author: Sine
 */

// can only be used by System

#ifndef PASSWORD_H_
#define PASSWORD_H_

#include "System.h"
#include <map>
#include <string>

#define PASSWORDMAP_FILE_NAME "PWM"

class PasswordMap : public std::map<std::string, std::string> {

    friend class System;

 private:

    // read data
    PasswordMap();
    // save data
    ~PasswordMap();

    void read();
    void save() const;

    static std::string encrypt(const std::string &);
    static std::string decrypt(const std::string &);

};

#endif /* PASSWORD_H_ */
