/*
 * EncryptionStorage.cpp
 *
 *  Created on: 2015Äê6ÔÂ10ÈÕ
 *      Author: Sine
 */

// you ought not to see this file

#include "Encryption.h"
#include "DoubleNine.h"
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <string>
#include <cmath>

std::string Encryption::encrypt(const std::string & in) {
//    return basicencrypt(basicencrypt2(basicencrypt2(basicencrypt(in))));
    return basicencrypt2(basicencrypt2(basicencrypt(in)));
}

std::string Encryption::decrypt(const std::string & in) {
//    return basicdecrypt(basicdecrypt2(basicdecrypt2(basicdecrypt(in))));
    return basicdecrypt(basicdecrypt2(basicdecrypt2(in)));
}

std::string Encryption::basicencrypt(const std::string & in) {
    std::string out;
    out.reserve(in.size() * LEN + 3);
    srand(time(NULL));
    for (unsigned int i = 0; i < in.size(); i++) {
        int num = (rand() % int(pow(10, LEN) / 128)) * 128 + in[i];
        char tem[LEN + 1] = { };
        snprintf(tem, LEN + 1, "%0*d", LEN, num);
        out += tem;  // may be not efficient
    }
    return out;
}

std::string Encryption::basicdecrypt(const std::string & in) {
    std::string out;
    out.reserve(in.size() / LEN + 3);
    std::stringstream ss, ss2;  // it is said that construct this cost much time
    ss << in;
    while (ss.rdstate() == ss.goodbit) {
        std::string tem;
        ss >> std::setw(LEN) >> tem;
        ss2.sync();
        ss2.clear();
        ss2 << tem;
        int num = 0;
        ss2 >> num;
        num %= 128;
        out += static_cast<char>(num);
    }
    return out;
}

std::string Encryption::basicencrypt2(const std::string & in) {
    std::string out;
    out.reserve(in.size() * 2 + 100);  // maybe efficient todo the size
    srand(time(NULL));
    int order[5] = { };
    {  // set the order of the five parts
        bool mark[5] = { };
        int ccount = 0, mcount = 0;
        while (mcount < 5) {
            if (ccount == 0) {
                int a = rand() % 10;
                out += '0' + a;
                ccount = a;
            }
            int a = rand() % 10;
            out += '0' + a;
            a %= 5;
            if (!mark[a]) {
                mark[a] = true;
                order[mcount++] = a + 1;
            }
            ccount--;
        }
    }  // end set five parts

    DoubleNine DN(order);
    std::stringstream ss;
    ss << in;
    while (ss.rdstate() == ss.goodbit) {  // todo may be weak
        int n1 = rand() % 10, m1 = rand() % 10, n2 = rand() % 10, m2 = rand()
                % 10;
        out += '0' + n1;
        out += '0' + m1;
        out += '0' + n2;
        out += '0' + m2;
        DN.setpoint(n1 + 1, m1 + 1, n2 + 1, m2 + 1);
        std::string tem;
        ss >> std::setw(DN.getsize()) >> tem;
        out += DN.encrypt(tem);
    }
    return out;
}

std::string Encryption::basicdecrypt2(const std::string & in) {
    std::string out;
    out.reserve(in.size());
    std::stringstream ss;
    ss << in;

    int order[5] = { };
    {  // get the order of the five parts
        bool mark[5] = { };
        int ccount = 0, mcount = 0;
        while (mcount < 5) {
            if (ccount == 0) {
                char tem;
                ss >> tem;
                ccount = tem - '0';
            }
            char tem;
            ss >> tem;
            int a = tem - '0';
            a %= 5;
            if (!mark[a]) {
                mark[a] = true;
                order[mcount++] = a + 1;
            }
            ccount--;
        }
    }  // end get five parts

    DoubleNine DN(order);
    while (ss.rdstate() == ss.goodbit) {
        char tem;
        ss >> tem;
        int n1 = tem - '0';
        ss >> tem;
        int m1 = tem - '0';
        ss >> tem;
        int n2 = tem - '0';
        ss >> tem;
        int m2 = tem - '0';
        DN.setpoint(n1 + 1, m1 + 1, n2 + 1, m2 + 1);
        std::string tem2;
        ss >> std::setw(DN.getsize()) >> tem2;
        out += DN.decrypt(tem2);
    }
    return out;
}
