/*
 * BinaryTreeTemplate.h
 *
 *  Created on: 2016Äê2ÔÂ5ÈÕ
 *      Author: Sine
 */

#ifndef BINARYTREETEMPLATE_H_
#define BINARYTREETEMPLATE_H_

#include "BinaryTree.h"
#include <cstdlib>

namespace Sine {

template<class Node>
BinaryTree<Node>::BinaryTree(getChild left, getChild right) {
    _left = left;
    _right = right;
    _root = NULL;
}

template<class Node>
BinaryTree<Node>::~BinaryTree() {
    deleteTree(_root);
}

template<class Node>
void BinaryTree<Node>::preorder(void (*visit)(ptr)) {
    recursivePreorder(_root, visit);
}

template<class Node>
void BinaryTree<Node>::inorder(void (*visit)(ptr)) {
    recursiveInorder(_root, visit);
}

template<class Node>
void BinaryTree<Node>::postorder(void (*visit)(ptr)) {
    recursivePostorder(_root, visit);
}

template<class Node>
typename BinaryTree<Node>::ptrRef BinaryTree<Node>::getRoot() {
    return _root;
}

template<class Node>
void BinaryTree<Node>::deleteTree(ptr root) {
    if (!root)
        return;
    deleteTree(_left(root));
    deleteTree(_right(root));
    delete root;
}

template<class Node>
void BinaryTree<Node>::recursivePreorder(ptr root, void (*visit)(ptr)) {
    if (!root)
        return;
    visit(root);
    recursivePreorder(_left(root), visit);
    recursivePreorder(_right(root), visit);
}

template<class Node>
void BinaryTree<Node>::recursiveInorder(ptr root, void (*visit)(ptr)) {
    if (!root)
        return;
    recursiveInorder(_left(root), visit);
    visit(root);
    recursiveInorder(_right(root), visit);
}

template<class Node>
void BinaryTree<Node>::recursivePostorder(ptr root, void (*visit)(ptr)) {
    if (!root)
        return;
    recursivePostorder(_left(root), visit);
    recursivePostorder(_right(root), visit);
    visit(root);
}

}

#endif /* BINARYTREETEMPLATE_H_ */
