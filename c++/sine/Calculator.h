/*
 * Calculator.h
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Sine
 */

#ifndef CALCULATOR_H_
#define CALCULATOR_H_


// only accept '(', ')'

#include <stack>
#include <stdexcept>
#include "CalculateStack.h"
#include "CalculatorError.h"

namespace Sine {

template<class T>
class Calculator : public Sine::CalculateStack<T> {

 public:

    Calculator();
    ~Calculator();

    void Insert(const T &);
    void Insert(char);

    T Calculate();
    T Result();

    void Clear();

 private:

    std::stack<CalculateStack<T>*> Stack;
    T result;

};

template<class T>
Calculator<T>::Calculator() {
    Stack.push(new CalculateStack<T>);  // keep the size >= 1
}

template<class T>
Calculator<T>::~Calculator() {
    while (!Stack.empty()) {
        delete Stack.top();
        Stack.pop();
    }
}

template<class T>
void Calculator<T>::Insert(const T &x) {
    Stack.top()->Insert(x);
    return;
}

template<class T>
void Calculator<T>::Insert(char x) {
    CalculateStack<T> *tem;
    switch (x) {
        case '(':
            if (Stack.top()->NextInsertType() == CalculateStack<T>::Operator)
                throw SyntaxError("miss operator before '('.", "Calculator::Insert(char)");
            Stack.push(new CalculateStack<T>);
            break;
        case ')':
            if (Stack.top()->StackValue.empty())
                throw SyntaxError("nothing to be calculated between ( ).", "Calculator::Insert()");
            if (Stack.size() <= 1)
                throw SyntaxError("more ')' than '('.", "Calculator::Insert(char)");
            tem = Stack.top();
            Stack.pop();
            Stack.top()->Insert(tem->Calculate());
            delete tem;
            break;
        default:
            Stack.top()->Insert(x);
    }
}

template<class T>
T Calculator<T>::Calculate() {
    if (Stack.size() != 1)
        throw SyntaxError("miss ')'.", "Calculator::Calculate()");
    Stack.top()->Calculate();
    result = Stack.top()->Calculate();
    Stack.top()->Clear();
    return result;
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
