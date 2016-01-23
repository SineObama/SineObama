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
    _root = 0;
}

template<class T>
AVLTree<T>::AVLTree(const AVLTree &x) {
    recursiveCopy(_root, x._root);
}

template<class T>
AVLTree<T>::~AVLTree() {
    recursiveRemoveTree(_root);
}

template<class T>
bool AVLTree<T>::insert(const T &data) {
    return insertToTree(_root, data);
}

template<class T>
bool AVLTree<T>::remove(const T &data) {
    return removeFromTree(_root, data);
}

template<class T>
const typename AVLTree<T>::Node *AVLTree<T>::search(const T &data) const {
    return searchInTree(_root, data);
}

template<class T>
void AVLTree<T>::inorder(void (*visit)(const T &)) const {
    recursiveInorder(_root, visit);
}

template<class T>
void AVLTree<T>::print() const {
    printTree(_root);
}

template<class T>
int AVLTree<T>::validator() const {
    int num = 0;
    getHeightAndCheck(_root, num);
    return num;
}

template<class T>
void AVLTree<T>::recursiveInorder(Node *root, void (*visit)(const T &)) const {
    if (!root)
        return;
    recursiveInorder(root->child[0], visit);
    (*visit)(root->data);
    recursiveInorder(root->child[1], visit);
}

template<class T>
void AVLTree<T>::recursiveCopy(Node *&root, const Node *other) {
    if (!other) {
        root = 0;
    } else {
        root = new Node(other->data);
        root->BF = other->BF;
        recursiveCopy(root->child[0], other->child[0]);
        recursiveCopy(root->child[1], other->child[1]);
    }
}

template<class T>
void AVLTree<T>::recursiveRemoveTree(Node *&root) {
    if (!root)
        return;
    recursiveRemoveTree(root->child[0]);
    recursiveRemoveTree(root->child[1]);
    delete root;
    root = 0;
}

template<class T>
bool AVLTree<T>::insertToTree(Node *&root, const T &data) {
    if (!root)
        return (root = new Node(data));
    if (data == root->data)
        return false;
    Node *&next = data < root->data ? root->child[0] : root->child[1];
    int source = next ? next->BF : 2;  // 子树原来的因子，2代表新节点
    if (!insertToTree(next, data))
        return false;
    // 插入到子树成功
    if (source != 2 && (next->BF == 0 || next->BF == source))  // 节点非新节点时，插入之后子树因子为0，或不变，说明子树高度没有变化
        return true;
    // 已知子树高度+1，修正本节点
    root->BF += data < root->data ? -1 : 1;
    fixNode(root);
    return true;
}

/* 找到要删除的节点后使他向下移动，最终成为叶节点再删除。
 * 如何通过因子判断出子树的高度变化真是个大问题。。。 */
template<class T>
bool AVLTree<T>::removeFromTree(Node *&root, const T &data) {
    if (!root)
        return false;
    int symbol = 2;
    if (root->data == data) {  // 尝试使当前节点下移
        if (root->BF == 0 && !root->child[0]) {  // 没有子树了
            delete root;
            root = 0;
            return true;
        }
        rotate(root, (symbol = root->BF > 0));
    }
    int num = root->data < data;
    Node *&next = root->child[num];
    if (!next)
        return false;
    if (fixNode(next))
        root->BF -= num * 2 - 1;
    int source = next->BF;
    if (!removeFromTree(next, data))
        return false;
    // 删除节点成功
    if (!next || (next->BF == 0 && next->BF != source))  // 子树被删除，或者子树因子从非0变为0，说明子树高度-1
        root->BF -= num * 2 - 1;
    fixNode(root);
    return true;
}

/* 节点高度发生变化时（减少1）返回真。 */
template<class T>
bool AVLTree<T>::fixNode(Node *&node) {
    if (!node)
        return false;
    int num = 0;
    if (node->BF > 1)
        num = 1;
    else if (node->BF > -2)
        return false;
    if (node->child[num]->BF * (num * 2 - 1) < 0)
        rotate(node->child[num], !num);
    bool result = node->BF * node->child[num]->BF > 0;
    rotate(node, num);
    return result;
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
const typename AVLTree<T>::Node *AVLTree<T>::searchInTree(const Node *root,
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
void AVLTree<T>::printTree(const Node *root, std::string s = "") {
    std::vector<std::stringstream *> v;
    inorderPrint(v, root, 0);
    std::cout << s << std::endl;
    for (unsigned int i = 0; i < v.size(); i++) {
        std::cout << v[i]->str() << std::endl;
        delete v[i];
    }
}

template<class T>
void AVLTree<T>::inorderPrint(std::vector<std::stringstream *> &v,
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
int AVLTree<T>::getHeightAndCheck(Node *root, int &n) {
    if (!root)
        return 0;
    int left = getHeightAndCheck(root->child[0], n);
    int right = getHeightAndCheck(root->child[1], n);
    int height = (left > right ? left : right) + 1;
    if (right - left != root->BF || (root->BF > 1 || root->BF < -1)) {
        n++;
        std::cout << "error! data: " << root->data << " height: " << height
                << " BF: " << root->BF << "  " << right - left << std::endl;
    }
    return height;
}

/* 一开始想的方法。找到要删除的点后，把它和左子树中最大的点交换，
 * 从而将问题转化成删除叶节点。 */
template<class T>
bool AVLTree<T>::removeFromTree1(Node *&root, const T &data) {
    if (!root)
        return false;
    int num = root->data < data ? 1 : 0;
    int source = root->child[num] ? root->child[num]->BF : 0;  // 子树原来的因子
    if (data == root->data)
        removeNode(root, num);
    else if (!removeFromTree1(root->child[num], data))
        return false;
    // 删除节点成功
    fixNodeAfterRemove(root, source, num);
    return true;
}

template<class T>
void AVLTree<T>::removeNode(Node *&node, int &num) {
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
        num = 1;
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
typename AVLTree<T>::Node *AVLTree<T>::removeTheBiggest(Node *&root) {
    if (!root)
        throw std::runtime_error("节点不存在");
    Node *&next = root->child[1];
    if (!next) {
        Node *newNode = root->child[0], *back = root;
        root->child[0] = 0;
        root = newNode;
        return back;
    }
    int source = next->BF;
    Node *back = removeTheBiggest(next);
    // 删除节点成功
    fixNodeAfterRemove(root, source, 1);
    return back;
}

template<class T>
void AVLTree<T>::fixNodeAfterRemove(Node *&node, int source, int num) {
    if (!node)
        return;
    Node *next = node->child[num];
    if (next && (next->BF != 0 || next->BF == source))  // 子树没变删除时，因子不为0，或者不变，则子树高度不变
        return;
    // 子树高度-1
    node->BF += num ? -1 : 1;
    fixNode1(node);
}

template<class T>
bool AVLTree<T>::fixNode1(Node *&node) {
    if (!node)
        return false;
    int num = 0;
    if (node->BF > 1)
        num = 1;
    else if (node->BF > -2)
        return false;
    if (node->child[num]->BF * (num * 2 - 1) < 0)
        rotate(node->child[num], !num);
    rotate(node, num);
    return true;
}

}

#endif /* AVLTREETEMPLATE_H_ */
