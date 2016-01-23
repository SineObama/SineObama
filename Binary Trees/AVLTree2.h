/*
 * AVLTree2.h
 *
 *  Created on: 2016年1月22日
 *      Author: Sine
 */

#ifndef AVLTREE2_H_
#define AVLTREE2_H_

#include <vector>
#include <string>
#include <sstream>

namespace Sine {

/* 节点保存高度的形式 */
template<class T>
class AVLTree2 {

 public:

    struct Node {
        T data;
        Node *child[2];  // 0 for left, the smaller, 1 for right
        short high;
        Node(const T &d);
    };
    AVLTree2();
    AVLTree2(const AVLTree2 &);
    ~AVLTree2();

    bool insert(const T &);
    bool remove(const T &);

    const Node *search(const T &) const;

    void inorder(void (*)(const T &)) const;

    void print() const;

    int validator() const;

    static int getBF(const Node *);
    static short getHigh(const Node *);

 protected:

    void recursiveInorder(Node *, void (*)(const T &)) const;

 private:

    static void recursiveCopy(Node *&, const Node *);
    static void recursiveRemoveTree(Node *&);

    static bool recursiveInsertToTree(Node *&, const T &);
    static bool recursiveRemoveFromTree(Node *&, const T &);

    /* helper function*/
    static void fixNode(Node *&);
    static void fixHigh(Node *&);
    static void rotate(Node *&, bool left);

    static const Node *searchInTree(const Node *, const T &);

    static void printTree(const Node *, std::string);
    static void inorderPrint(std::vector<std::stringstream *> &, const Node *,
                             int);

    static short checkHeight(Node *, int &);
    static short getHigherChild(Node *);

    /* another longer way to remove node */
    static bool recursiveRemoveFromTree1(Node *&, const T &);
    static void removeNode(Node *&);
    static Node *removeTheBiggest(Node *&);

    Node *_root;

};

}

#include "AVLTreeTemplate2.h"

#endif /* AVLTREE2_H_ */
