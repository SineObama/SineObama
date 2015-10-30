/*
 * Calculator.h
 *
 *  Created on: 2015Äê10ÔÂ21ÈÕ
 *      Author: Sine
 */

#ifndef CALCULATOR_H_
#define CALCULATOR_H_

// only accept '(', ')'

#include <stack>
#include <stdexcept>
#include "CalculateTree.h"

namespace Sine {

template<class T>
class Calculator : public Sine::CalculateTree<T> {

 public:

    Calculator();
    ~Calculator();

    bool Insert(const T &);
    bool Insert(char);

    bool Calculate();
    T Result();

    void Clear();

 private:

    std::stack<CalculateTree<T>*> Stack;
    std::stack<int> look;

    T result;

};

template<class T>
Calculator<T>::Calculator() {
    Stack.push(new CalculateTree<T>);  // keep the size >= 1
}

template<class T>
Calculator<T>::~Calculator() {
    while (!Stack.empty()) {
        delete Stack.top();
        Stack.pop();
    }
}

template<class T>
bool Calculator<T>::Insert(const T &x) {
    return Stack.top()->Insert(x);
}

template<class T>
bool Calculator<T>::Insert(char x) {
    CalculateTree<T> *tem;
    switch (x) {
        case '(':
            if (Stack.top()->countnum != Stack.top()->countchar)
                return false;
            Stack.push(new CalculateTree<T>);
            break;
        case ')':
            if (Stack.top()->Calculate() == false)  // todo judge what kind of false
                return false;
            if (Stack.size() <= 1)  // means e.g. ( ) )
                return false;
            tem = Stack.top();
            Stack.pop();
            Stack.top()->Insert(tem->Result());
            delete tem;
            break;
        default:
            Stack.top()->Insert(x);
    }
    return true;
}

template<class T>
bool Calculator<T>::Calculate() {
    if (Stack.size() != 1)  // todo about error code
        return false;
    if (Stack.top()->Calculate() == false)  // todo about (1+2)
        return false;
    result = Stack.top()->Result();
    Stack.top()->Clear();
    return true;
}

template<class T>
T Calculator<T>::Result() {
    return result;
}

template<class T>
void Calculator<T>::Clear() {
    while (Stack.size() != 1) {
        delete Stack.top();
        Stack.pop();
    }
    Stack.top()->Clear();
}

}

#endif /* CALCULATOR_H_ */
