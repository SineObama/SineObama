/*
 * System.h
 *
 *  Created on: 2015Äê6ÔÂ7ÈÕ
 *      Author: Sine
 */

#ifndef SYSTEM_H_
#define SYSTEM_H_

#include "Password.h"
#include <string>
#include <vector>
#include <utility>

#define DATA_PATH ".\\data\\"

class System {

 public:

    static bool user_register(const std::string & username, const std::string & password);

    static bool login(const std::string & username, const std::string & password);
    static bool logout();

    static bool modify_password(const std::string & password);


 private:

    System();
// todo not private ?
    ~System();
    // remind the coming event......
    static void remind() const;

    static std::string encrypt(const std::string &);
    static std::string decrypt(const std::string &);

    static bool _adminmode;

    static std::string _username;
    static std::string _password;

    // containing user name and its password
    friend class PasswordMap;
    static PasswordMap _usersdata;

};

#endif /* SYSTEM_H_ */
