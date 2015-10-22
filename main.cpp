/*
 * main.cpp
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Sine
 */

#include "CalculateStack.h"

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

using namespace std;

int main() {
//    throw Sine::syntax_error("hehe");
    Sine::CalculateStack<int> t;
    Sine::CalculateStack<int>::AddFunction('+', &jia, 1);
    Sine::CalculateStack<int>::AddFunction('-', &jian, 1);
    Sine::CalculateStack<int>::AddFunction('*', &cheng, 2);
    Sine::CalculateStack<int>::AddFunction('/', &chu, 2);
    while (1) {
        char c = cin.peek();
        if (c >= '0' && c <= '9') {
            int i;
            cin >> i;
//            cerr << i << " i\n";
            t.Insert(i);
        } else {
            while (cin.peek() == ' ')
                cin.ignore();
            cin >> c;
            while (cin.peek() == ' ')
                cin.ignore();
            if (c == '=')
                break;
//            cerr << c << " c\n";
            t.Insert(c);
        }
    }
    cout << t.Calculate();
}

