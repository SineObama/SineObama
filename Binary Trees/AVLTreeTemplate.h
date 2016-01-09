/*
 * AVLTreeTemplate.h
 *
 *  Created on: 2016��1��9��
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
    int source = next ? next->BF : 0;  // ����ԭ��������

    bool isNewChild = !next;
    if (!insertToNode(next, data))
        return false;
    // ���뵽�����ɹ�

    if (isNewChild) {  // ����Ϊ�½ڵ�
        node->BF += data < node->data ? -1 : 1;
        return true;
    }
    // ����Ϊ���½ڵ�
    // ����֮����������Ϊ0���򲻱䣬�������߶Ȳ����ܱ仯
    if (next->BF == 0)
        return true;
    if (next->BF == source)
        return true;

    // ��֪�����߶�+1�������ڵ�
    int symbol = data < node->data ? -1 : 1;  // Ϊ�˺ϲ������������������
    node->BF += symbol;
    if (node->BF > -2 && node->BF < 2)  // ����Ȼƽ��
        return true;
    // ��Ҫ��ת
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
    int source = next->BF;  // ����ԭ��������
    if (!removeFromNode(next, data))
        return false;
    // ɾ���ڵ�ɹ�

    if (!next) {  // ������ɾ��
        node->BF += symbol;
        if (node->BF > -2 && node->BF < 2)
            return true;
        // ��Ҫ��ת
        if (another->child[num])
            rotate(another, num);
        rotate(node, !num);
        return true;
    }
    // ����û��ɾ��
    // �������Ӳ�Ϊ0�����߲��䣬�������߶Ȳ���
    if (next->BF != 0)
        return true;
    if (next->BF == source)
        return true;

    // ��֪�����߶�-1
    node->BF += symbol;
    if (node->BF > -2 && node->BF < 2)
        return true;
    // ��Ҫ��ת
    if (next->BF == -symbol)
        rotate(next, symbol == -1);
    rotate(node, symbol == 1);
    return true;
}

template<class T>
T AVLTree<T>::removeTheBiggest(Node *&node) {
    if (!node)
        throw std::runtime_error("�ڵ㲻����");
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
    // ɾ���ڵ�ɹ�

    int symbol = -1;
    if (!next) {  // ������ɾ��
        node->BF += symbol;
        if (node->BF > -2 && node->BF < 2)
            return data;
        // ��Ҫ��ת
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
    // ����û��ɾ��
    // �������Ӳ�Ϊ0�����߲��䣬�������߶Ȳ���
    if (next->BF != 0)
        return data;
    if (next->BF == source)
        return data;

    // ��֪�����߶�-1
    node->BF += symbol;
    if (node->BF > -2 && node->BF < 2)
        return data;
    // ��Ҫ��ת
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

// ֻ���ܽڵ�������-2 ~ 2������ת��ֻ����ƽ���
template<class T>
bool AVLTree<T>::rotate(Node *&node, bool left) {
    int num = left, symbol = left ? 1 : -1;
    if (node->BF > 2 || node->BF < -2)
        throw std::runtime_error("���Ӳ��Ϸ�");
    if (node->BF == 0)
        throw std::runtime_error("����Ϊ0���ܱ���ת");
    Node *newRoot = node->child[num];
    if (!newRoot)
        throw std::runtime_error("û�п���ת�Ľڵ�");
    if (node->BF * newRoot->BF < 0)
        throw std::runtime_error("��ת��ƽ��");
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
