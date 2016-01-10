/*
 * AVLTreeTemplate.h
 *
 *  Created on: 2016��1��9��
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
AVLTree<T>::~AVLTree() {
    removeTree(_root);
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
    std::vector<std::stringstream *> v;
    inorderPrint(v, _root, 0);
    for (unsigned int i = 0; i < v.size(); i++) {
        std::cout << v[i]->str() << std::endl;
        delete v[i];
    }
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
bool AVLTree<T>::insertToTree(Node *&root, const T &data) {
    if (!root)
        return (root = new Node(data));
    if (data == root->data)
        return false;
    Node *&next = data < root->data ? root->child[0] : root->child[1];
    int source = next ? next->BF : 0;  // ����ԭ��������

    bool isNewChild = !next;
    if (!insertToTree(next, data))
        return false;
    // ���뵽�����ɹ�

    if (isNewChild) {  // ����Ϊ�½ڵ�
        root->BF += data < root->data ? -1 : 1;
        return true;
    }
    // ����Ϊ���½ڵ�
    // ����֮����������Ϊ0���򲻱䣬�������߶Ȳ����ܱ仯
    if (next->BF == 0 || next->BF == source)
        return true;

    // ��֪�����߶�+1�������ڵ�
    root->BF += data < root->data ? -1 : 1;
    fixNode(root);
    return true;
}

template<class T>
bool AVLTree<T>::removeFromTree(Node *&root, const T &data) {
    if (!root)
        return false;
    int num = root->data < data ? 1 : 0;
    int source = root->child[num] ? root->child[num]->BF : 0;  // ����ԭ��������
    if (data == root->data)
        removeNode(root, num);
    else if (!removeFromTree(root->child[num], data))
        return false;
    // ɾ���ڵ�ɹ�
    fixNodeAfterRemove(root, source, num);
    return true;
}

template<class T>
typename AVLTree<T>::Node *AVLTree<T>::removeTheBiggest(Node *&root) {
    if (!root)
        throw std::runtime_error("�ڵ㲻����");
    Node *&next = root->child[1];
    if (!next) {
        Node *newNode = root->child[0], *back = root;
        root->child[0] = 0;
        root = newNode;
        return back;
    }
    int source = next->BF;
    Node *back = removeTheBiggest(next);
    // ɾ���ڵ�ɹ�
    fixNodeAfterRemove(root, source, 1);
    return back;
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

// ��������ת�����������������ӵı仯����
template<class T>
bool AVLTree<T>::rotate(Node *&node, bool left) {
//    if (node->BF > 2 || node->BF < -2)
//        throw std::runtime_error("���Ӳ��Ϸ�");
//    if (node->BF * newRoot->BF < 0)
//        throw std::runtime_error("��ת��ƽ��");
//    if (node->BF == 0)
//        throw std::runtime_error("����Ϊ0��Ӧ�ñ���ת");
    int num = left, symbol = left ? 1 : -1;
    Node *newRoot = node->child[num];
    if (!newRoot)
        throw std::runtime_error("û�п���ת�Ľڵ�");
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
void AVLTree<T>::inorderPrint(std::vector<std::stringstream *> &v, Node *root,
                              int n) {
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

template<class T>
void AVLTree<T>::fixNodeAfterRemove(Node *&node, int source, int num) {
    if (!node)
        return;
    Node *next = node->child[num];
    if (next && (next->BF != 0 || next->BF == source))  // ����û��ɾ��ʱ�����Ӳ�Ϊ0�����߲��䣬�������߶Ȳ���
        return;
    // �����߶�-1
    node->BF += num ? -1 : 1;
    fixNode(node);
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
