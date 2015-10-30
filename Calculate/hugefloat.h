/*
 * hugefloat.h
 *
 *  Created on: 2015Äê10ÔÂ1ÈÕ
 *      Author: Sine
 */

#ifndef HUGEFLOAT_H_
#define HUGEFLOAT_H_

#include <string>
#include <stdexcept>

#define Length 4

class hugefloat {

    friend hugefloat operator+(hugefloat);
    friend hugefloat operator-(hugefloat);
    friend hugefloat abs(hugefloat);
    friend hugefloat operator+(const hugefloat &, const hugefloat &);
    friend hugefloat operator-(const hugefloat &, const hugefloat &);
    friend hugefloat operator*(const hugefloat &, const hugefloat &);
    friend hugefloat operator/(hugefloat, hugefloat);  // depend on binary operator -
    friend hugefloat radical(hugefloat, std::size_t root = 2, std::size_t time =
    50);  // todo

    // todo the precision....
    // return 1 while a > b, -1 while a < b, 0 while a = b
    friend int compare(const hugefloat &, const hugefloat &,
                       int comp_precision = -1);  // -1 for default
    friend bool operator <(const hugefloat & a, const hugefloat & b);
    friend bool operator >(const hugefloat & a, const hugefloat & b);
    friend bool operator <=(const hugefloat & a, const hugefloat & b);
    friend bool operator >=(const hugefloat & a, const hugefloat & b);
    friend bool operator ==(const hugefloat & a, const hugefloat & b);
    friend bool operator !=(const hugefloat & a, const hugefloat & b);

 public:

    hugefloat();
    hugefloat(int);
    hugefloat(double);
    hugefloat(const std::string &);

    void set(int);
    void set(double);
    bool set(const std::string &);

 protected:

    static hugefloat plus(const hugefloat & a, bool asign, const hugefloat & b,
                            bool bsign);  // the implement of +-

 private:

    bool S;
    int E;
    unsigned int M[Length];

};

#endif /* HUGEFLOAT_H_ */
