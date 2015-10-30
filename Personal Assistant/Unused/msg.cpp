/*
 * msg.cpp
 *
 *  Created on: 2015Äê5ÔÂ31ÈÕ
 *      Author: Sine
 */

#include "msg.h"

msg::msg(std::string message)
        : _message(message) {
}

std::string msg::what() {
    return _message;
}
