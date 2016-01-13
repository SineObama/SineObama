/*
 * AVLTree.h
 *
 *  Created on: 2016年1月9日
 *      Author: Sine
 */

// todo 2016年1月9日21:42:43 刚刚写完，只做了简单测试，思路上不知道为什么西西里过不了
#ifndef AVLTREE_H_
#define AVLTREE_H_

#include <vector>
#include <string>
#include <sstream>

namespace Sine {

template<class T>
class AVLTree {

 public:

    struct Node {
        T data;
        Node *child[2];  // 0 for left, the smaller, 1 for right
        int BF;
        Node(const T &d)
                : data(d) {
            child[0] = child[1] = 0;
            BF = 0;
        }
    };
    AVLTree();
    AVLTree(const AVLTree &);
    ~AVLTree();

    bool insert(const T &);
    bool remove(const T &);

    const Node *search(const T &) const;

    void inorder(void (*)(const T &)) const;

    void print() const;

    int validator() const;

 protected:

    void recursiveInorder(Node *, void (*)(const T &)) const;

 private:

    static void recursiveCopy(Node *&, const Node *);
    static void removeTree(Node *&);

    static bool insertToTree(Node *&, const T &);
    static bool removeFromTree(Node *&, const T &);

    /* helper function*/
    static bool fixNode(Node *&);
    static bool rotate(Node *&, bool left);

    static const Node *searchInTree(const Node *, const T &);

    static void printTree(const Node *, std::string);
    static void inorderPrint(std::vector<std::stringstream *> &, const Node *,
                             int);

    static int getHeightAndCheck(Node *, int &);

    /* another longer way to remove node */
    static bool removeFromTree1(Node *&, const T &);
    static void removeNode(Node *&, int &);
    static Node *removeTheBiggest(Node *&);
    static void fixNodeAfterRemove(Node *&, int source, int num);
    static bool fixNode1(Node *&);

    Node *_root;

};

}

#include "AVLTreeTemplate.h"

#endif /* AVLTREE_H_ */
