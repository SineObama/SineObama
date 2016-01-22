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
AVLTree2<T>::AVLTree2() {
    _root = 0;
}

template<class T>
AVLTree2<T>::AVLTree2(const AVLTree2 &x) {
    recursiveCopy(_root, x._root);
}

template<class T>
AVLTree2<T>::~AVLTree2() {
    removeTree(_root);
}

template<class T>
bool AVLTree2<T>::insert(const T &data) {
    return insertToTree(_root, data);
}

template<class T>
bool AVLTree2<T>::remove(const T &data) {
    return removeFromTree1(_root, data);
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
    if (!root)
        return 0;
    short left = root->child[0] ? root->child[0]->high : 0;
    short right = root->child[1] ? root->child[1]->high : 0;
    return right - left;
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
    } else {
        root = new Node(other->data);
        root->high = other->high;
        recursiveCopy(root->child[0], other->child[0]);
        recursiveCopy(root->child[1], other->child[1]);
    }
}

template<class T>
void AVLTree2<T>::removeTree(Node *&root) {
    if (!root)
        return;
    removeTree(root->child[0]);
    removeTree(root->child[1]);
    delete root;
    root = 0;
}

template<class T>
bool AVLTree2<T>::insertToTree(Node *&root, const T &data) {
    if (!root)
        return (root = new Node(data));
    if (data == root->data)
        return false;
    int num = data < root->data ? 0 : 1;
    Node *&next = root->child[num];
    short high = next ? next->high : 0;
    if (!insertToTree(next, data))
        return false;
    if (!high)
        return (root->high = 2);
    if (high == next->high || root->high > next->high)
        return true;
    if (getBF(next) * (num ? 1 : -1) < 0)
        rotate(next, !num);
    rotate(root, num);
    return true;
}

/* 找到要删除的节点后使他向下移动，最终成为叶节点再删除 */
template<class T>
bool AVLTree2<T>::removeFromTree(Node *&root, const T &data) {
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
    int num = root->data < data;
    Node *&next = root->child[num];
    if (!next)
        return false;
    if (!removeFromTree(next, data))
        return false;
    fixNode(root);
    return true;
}

/* 节点高度发生变化时（减少1）返回真。 */
template<class T>
bool AVLTree2<T>::fixNode(Node *&node) {
    if (!node)
        return false;
    int BF = getBF(node);
    if (BF < 2 && BF > -2) {
        short left = getHigh(node->child[0]);
        short right = getHigh(node->child[1]);
        node->high = left > right ? left + 1 : right + 1;
        return false;
    }
    int num = BF > 0;
    if (getBF(node->child[num]) * BF < 0)
        rotate(node->child[num], !num);
    rotate(node, num);
    return true;
}

template<class T>
bool AVLTree2<T>::rotate(Node *&node, bool left) {
    int num = left;
    Node *newRoot = node->child[num];
    if (!newRoot)
        throw std::runtime_error("没有可旋转的节点");
    short grand1 = getHigh(node->child[1 - num]);
    short grand2 = getHigh(newRoot->child[1 - num]);
    node->high = grand1 > grand2 ? grand1 + 1 : grand2 + 1;
    short son = getHigh(newRoot->child[num]);
    newRoot->high = node->high > son ? node->high + 1 : son + 1;
    node->child[num] = newRoot->child[1 - num];
    newRoot->child[1 - num] = node;
    node = newRoot;
    return true;
}

template<class T>
const typename AVLTree2<T>::Node *AVLTree2<T>::searchInTree(const Node *root,
                                                            const T &data) {
    if (root == 0)
        return 0;
    if (data == root->data)
        return root;
    if (data < root->data)
        return searchInTree(root->child[0], data);
    return searchInTree(root->child[1], data);
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

template<class T>
int AVLTree2<T>::checkHeight(Node *root, int &n) {
    if (!root)
        return 0;
    int left = checkHeight(root->child[0], n);
    int right = checkHeight(root->child[1], n);
    int bigger = left > right ? left : right;
    if (root->high != bigger + 1) {
        std::cerr << "error: " << root->data << " " << root->high << "\n";
        n++;
    }
    if (right - left > 1 || right - left < -1)
        n++;
    return root->high;
}

/* 一开始想的方法。找到要删除的点后，把它和左子树中最大的点交换，
 * 从而将问题转化成删除叶节点。 */
template<class T>
bool AVLTree2<T>::removeFromTree1(Node *&root, const T &data) {
    if (!root)
        return false;
    int num = root->data < data ? 1 : 0;
    if (data == root->data)
        removeNode(root, num);
    else if (!removeFromTree1(root->child[num], data))
        return false;
    fixNode(root);
    return true;
}

template<class T>
void AVLTree2<T>::removeNode(Node *&node, int &num) {
    if (!node)
        return;
    if (!node->child[0]) {
        Node *tem = node;
        if (!node->child[1]) {
            node = 0;
        } else {
            node = node->child[1];
            node->high--;
        }
        delete tem;
        num = 1;
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
