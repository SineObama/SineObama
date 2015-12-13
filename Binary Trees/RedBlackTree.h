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

    bool insert(const Entry &, bool recursive = true);
    bool remove(const Entry &, bool recursive = true);
    void clear();

    void preorder(void (*)(Entry *));
    void inorder(void (*)(Entry *));
    void postorder(void (*)(Entry *));

 protected:

    static void recursivePreorder(NodePtr, void (*)(Entry *));
    static void recursiveInorder(NodePtr, void (*)(Entry *));
    static void recursivePostorder(NodePtr, void (*)(Entry *));

    bool recursiveInsert(const Entry &);
    bool nonRecursiveInsert(const Entry &);

    bool recursiveRemove(const Entry &);
    bool nonRecursiveRemove(const Entry &);

 private:

    static NodePtr rotate(NodePtr, int);
    static bool insertToNodeAndFix(NodePtr &, const Entry &);
    static bool removeFromNodeAndFix(NodePtr &, const Entry &);

    NodePtr root;

};

}

#include "RedBlackTreeTemplate.h"

#endif /* REDBLACKTREE_H_ */
