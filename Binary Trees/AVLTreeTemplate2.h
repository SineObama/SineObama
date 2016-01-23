/*
 * AVLTreeTemplate2.h
 *
 *  Created on: 2016年1月22日
 *      Author: Sine
 */

#ifndef AVLTREETEMPLATE2_H_
#define AVLTREETEMPLATE2_H_

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "AVLTree2.h"

namespace Sine {

template<class T>
AVLTree2<T>::Node::Node(const T &d)
        : data(d) {
    child[0] = child[1] = 0;
    high = 1;
}

template<class T>
AVLTree2<T>::AVLTree2() {
    _root = 0;
}

template<class T>
AVLTree2<T>::AVLTree2(const AVLTree2 &x) {
    recursiveCopy(_root, x._root);
}

template<class T>
AVLTree2<T>::~AVLTree2() {
    recursiveRemoveTree(_root);
}

template<class T>
bool AVLTree2<T>::insert(const T &data) {
    return recursiveInsertToTree(_root, data);
}

template<class T>
bool AVLTree2<T>::remove(const T &data) {
    return recursiveRemoveFromTree1(_root, data);
}

template<class T>
const typename AVLTree2<T>::Node *AVLTree2<T>::search(const T &data) const {
    return searchInTree(_root, data);
}

template<class T>
void AVLTree2<T>::inorder(void (*visit)(const T &)) const {
    recursiveInorder(_root, visit);
}

template<class T>
void AVLTree2<T>::print() const {
    printTree(_root);
}

template<class T>
int AVLTree2<T>::validator() const {
    int num = 0;
    checkHeight(_root, num);
    return num;
}

template<class T>
int AVLTree2<T>::getBF(const Node *root) {
    return root ? getHigh(root->child[1]) - getHigh(root->child[0]) : 0;
}

template<class T>
short AVLTree2<T>::getHigh(const Node *root) {
    return root ? root->high : 0;
}

template<class T>
void AVLTree2<T>::recursiveInorder(Node *root, void (*visit)(const T &)) const {
    if (!root)
        return;
    recursiveInorder(root->child[0], visit);
    (*visit)(root->data);
    recursiveInorder(root->child[1], visit);
}

template<class T>
void AVLTree2<T>::recursiveCopy(Node *&root, const Node *other) {
    if (!other) {
        root = 0;
        return;
    }
    root = new Node(other->data);
    root->high = other->high;
    recursiveCopy(root->child[0], other->child[0]);
    recursiveCopy(root->child[1], other->child[1]);

}

template<class T>
void AVLTree2<T>::recursiveRemoveTree(Node *&root) {
    if (!root)
        return;
    recursiveRemoveTree(root->child[0]);
    recursiveRemoveTree(root->child[1]);
    delete root;
    root = 0;
}

template<class T>
bool AVLTree2<T>::recursiveInsertToTree(Node *&root, const T &data) {
    if (!root)
        return (root = new Node(data));
    if (data == root->data)
        return false;
    Node *&next = root->child[data > root->data];
    if (!recursiveInsertToTree(next, data))
        return false;
    fixNode(root);
    return true;
}

/* 找到要删除的节点后使他向下移动，最终成为叶节点再删除 */
template<class T>
bool AVLTree2<T>::recursiveRemoveFromTree(Node *&root, const T &data) {
    if (!root)
        return false;
    if (root->data == data) {  // 尝试使当前节点下移
        if (root->high == 1) {  // 没有子树了
            delete root;
            root = 0;
            return true;
        }
        rotate(root, (getBF(root) > 0));
    }
    Node *&next = root->child[data > root->data];
    if (!next || !recursiveRemoveFromTree(next, data))
        return false;
    fixNode(root);
    return true;
}

/* 节点高度发生变化时（减少1）返回真。 */
template<class T>
void AVLTree2<T>::fixNode(Node *&node) {
    if (!node)
        return;
    int BF = getBF(node);
    if (BF < 2 && BF > -2) {
        fixHigh(node);
        return;
    }
    int num = BF > 0;
    if (getBF(node->child[num]) * BF < 0)
        rotate(node->child[num], !num);
    rotate(node, num);
    return;
}

template<class T>
void AVLTree2<T>::fixHigh(Node *&node) {
    if (!node)
        throw std::runtime_error("节点为空");
    node->high = getHigherChild(node);
}

template<class T>
void AVLTree2<T>::rotate(Node *&node, bool left) {
    int num = left;
    Node *newRoot = node->child[num];
    if (!newRoot)
        throw std::runtime_error("没有可旋转的节点");
    node->child[num] = newRoot->child[1 - num];
    fixHigh(node);
    newRoot->child[1 - num] = node;
    node = newRoot;
    fixHigh(node);
    return;
}

template<class T>
const typename AVLTree2<T>::Node *AVLTree2<T>::searchInTree(const Node *root,
                                                            const T &data) {
    if (!root || data == root->data)
        return root;
    return data < root->data ?
            searchInTree(root->child[0], data) :
            searchInTree(root->child[1], data);
}

template<class T>
void AVLTree2<T>::printTree(const Node *root, std::string s = "") {
    std::vector<std::stringstream *> v;
    inorderPrint(v, root, 0);
    std::cout << s << std::endl;
    for (unsigned int i = 0; i < v.size(); i++) {
        std::cout << v[i]->str() << std::endl;
        delete v[i];
    }
}

template<class T>
void AVLTree2<T>::inorderPrint(std::vector<std::stringstream *> &v,
                               const Node *root, int n) {
    if (!root)
        return;
    if (v.size() <= n) {
        v.push_back(new std::stringstream());
    }
    inorderPrint(v, root->child[0], n + 1);
    (*v[n]) << ' ' << root->data;
    inorderPrint(v, root->child[1], n + 1);
}

// for validator
template<class T>
short AVLTree2<T>::checkHeight(Node *root, int &n) {
    if (!root)
        return 0;
    if (root->high != getHigherChild(root) + 1)
        n++;
    int BF = getBF(root);
    if (BF > 1 || BF < -1)
        n++;
    return root->high;
}

template<class T>
short AVLTree2<T>::getHigherChild(Node *node) {
    if (!node)
        throw std::runtime_error("节点为空");
    short left = getHigh(node->child[0]);
    short right = getHigh(node->child[1]);
    return left > right ? left : right;
}

/* 一开始想的方法。找到要删除的点后，把它和左子树中最大的点交换，
 * 从而将问题转化成删除叶节点。 */
template<class T>
bool AVLTree2<T>::recursiveRemoveFromTree1(Node *&root, const T &data) {
    if (!root)
        return false;
    if (data == root->data)
        removeNode(root);
    else if (!recursiveRemoveFromTree1(root->child[root->data < data], data))
        return false;
    fixNode(root);
    return true;
}

template<class T>
void AVLTree2<T>::removeNode(Node *&node) {
    if (!node)
        return;
    if (!node->child[0]) {
        Node *tem = node;
        node = node->child[1];
        delete tem;
        return;
    }
    Node *tem = removeTheBiggest(node->child[0]);
    tem->child[0] = node->child[0];
    tem->child[1] = node->child[1];
    tem->high = node->high;
    delete node;
    node = tem;
}

template<class T>
typename AVLTree2<T>::Node *AVLTree2<T>::removeTheBiggest(Node *&root) {
    if (!root)
        throw std::runtime_error("节点不存在");
    Node *&next = root->child[1];
    if (!next) {
        Node *newNode = root->child[0], *back = root;
        root->child[0] = 0;
        root = newNode;
        return back;
    }
    Node *back = removeTheBiggest(next);
    fixNode(root);
    return back;
}

}

#endif /* AVLTREETEMPLATE2_H_ */
