/*
 * AVLTreeTemplate.h
 *
 *  Created on: 2016年1月9日
 *      Author: Sine
 */

#ifndef AVLTREETEMPLATE_H_
#define AVLTREETEMPLATE_H_

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include "AVLTree.h"

namespace Sine {

template<class T>
AVLTree<T>::AVLTree() {
    root = 0;
}

template<class T>
AVLTree<T>::~AVLTree() {
    removeTree(root);
}

template<class T>
bool AVLTree<T>::insert(const T &data) {
    return insertToNode(root, data);
}

template<class T>
bool AVLTree<T>::remove(const T &data) {
    return removeFromNode(root, data);
}

template<class T>
const typename AVLTree<T>::Node *AVLTree<T>::search(const T &data) const {
    return searchInNode(root, data);
}

template<class T>
void AVLTree<T>::inorder(void (*visit)(const T &)) const {
    recursiveInorder(root, visit);
}

template<class T>
void AVLTree<T>::print() const {
    std::vector<std::stringstream *> v;
    inorderPrint(v, root, 0);
    for (unsigned int i = 0; i < v.size(); i++) {
        std::cout << v[i]->str() << std::endl;
        delete v[i];
    }
}

template<class T>
int AVLTree<T>::validator() const {
    int num = 0;
    getHeightAndCheck(root, num);
    return num;
}

template<class T>
void AVLTree<T>::recursiveInorder(Node *node, void (*visit)(const T &)) const {
    if (!node)
        return;
    recursiveInorder(node->child[0], visit);
    (*visit)(node->data);
    recursiveInorder(node->child[1], visit);
}

template<class T>
bool AVLTree<T>::fixNode(Node *&node) {
    if (!node)
        return false;
    if (node->BF > 1) {
        fixNode(node->child[1]);
        if (node->child[1]->BF < 0)
            rotate(node->child[1], false);
        rotate(node, true);
    } else if (node->BF < -1) {
        fixNode(node->child[0]);
        if (node->child[0]->BF > 0)
            rotate(node->child[0], true);
        rotate(node, false);
    } else {
        return false;
    }
    return true;
}

template<class T>
bool AVLTree<T>::insertToNode(Node *&node, const T &data) {
    if (!node)
        return (node = new Node(data));
    if (data == node->data)
        return false;
    Node *&next = data < node->data ? node->child[0] : node->child[1];
    int source = next ? next->BF : 0;  // 子树原来的因子

    bool isNewChild = !next;
    if (!insertToNode(next, data))
        return false;
    // 插入到子树成功

    if (isNewChild) {  // 子树为新节点
        node->BF += data < node->data ? -1 : 1;
        return true;
    }
    // 子树为非新节点
    // 插入之后子树因子为0，或不变，则子树高度不可能变化
    if (next->BF == 0 || next->BF == source)
        return true;

    // 已知子树高度+1，处理本节点
    node->BF += data < node->data ? -1 : 1;
    fixNode(node);
    return true;
}

template<class T>
bool AVLTree<T>::removeFromNode(Node *&node, const T &data) {
    if (!node)
        return false;
    int num = node->data < data ? 1 : 0;
    int source = node->child[num] ? node->child[num]->BF : 0;  // 子树原来的因子
    if (data == node->data) {
        if (!node->child[0])
            num = 1;
        removeNode(node);
    } else if (!removeFromNode(node->child[num], data)) {
        return false;
    }
    // 删除节点成功
    fixAfterRemove(node, source, num);
    return true;
}

template<class T>
typename AVLTree<T>::Node *AVLTree<T>::removeTheBiggest(Node *&node) {
    if (!node)
        throw std::runtime_error("节点不存在");
    Node *&next = node->child[1];
    if (!next) {
        Node *newNode = node->child[0], *back = node;
        node->child[0] = 0;
        node = newNode;
        return back;
    }
    int source = next->BF;
    Node *back = removeTheBiggest(next);
    // 删除节点成功
    fixAfterRemove(node, source, 1);
    return back;
}

template<class T>
const typename AVLTree<T>::Node *AVLTree<T>::searchInNode(const Node *node,
                                                          const T &data) {
    if (node == 0)
        return 0;
    if (data == node->data)
        return node;
    if (data < node->data)
        return searchInNode(node->child[0], data);
    return searchInNode(node->child[1], data);
}

// 精华的旋转啊。。。包含了因子的变化规律
template<class T>
bool AVLTree<T>::rotate(Node *&node, bool left) {
//    if (node->BF > 2 || node->BF < -2)
//        throw std::runtime_error("因子不合法");
//    if (node->BF * newRoot->BF < 0)
//        throw std::runtime_error("旋转后不平衡");
//    if (node->BF == 0)
//        throw std::runtime_error("因子为0不应该被旋转");
    int num = left, symbol = left ? 1 : -1;
    Node *newRoot = node->child[num];
    if (!newRoot)
        throw std::runtime_error("没有可旋转的节点");
    int tem = node->BF - symbol + (newRoot->BF * symbol < 0 ? newRoot->BF : 0);
    node->BF = node->BF - symbol - (newRoot->BF * symbol > 0 ? newRoot->BF : 0);
    if (newRoot->BF * symbol > tem * symbol)
        newRoot->BF = tem;
    newRoot->BF -= symbol;
    node->child[num] = newRoot->child[1 - num];
    newRoot->child[1 - num] = node;
    node = newRoot;
    return true;
}

template<class T>
void AVLTree<T>::inorderPrint(std::vector<std::stringstream *> &v, Node *node,
                              int n) {
    if (!node)
        return;
    if (v.size() <= n) {
        v.push_back(new std::stringstream());
    }
    inorderPrint(v, node->child[0], n + 1);
    (*v[n]) << ' ' << node->data;
    inorderPrint(v, node->child[1], n + 1);
}

template<class T>
int AVLTree<T>::getHeightAndCheck(Node *node, int &n) {
    if (!node)
        return 0;
    int left = getHeightAndCheck(node->child[0], n);
    int right = getHeightAndCheck(node->child[1], n);
    int height = (left > right ? left : right) + 1;
    if (right - left != node->BF || (node->BF > 1 || node->BF < -1)) {
        n++;
        std::cout << "error! data: " << node->data << " height: " << height
                << " BF: " << node->BF << "  " << right - left << std::endl;
    }
    return height;
}

template<class T>
void AVLTree<T>::fixAfterRemove(Node *&node, int source, int num) {
    if (!node)
        return;
    Node *next = node->child[num];
    if (next && (next->BF != 0 || next->BF == source))  // 子树没变删除时，因子不为0，或者不变，则子树高度不变
        return;
    // 子树高度-1
    node->BF += num ? -1 : 1;
    ;
    fixNode(node);
}

template<class T>
void AVLTree<T>::removeNode(Node *&node) {
    if (!node)
        return;
    if (!node->child[0]) {
        if (!node->child[1]) {
            delete node;
            node = 0;
        } else {
            Node *tem = node;
            node = node->child[1];
            node->BF = tem->BF;
            delete tem;
        }
        return;
    }
    Node *tem = removeTheBiggest(node->child[0]);
    tem->child[0] = node->child[0];
    tem->child[1] = node->child[1];
    tem->BF = node->BF;
    delete node;
    node = tem;
}

template<class T>
void AVLTree<T>::removeTree(Node *&root) {
    if (!root)
        return;
    removeTree(root->child[0]);
    removeTree(root->child[1]);
    delete root;
    root = 0;
}

}

#endif /* AVLTREETEMPLATE_H_ */
