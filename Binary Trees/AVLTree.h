/*
 * AVLTree.h
 *
 *  Created on: 2016年1月9日
 *      Author: Sine
 */

// todo 2016年1月9日21:42:43 刚刚写完，只做了简单测试，思路上不知道为什么西西里过不了

#ifndef AVLTREE_H_
#define AVLTREE_H_

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
        ~Node() {
            delete child[0];
            delete child[1];
        }
    };
    AVLTree();
    ~AVLTree();
    bool insert(const T &);
    bool remove(const T &);
    const Node *search(const T &) const;
    void inorder(void (*)(const T &)) const;
 protected:
    void recursiveInorder(Node *, void (*)(const T &)) const;
 private:
    static bool insertToNode(Node *&, const T &);
    static bool removeFromNode(Node *&, const T &);
    static T removeTheBiggest(Node *&);
    static const Node *searchInNode(const Node *, const T &);
    static bool rotate(Node *&, bool left);
    Node *root;
};

}

#include "AVLTreeTemplate.h"

#endif /* AVLTREE_H_ */
