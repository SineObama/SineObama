/*
 * DoubleNine.h
 *
 *  Created on: 2015Äê6ÔÂ12ÈÕ
 *      Author: Sine
 */

// a "stupid" way to encrypt
// every time the max number of charactors to be encrypt (simply misordered)
// IS CERTAIN
// constructor needs five different numbers from 1~5
// setpoint needs four numbers from 1~ROW-1
// then the max size is certain
// to decrypt, need these numbers too

#ifndef DOUBLENINE_H_
#define DOUBLENINE_H_

#include <string>

const int ROW = 11;

class DoubleNine {

 public:

    DoubleNine(const int order[]);
    DoubleNine(int a, int b, int c, int d, int e);
    void setpoint(int n1, int m1, int n2, int m2);
    unsigned int getsize();

    std::string encrypt(const std::string &);
    std::string decrypt(const std::string &);

 private:

    bool checkorder();
    void writetomap(const std::string &, char [][ROW]);
    std::string readfrommap(const char [][ROW]);

    int _order[5];
    int _nu, _nd, _ml, _mr;

};

#endif /* DOUBLENINE_H_ */
