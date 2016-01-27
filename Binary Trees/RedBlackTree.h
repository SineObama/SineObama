/*
 * RedBlackTree.h
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#ifndef REDBLACKTREE_H_
#define REDBLACKTREE_H_

#include <vector>
#include <sstream>

namespace Sine {

template<class Entry>
class RedBlackTree {

 public:

    enum Color {
        red,
        black
    };

    struct Node {
        Entry data;
        Node *child[2];  // child[0] for left child, child[1] for right child
        Color color;
        Node(const Entry &, Color);
        Node *&operator[](int);
    };

    typedef Node * NodePtr;
    typedef const Node * constPtr;

    RedBlackTree();
    ~RedBlackTree();

    bool insert(const Entry &, bool recursive = true);
    bool remove(const Entry &, bool recursive = true);
    void clear();

    void preorder(void (*)(const Entry *));
    void inorder(void (*)(const Entry *));
    void postorder(void (*)(const Entry *));
    void preorder(void (*)(constPtr));
    void inorder(void (*)(constPtr));
    void postorder(void (*)(constPtr));

    int testRedTheory();
    int testBlackTheory();

    void print() const;
    void printTree(NodePtr) const;

 protected:

    static void recursivePreorder(NodePtr, void (*)(const Entry *));
    static void recursiveInorder(NodePtr, void (*)(const Entry *));
    static void recursivePostorder(NodePtr, void (*)(const Entry *));
    static void recursivePreorder(NodePtr, void (*)(constPtr));
    static void recursiveInorder(NodePtr, void (*)(constPtr));
    static void recursivePostorder(NodePtr, void (*)(constPtr));

    bool recursiveInsert(const Entry &);
    bool nonRecursiveInsert(const Entry &);

    bool recursiveRemove(const Entry &);
    bool nonRecursiveRemove(const Entry &);

 private:

    static NodePtr rotateForInsert(NodePtr, int, int);
    static void rotate(NodePtr &, bool);
    static bool insertToNodeAndFix(NodePtr &, const Entry &);
    static bool removeFromNodeAndFix(NodePtr &, const Entry &);

    static void replace(NodePtr &, NodePtr);
    static NodePtr removeBiggestAndFix(NodePtr &);
    static void fixLackBlack(NodePtr &, int);

    bool inorderTestForRedTheory(NodePtr, int &);
    int inorderTestForBlackTheory(NodePtr, int &);

    static void inorderPrint(std::vector<std::stringstream *> &, const Node *,
                             int);

    NodePtr root;

};

}

#include "RedBlackTreeTemplate.h"

#endif /* REDBLACKTREE_H_ */
