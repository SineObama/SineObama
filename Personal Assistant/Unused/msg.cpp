/*
 * msg.cpp
 *
 *  Created on: 2015��5��31��
 *      Author: Sine
 */

#include "msg.h"

msg::msg(std::string message)
        : _message(message) {
}

std::string msg::what() {
    return _message;
}
