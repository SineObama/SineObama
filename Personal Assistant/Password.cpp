/*
 * Password.cpp
 *
 *  Created on: 2015Äê6ÔÂ7ÈÕ
 *      Author: Sine
 */

#include "Password.h"
#include <string>
#include <fstream>

PasswordMap::PasswordMap() {
    read();
}

PasswordMap::~PasswordMap() {
    save();
}

void PasswordMap::read() {
    // todo check....
    std::ifstream file;
    std::string filefullpath = DATA_PATH + encrypt(PASSWORDMAP_FILE_NAME)
            + ".txt";
    file.open(filefullpath.c_str(), std::ios::in);
    std::string name, password;
    while (file >> name >> password) {
        (*this).insert(make_pair(decrypt(name), decrypt(password)));
    }
}

void PasswordMap::save() const {
    // todo check....
    std::ofstream file;
    std::string filefullpath = DATA_PATH + encrypt(PASSWORDMAP_FILE_NAME)
            + ".txt";
    file.open(filefullpath.c_str(), std::ios::out);
    std::map<std::string, std::string>::iterator it;
    for (it = this->begin(); it != this->end(); it++) {
        file << encrypt(it->first) << ' ' << encrypt(it->second) << '\n';
    }
}

std::string PasswordMap::encrypt(const std::string & s) {
    // todo further......
    std::string out = s;
    int i;
    for (i = 0; i < s.size(); i++) {
        out[i] = 127 - out[i];
    }
    return s;
}
std::string PasswordMap::decrypt(const std::string & s) {
    // todo further......
    std::string out = s;
    int i;
    for (i = 0; i < s.size(); i++) {
        out[i] = 127 - out[i];
    }
    return s;
}
