/*
 * RedBlackTree.h
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#ifndef REDBLACKTREE_H_
#define REDBLACKTREE_H_

namespace Sine {

enum RedBlackColor {
    red,
    black
};

template<class Entry>
struct RedBlackTreeNode {
    Entry data;
//    RedBlackTreeNode<Entry> *parent;
    RedBlackTreeNode<Entry> *child[2];  // child[0] for left child, child[1] for right child
    RedBlackColor color;
    RedBlackTreeNode(const Entry &, RedBlackColor);
};

template<class Entry>
class RedBlackTree {

    typedef RedBlackTreeNode<Entry> * NodePtr;
    typedef RedBlackTreeNode<Entry>  Node;

 public:

    RedBlackTree();
    ~RedBlackTree();

    bool insert(const Entry &);
    bool remove(const Entry &);
    void clear();

    void preorder(void (*)(Entry *));
    void inorder(void (*)(Entry *));
    void postorder(void (*)(Entry *));

 protected:

    void recursivePreorder(NodePtr, void (*)(Entry *));
    void recursiveInorder(NodePtr, void (*)(Entry *));
    void recursivePostorder(NodePtr, void (*)(Entry *));

    NodePtr rotate(NodePtr, int);

 private:

    NodePtr root;

};

}

#include "RedBlackTreeTemplate.h"

#endif /* REDBLACKTREE_H_ */
