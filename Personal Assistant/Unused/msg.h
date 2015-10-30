/*
 * msg.h
 *
 *  Created on: 2015��5��31��
 *      Author: Sine
 */

// used for exception handling...(so cute

#ifndef MSG_H_
#define MSG_H_

#include <string>

class msg {
 public:
    msg(std::string message);
    std::string what();
 private:
    std::string _message;
};

#endif /* MSG_H_ */
