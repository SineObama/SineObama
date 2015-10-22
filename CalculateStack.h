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

namespace Sine {

class syntax_error : public std::runtime_error {
 public:
    syntax_error(const std::string &s)
            : std::runtime_error(s) {
    }
};

template<class Value>
class Calculator;

template<class Value>
class CalculateStack {

    friend class Calculator<Value> ;

 public:

    typedef Value (*fptr)(const Value &, const Value &);

    CalculateStack();
    ~CalculateStack();

    static void AddFunction(char, fptr, int);
    static void ClearFunction();

    void Insert(char);
    void Insert(const Value &);

    Value Calculate();
    Value GetLastResult();

    void Clear();

 private:

    void CalculateOnce();

    std::stack<Value *> StackValue;
    std::stack<char> StackChar;
    Value *LastResult;

    static std::map<char, fptr> Func;
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
void CalculateStack<Value>::AddFunction(char x, fptr p, int n) {
    if (Prior.find(x) != Prior.end())
        throw std::runtime_error(
                "CalculateStack::AddFunction(): charactor exists.");
    if (x == '(' || x == ')')
        throw std::invalid_argument(
                "CalculateStack::AddFunction(): parentheses added.");
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
    if (StackValue.size() != StackChar.size() + 1)
        throw std::runtime_error(
                "CalculateStack::Insert(char): continual charactor.");
    if (Prior.find(x) == Prior.end())
        throw std::runtime_error(
                "CalculateStack::Insert(char): charactor not found.");
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
    if (StackValue.size() != StackChar.size())
        throw std::runtime_error(
                "CalculateStack::Insert(value): continual number.");
    StackValue.push(new Value(x));
}

template<class Value>
Value CalculateStack<Value>::Calculate() {
    if (StackValue.empty())
        throw std::runtime_error(
                "CalculateStack::Calculate(): nothing to be calculated.");
    while (StackValue.size() > 1)
        CalculateOnce();
    return *(LastResult = new Value(*StackValue.top()));
}

template<class Value>
Value CalculateStack<Value>::GetLastResult() {
    if (LastResult == NULL)
        throw std::runtime_error(
                "CalculateStack::GetLastResult(): no result before.");
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
