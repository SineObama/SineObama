/*
 * BinaryTree.h
 *
 *  Created on: 2016Äê2ÔÂ5ÈÕ
 *      Author: Sine
 */

#ifndef BINARYTREE_H_
#define BINARYTREE_H_

namespace Sine {

template<class Node>
class BinaryTree {

    typedef Node * ptr;
    typedef Node *& ptrRef;
    typedef ptrRef getChild(ptr);

 public:

    BinaryTree(getChild, getChild);
    ~BinaryTree();

    void preorder(void (*)(ptr));
    void inorder(void (*)(ptr));
    void postorder(void (*)(ptr));

    ptrRef getRoot();

 private:

    static void deleteTree(ptr);
    static void recursivePreorder(ptr, void (*)(ptr));
    static void recursiveInorder(ptr, void (*)(ptr));
    static void recursivePostorder(ptr, void (*)(ptr));

    Node _root;
    getChild _left;
    getChild _right;

};

}

#include "BinaryTreeTemplate.h"

#endif /* BINARYTREE_H_ */
