/*
 * main.cpp
 *
 *  Created on: 2015Äê10ÔÂ21ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>
#include <cmath>
#include "Calculator.h"

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

int main() {
    Sine::Calculator<double> tem;
    tem.AddFunction('+', &jia, 1);
    tem.AddFunction('-', &jian, 1);
    tem.AddFunction('*', &cheng, 2);
    tem.AddFunction('/', &chu, 2);
    tem.AddFunction('^', &cifang, 3);
    tem.Insert('(');
    tem.Insert(1.0);
    tem.Insert('+');
    tem.Insert(7.0);
    tem.Insert(')');
    tem.Insert('-');
    tem.Insert(9.0);
    tem.Insert('*');
    tem.Insert('(');
    tem.Insert(3.0);
    tem.Insert('/');
    tem.Insert(3.0);
    tem.Insert(')');
    tem.Insert('^');
    tem.Insert(2.0);
    if (tem.Calculate())
    cout << tem.Result() << endl;
}
