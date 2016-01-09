/*
 * AVLTreeTemplate.h
 *
 *  Created on: 2016年1月9日
 *      Author: Sine
 */

#ifndef AVLTREETEMPLATE_H_
#define AVLTREETEMPLATE_H_

#include "AVLTree.h"
#include <stdexcept>

namespace Sine {

template<class T>
AVLTree<T>::AVLTree() {
    root = 0;
}

template<class T>
AVLTree<T>::~AVLTree() {
    delete root;
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
void AVLTree<T>::recursiveInorder(Node *node, void (*visit)(const T &)) const {
    if (!node)
        return;
    recursiveInorder(node->child[0], visit);
    (*visit)(node->data);
    recursiveInorder(node->child[1], visit);
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
    if (next->BF == 0)
        return true;
    if (next->BF == source)
        return true;

    // 已知子树高度+1，处理本节点
    int symbol = data < node->data ? -1 : 1;  // 为了合并左右子树这两种情况
    node->BF += symbol;
    if (node->BF > -2 && node->BF < 2)  // 树依然平衡
        return true;
    // 需要旋转
    if (next->BF == -symbol)
        rotate(next, symbol == -1);
    rotate(node, symbol == 1);
    return true;
}

template<class T>
bool AVLTree<T>::removeFromNode(Node *&node, const T &data) {
    if (!node)
        return false;
    if (data == node->data) {
        if (!node->child[0]) {
            if (!node->child[1]) {
                delete node;
                node = 0;
            } else {
                node->data = node->child[1]->data;
                node->BF = 0;
                delete node->child[1];
                node->child[1] = 0;
            }
            return true;
        }
        node->data = removeTheBiggest(node->child[0]);
        return true;
    }
    int num = data < node->data ? 0 : 1, symbol = num ? -1 : 1;
    Node *&next = node->child[num], *&another = node->child[1 - num];
    int source = next->BF;  // 子树原来的因子
    if (!removeFromNode(next, data))
        return false;
    // 删除节点成功

    if (!next) {  // 子树被删除
        node->BF += symbol;
        if (node->BF > -2 && node->BF < 2)
            return true;
        // 需要旋转
        if (another->child[num])
            rotate(another, num);
        rotate(node, !num);
        return true;
    }
    // 子树没变删除
    // 子树因子不为0，或者不变，则子树高度不变
    if (next->BF != 0)
        return true;
    if (next->BF == source)
        return true;

    // 已知子树高度-1
    node->BF += symbol;
    if (node->BF > -2 && node->BF < 2)
        return true;
    // 需要旋转
    if (next->BF == -symbol)
        rotate(next, symbol == -1);
    rotate(node, symbol == 1);
    return true;
}

template<class T>
T AVLTree<T>::removeTheBiggest(Node *&node) {
    if (!node)
        throw std::runtime_error("节点不存在");
    Node *&next = node->child[1], *&another = node->child[0];
    if (!next) {
        Node *newNode = node->child[0];
        T data = node->data;
        delete node;
        node = newNode;
        return data;
    }
    int source = next->BF;
    T data = removeTheBiggest(next);
    // 删除节点成功

    int symbol = -1;
    if (!next) {  // 子树被删除
        node->BF += symbol;
        if (node->BF > -2 && node->BF < 2)
            return data;
        // 需要旋转
        if (another->child[0] && another->child[1]) {
            node->BF = 1;
            another->BF = -1;
        } else {
            node->BF = 0;
            another->BF = 0;
            if (!another->child[1])
                rotate(another, false);
        }
        rotate(node, true);
        return data;
    }
    // 子树没变删除
    // 子树因子不为0，或者不变，则子树高度不变
    if (next->BF != 0)
        return data;
    if (next->BF == source)
        return data;

    // 已知子树高度-1
    node->BF += symbol;
    if (node->BF > -2 && node->BF < 2)
        return data;
    // 需要旋转
    if (next->BF == -symbol)
        rotate(next, symbol == -1);
    rotate(node, symbol == 1);

    return data;
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

// 只接受节点因子是-2 ~ 2，且旋转后只能是平衡的
template<class T>
bool AVLTree<T>::rotate(Node *&node, bool left) {
    int num = left, symbol = left ? 1 : -1;
    if (node->BF > 2 || node->BF < -2)
        throw std::runtime_error("因子不合法");
    if (node->BF == 0)
        throw std::runtime_error("因子为0不能被旋转");
    Node *newRoot = node->child[num];
    if (!newRoot)
        throw std::runtime_error("没有可旋转的节点");
    if (node->BF * newRoot->BF < 0)
        throw std::runtime_error("旋转后不平衡");
//    if (node->BF == 1 || node->BF == -1) {
//        node->BF = newRoot->BF == symbol ? -symbol : 0;
//        newRoot->BF = -symbol;
//    } else {  // node->BF == 2 || node->BF == -2
//        if (newRoot->BF == symbol) {
//            newRoot->BF = 0;
//            node->BF = 0;
//        } else {  // newRoot->BF == 0
//            newRoot->BF = -symbol;
//            node->BF = symbol;
//        }
//    }
    node->BF = node->BF - newRoot->BF - symbol;
    int tem = node->BF + newRoot->BF - symbol;
    if (newRoot->BF * symbol > tem * symbol)
        newRoot->BF = tem;
    newRoot->BF -= symbol;
    node->child[num] = newRoot->child[1 - num];
    newRoot->child[1 - num] = node;
    node = newRoot;
    return true;
}

}

#endif /* AVLTREETEMPLATE_H_ */
