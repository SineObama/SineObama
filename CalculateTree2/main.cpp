/*
 * main.cpp
 *
 *  Created on: 2015Äê10ÔÂ21ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>
#include <cmath>

using namespace std;

double jia(const double &a, const double &b) {
    cout << a << "+" << b << "\n";
    return a + b;
}

double jian(const double &a, const double &b) {
    cout << a << "-" << b << "\n";
    return a - b;
}

double cheng(const double &a, const double &b) {
    cout << a << "*" << b << "\n";
    return a * b;
}

double chu(const double &a, const double &b) {
    cout << a << "/" << b << "\n";
    return a / b;
}

double cifang(const double &a, const double &b) {
    cout << a << "^" << b << "\n";
    return pow(a, b);
}

//#include <map>
//template<class T>
//class hehe {
// public:
//    typedef T (*lala)();
//    map<lala, int> m;
//    void t();
//};
//
//template<class T>
//void hehe<T>::t() {
//    typename map<lala, int>::iterator it = m.begin();
//    int a = it->second;
//}

#include "CalculateTree2.h"

int main() {
    Sine::CalculateTree2<double> t;
}
