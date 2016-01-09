/*
 * CalculateTree2.h
 *
 *  Created on: 2015Äê12ÔÂ31ÈÕ
 *      Author: Sine
 */

#ifndef CALCULATETREE2_H_
#define CALCULATETREE2_H_

#include <list>
#include <string>
#include <stdexcept>
#include <sstream>
#include <map>

namespace Sine {

template<class Data>
class CalculateTree2 {

    struct Token {
        std::string s;
        char getOperator() {
            if (s.length() != 1)
                throw std::runtime_error("not an operator");
            return s[0];
        }
        bool isBracket() {
            if (!s.length())
                throw std::runtime_error("empty Token");
            return s[0] == '(' || s[0] == ')';
        }
        Data getData() {
            Data data;
            std::stringstream ss(s);
            ss >> data;
            if (ss.str().length())
                throw std::runtime_error("not only data");
            return data;
        }
    };

    struct Node {
        Token token;
        Node *left, *right;
        Node(Token t)
                : token(t) {
            left = right = NULL;
        }
        ~Node() {
            delete left;
            delete right;
        }
    };

 public:

    typedef Data (*Func)(const Data &, const Data &);

    CalculateTree2();
    ~CalculateTree2();
    bool addFunc(char, int, Func);
    bool parse(const std::list<Token> &);
    Data execute();

 protected:

    void deal(std::list<Token> &, Node *&);
    bool getPart(std::list<Token> &, Node *&);
    Data execute(Node *);

 private:

    Token result;
    Node *root;

    typedef std::map<char, Func> MapFunc;
    typedef std::map<char, int> MapPrior;
    typedef typename MapFunc::iterator FuncIt;
    typedef typename MapPrior::iterator PriorIt;

    MapFunc mapFunc;
    MapPrior mapPrior;

};

template<class Data>
CalculateTree2<Data>::CalculateTree2() {
    root = NULL;
}

template<class Data>
CalculateTree2<Data>::~CalculateTree2() {
    delete root;
}

template<class Data>
bool CalculateTree2<Data>::addFunc(char ch, int prior, Func func) {
    if (mapPrior.find(ch) == mapPrior.end())
        return false;
    mapFunc.insert(std::make_pair(ch, func));
    mapPrior.insert(std::make_pair(ch, prior));
    return true;
}

template<class Data>
bool CalculateTree2<Data>::parse(const std::list<Token> &list) {
    delete root;
    root = 0;
    std::list<Token> tem(list);
    deal(tem, root);
    return true;
}

template<class Data>
Data CalculateTree2<Data>::execute() {
    if (!root)
        return result = Data(0);
    return result = execute(root);
}

template<class Data>
void CalculateTree2<Data>::deal(std::list<Token> &list, Node *&root) {
    if (getPart(list, root))
        return;

    Node *left = root;
    root = new Node(list.front());
    list.pop_front();
    root->left = left;

    if (getPart(list, root->right))
        return;
    PriorIt rootPit = mapPrior.find(root->token.getOperator());
    PriorIt nextPit = mapPrior.find(list.front().getOperator());
    if (rootPit == mapPrior.end() || nextPit == mapPrior.end())
        throw std::runtime_error("unknown operator");
    if (rootPit->second < nextPit->second)
        deal(list, root->right);
    else
        deal(list, root);
}

template<class Data>
bool CalculateTree2<Data>::getPart(std::list<Token> &list, Node *&root) {
    if (!root) {
        if (list.front().isBracket()) {
            list.pop_front();
            deal(list, root);
        } else {
            root = new Node(list.front());
            list.pop_front();
        }
    }
    if (list.empty())
        return true;
    if (list.front().isBracket()) {
        list.pop_front();
        return true;
    }
    return false;
}

template<class Data>
Data CalculateTree2<Data>::execute(Node *p) {
    if (!p->left && !p->right)
        return p->token;
    if (!p->left || !p->right)
        throw std::runtime_error("an tree node with one child");
    typename std::map<char, Func>::iterator it = mapFunc.find(
            p->token.getOperator());
    if (it == mapFunc.end())
        throw std::runtime_error("unknown operator");
    return (*it->second)(execute(p->left), execute(p->right));
}

}

#endif /* CALCULATETREE2_H_ */
