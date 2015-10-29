/*
 * CalculateStack.h
 *
 *  Created on: 2015Äê10ÔÂ22ÈÕ
 *      Author: Sine
 */

#ifndef CALCULATESTACK_H_
#define CALCULATESTACK_H_

#include <stack>
#include <map>
#include <stdexcept>
#include <string>
#include <cstdlib>
#include <iostream>
#include "CalculatorError.h"

namespace Sine {

template<class Value>
class Calculator;

template<class Value>
class CalculateStack {

    friend class Calculator<Value> ;

 public:

    typedef Value (*Fptr)(const Value &, const Value &);

    enum InsertType {
        Number,
        Operator
    };

    CalculateStack();
    ~CalculateStack();

    static void AddFunction(char, Fptr, int);
    static void ClearFunction();

    void Insert(char);
    void Insert(const Value &);
    InsertType NextInsertType();

    Value Calculate();
    Value GetLastResult();

    void Clear();

 private:

    void CalculateOnce();

    std::stack<Value *> StackValue;
    std::stack<char> StackChar;
    Value *LastResult;

    static std::map<char, Fptr> Func;
    static std::map<char, int> Prior;

};

template<class Value>
std::map<char, Value (*)(const Value &, const Value &)> CalculateStack<Value>::Func;
template<class Value>
std::map<char, int> CalculateStack<Value>::Prior;

template<class Value>
CalculateStack<Value>::CalculateStack() {
    LastResult = NULL;
}

template<class Value>
CalculateStack<Value>::~CalculateStack() {
    while (!StackValue.empty()) {
        delete StackValue.top();
        StackValue.pop();
    }
    delete LastResult;
}

template<class Value>
void CalculateStack<Value>::AddFunction(char x, Fptr p, int n) {
    if (Prior.find(x) != Prior.end())
        throw CalculatorError("operator exists.",
                              "CalculateStack::AddFunction()");
    if (x == '(' || x == ')')
        throw CalculatorError("parentheses added.",
                              "CalculateStack::AddFunction()");
    Func.insert(std::make_pair(x, p));
    Prior.insert(std::make_pair(x, n));
    return;
}

template<class Value>
void CalculateStack<Value>::ClearFunction() {
    Func.clear();
    Prior.clear();
}

template<class Value>
void CalculateStack<Value>::Insert(char x) {
    if (Prior.find(x) == Prior.end())
        throw SyntaxError(std::string("operator ") + x + " not found.",
                          "CalculateStack::Insert(char)");
    if (NextInsertType() == Number)
        throw SyntaxError(std::string("extra operator ") + x + ".",
                          "CalculateStack::Insert(char)");
    if (StackChar.empty()) {
        StackChar.push(x);
        return;
    }
    while (Prior.find(StackChar.top())->second >= Prior.find(x)->second) {
        CalculateOnce();
        if (StackChar.empty())
            break;
    }
    StackChar.push(x);
}

template<class Value>
void CalculateStack<Value>::Insert(const Value &x) {
    if (NextInsertType() == Operator)
        throw SyntaxError("continual number.", "CalculateStack::Insert(value)");
    StackValue.push(new Value(x));
}

template<class Value>
typename CalculateStack<Value>::InsertType CalculateStack<Value>::NextInsertType() {
    if (StackValue.size() == StackChar.size())
        return Number;
    else
        return Operator;
}

template<class Value>
Value CalculateStack<Value>::Calculate() {
    if (StackValue.empty())
        throw CalculatorError("nothing to be calculated.",
                              "CalculateStack::Calculate()");
    while (StackValue.size() > 1)
        CalculateOnce();
    return *(LastResult = new Value(*StackValue.top()));
}

template<class Value>
Value CalculateStack<Value>::GetLastResult() {
    if (LastResult == NULL)
        throw CalculatorError("no result before.",
                              "CalculateStack::GetLastResult()");
    return *LastResult;
}

template<class Value>
void CalculateStack<Value>::Clear() {
    while (!StackValue.empty()) {
        delete StackValue.top();
        StackValue.pop();
    }
    while (!StackChar.empty())
        StackChar.pop();
}

template<class Value>
void CalculateStack<Value>::CalculateOnce() {
    Value *a, *b;
    a = StackValue.top();
    StackValue.pop();
    b = StackValue.top();
    StackValue.pop();
    StackValue.push(new Value((*Func.find(StackChar.top())->second)(*b, *a)));
    StackChar.pop();
    delete a;
    delete b;
}

}

#endif /* CALCULATESTACK_H_ */
