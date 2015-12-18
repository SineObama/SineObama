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
    RedBlackTreeNode<Entry> *&operator[](int);
};

template<class Entry>
class RedBlackTree {

    typedef RedBlackTreeNode<Entry> * NodePtr;
    typedef RedBlackTreeNode<Entry> Node;
    typedef const RedBlackTreeNode<Entry> * constPtr;

 public:

    RedBlackTree();
    ~RedBlackTree();

    bool insert(const Entry &, bool recursive = true);
    bool remove(const Entry &, bool recursive = true);
    void clear();

    void preorder(void (*)(const Entry *));
    void inorder(void (*)(const Entry *));
    void postorder(void (*)(const Entry *));
    void preorder(void (*)(constPtr));
    template<class T>
    void inorder(T (*)(constPtr));
    void postorder(void (*)(constPtr));

    int testRedTheory();
    int testBlackTheory();

 protected:

    static void recursivePreorder(NodePtr, void (*)(const Entry *));
    static void recursiveInorder(NodePtr, void (*)(const Entry *));
    static void recursivePostorder(NodePtr, void (*)(const Entry *));
    static void recursivePreorder(NodePtr, void (*)(constPtr));
    template<class T>
    static void recursiveInorder(NodePtr, T (*)(constPtr));
    static void recursivePostorder(NodePtr, void (*)(constPtr));

    bool recursiveInsert(const Entry &);
    bool nonRecursiveInsert(const Entry &);

    bool recursiveRemove(const Entry &);
    bool nonRecursiveRemove(const Entry &);

 private:

    static NodePtr rotateForInsert(NodePtr, int, int);
    static bool rotate(NodePtr &, int);
    static bool insertToNodeAndFix(NodePtr &, const Entry &);
    static bool removeFromNodeAndFix(NodePtr &, const Entry &, bool &);

    static bool tryReduceBlack(NodePtr &);

    void inorderTestForRedTheory(NodePtr, int &);
    void inorderTestForBlackTheory(NodePtr, int &, int &, int &);

    NodePtr root;

};

}

#include "RedBlackTreeTemplate.h"

#endif /* REDBLACKTREE_H_ */
