/*
 * CalculateTree.h
 *
 *  Created on: 2015年10月21日
 *      Author: Sine
 */

#ifndef CALCULATETREE_H_
#define CALCULATETREE_H_

// class T for the element to be calculated
// insert the elements and charactors one by one using Insert()
// by infix order(中序), e.g. 1,'+',2,'*',3

// DON'T insert any parentheses
// (this can be implement by using a subtree to calculate between the parentheses)
// for more, '(' and ')' can not be used as the charactor

// the operation and priority of charactors will be specified by you
// using AddFunction(the charactor, the function pointer, the priority number)
// the bigger the priority number is, the more prior the charactor is

// if T is a class, it must have a default constructor and a copy constructor

#include <map>
#include <cstdlib>
#include <stdexcept>

namespace Sine {

template<class T>
struct TreeNode {
    TreeNode<T> *Father, *LeftSon, *RightSon;
    T value;
    char ch;
    TreeNode() {
        Father = LeftSon = RightSon = NULL;
        ch = 0;
    }
    TreeNode(const T & x)
            : value(x) {
        Father = LeftSon = RightSon = NULL;
        ch = 0;
    }
};

template<class T>
class Calculator;

template<class T>
class CalculateTree {

    friend Calculator<T> ;

 public:

    typedef T (*fptr)(const T &, const T &);

    CalculateTree();
    ~CalculateTree();

    void Insert(char);
    void Insert(const T &);

    static void AddFunction(char charactor, fptr functionPointer, int priority);

    void Calculate();
    // get the result calculated at the last time
    T Result();

    // clear the whole tree
    void Clear();
    // clear the specified charactor and function and priority
    static void ClearFunction();

 protected:
    // the implement of some function

    T CalculateNode(TreeNode<T>*);
    void DeleteNode(TreeNode<T>*);

 private:

    static std::map<char, fptr> func;
    static std::map<char, int> prior;

    TreeNode<T> *root, *end;
    T result;
    int countchar, countnum;

};

template<class T>
std::map<char, T (*)(const T &, const T &)> CalculateTree<T>::func;
template<class T>
std::map<char, int> CalculateTree<T>::prior;

using namespace std;

template<class T>
CalculateTree<T>::CalculateTree() {
    root = end = NULL;
    countchar = countnum = 0;
}

template<class T>
CalculateTree<T>::~CalculateTree() {
    Clear();
}

template<class T>
void CalculateTree<T>::Insert(char x) {
    if (countchar != countnum - 1)
        throw std::invalid_argument(
                "CalculateTree::Insert(): continual number or charactor.");
    if (prior.find(x) == prior.end())
        throw std::invalid_argument(
                "CalculateTree::Insert(): charactor not found.");
    countchar++;

    int newprior = prior[x];
    if (end->Father != NULL
            && prior.find(end->Father->ch)->second <= newprior) {  // end is to be replace
        while (end->Father != NULL
                && prior.find(end->Father->ch)->second == newprior)
            end = end->Father;
        if (end->Father != NULL) {
            TreeNode<T> *Father = end->Father, *LeftSon = end, *me =
                    new TreeNode<T>;
            me->ch = x;
            me->Father = Father;
            if (Father == NULL)
                root = me;
            me->LeftSon = LeftSon;
            end->Father->RightSon = me;
            end = me;
        }
    }
    if (end->Father == NULL) {
        end->Father = new TreeNode<T>;
        TreeNode<T> *LeftSon = end;
        end = end->Father;
        root = end;
        end->LeftSon = LeftSon;
        end->ch = x;
    }
    return;
}

template<class T>
void CalculateTree<T>::Insert(const T &x) {
    if (countchar != countnum)
        throw std::invalid_argument(
                "CalculateTree::Insert(): continual number or charactor.");
    countnum++;

    if (root == NULL) {
        root = new TreeNode<T>(x);
        end = root;
    } else {
        end->RightSon = new TreeNode<T>(x);
        TreeNode<T> *Father = end;
        end = end->RightSon;
        end->Father = Father;
    }
    return;
}

template<class T>
void CalculateTree<T>::AddFunction(char x, fptr p, int n) {
    if (prior.find(x) != prior.end())
        throw std::invalid_argument(
                "CalculateTree::AddFunction(): charactor exists.");
    if (x == '(' || x == ')')
        throw std::invalid_argument(
                "CalculateTree::AddFunction(): parentheses added.");
    func.insert(make_pair(x, p));
    prior.insert(make_pair(x, n));
    return;
}

template<class T>
void CalculateTree<T>::Calculate() {
    if (countnum != countchar + 1)
        throw std::invalid_argument("CalculateTree::Calculate(): lost of number.");
    if (root == NULL)
        throw std::invalid_argument("CalculateTree::Calculate(): nothing to be calculated.");  // todo too...
    result = CalculateNode(root);
    Clear();
    return;
}

template<class T>
T CalculateTree<T>::Result() {
    return result;
}

template<class T>
void CalculateTree<T>::Clear() {
    if (root != NULL)
        DeleteNode(root);
    root = end = NULL;
    countchar = countnum = 0;
}

template<class T>
void CalculateTree<T>::ClearFunction() {
    prior.clear();
    func.clear();
}

template<class T>
T CalculateTree<T>::CalculateNode(TreeNode<T> *x) {
    if (x->ch == 0)
        return x->value;
    return (*func.find(x->ch)->second)(CalculateNode(x->LeftSon),
                                       CalculateNode(x->RightSon));
}

template<class T>
void CalculateTree<T>::DeleteNode(TreeNode<T> *x) {
    if (x->LeftSon != NULL)
        DeleteNode(x->LeftSon);
    if (x->RightSon != NULL)
        DeleteNode(x->RightSon);
    delete x;
}

}

#endif /* CALCULATETREE_H_ */
