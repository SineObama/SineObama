/*
 * hugefloat.cpp
 *
 *  Created on: 2015Äê10ÔÂ1ÈÕ
 *      Author: Sine
 */

#include "hugefloat.h"

hugefloat operator+(hugefloat a) {
    return a;
}

hugefloat operator-(hugefloat a) {
    a.S = !a.S;
    return a;
}

hugefloat abs(hugefloat a) {
    a.S = false;
    return a;
}

hugefloat operator+(const hugefloat &a, const hugefloat &b) {
    return hugefloat::plus(a, 0, b, 0);
}

hugefloat operator-(const hugefloat &a, const hugefloat &b) {
    return hugefloat::plus(a, 0, b, 1);
}

hugefloat operator*(const hugefloat &a, const hugefloat &b) {
    return a;
}

hugefloat operator/(hugefloat a, hugefloat b) {
    return a;
}

hugefloat radical(hugefloat a, std::size_t root = 2, std::size_t time = 50) {
    return a;
}

// todo the precision....
// return 1 while a > b, -1 while a < b, 0 while a = b
int compare(const hugefloat &, const hugefloat &, int comp_precision = -1);  // -1 for default
bool operator <(const hugefloat & a, const hugefloat & b);
bool operator >(const hugefloat & a, const hugefloat & b);
bool operator <=(const hugefloat & a, const hugefloat & b);
bool operator >=(const hugefloat & a, const hugefloat & b);
bool operator ==(const hugefloat & a, const hugefloat & b);
bool operator !=(const hugefloat & a, const hugefloat & b);

hugefloat::hugefloat();
hugefloat::hugefloat(int);
hugefloat::hugefloat(double);
hugefloat::hugefloat(const std::string &);

void hugefloat::set(int);
void hugefloat::set(double);
bool hugefloat::set(const std::string &);

hugefloat hugefloat::plus(const hugefloat & a, bool asign, const hugefloat & b,
                          bool bsign) {
    hugefloat result;
    asign ^= a.S;
    bsign ^= b.S;
    if (asign && bsign) {
        result.S = true;
    } else if (!asign && !bsign) {
        result.S = false;
    } else if (asign && a > b) {
        result.S = true;
    } else if (bsign && b > a) {
        result.S = true;
    }
    return result;
}
