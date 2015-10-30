/*
 * main.cpp
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Sine
 */


int jia(const int & a, const int & b) {
    return a + b;
}

int jian(const int & a, const int & b) {
    return a - b;
}

int cheng(const int & a, const int & b) {
    return a * b;
}

int chu(const int & a, const int & b) {
    return a / b;
}

#include <iostream>
#include <cstdio>
#include <sstream>
#include "Calculator.h"
#include "Sqrt.h"

using namespace std;

int main() {
    cout << Sine::Sqrt1(1000000000000000.0, 3, 1.0);
    return 0;
    Sine::Calculator<int> t;
    Sine::Calculator<int>::AddFunction('+', &jia, 1);
    Sine::Calculator<int>::AddFunction('-', &jian, 1);
    Sine::Calculator<int>::AddFunction('*', &cheng, 2);
    Sine::Calculator<int>::AddFunction('/', &chu, 2);
    while (1) {
        char c = cin.peek();
        if (c >= '0' && c <= '9') {
            int i;
            cin >> i;
//            cerr << i << " i\n";
            t.Insert(i);
        } else {
            cin >> c;
            if (c == '=')
                break;
//            cerr << c << " c\n";
            t.Insert(c);
        }
            while (cin.peek() == ' ')
                cin.ignore();
    }
    cout << t.Calculate();
}

