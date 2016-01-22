/*
 * AVLTree2.h
 *
 *  Created on: 2016Äê1ÔÂ22ÈÕ
 *      Author: Sine
 */

#ifndef AVLTREE2_H_
#define AVLTREE2_H_

#include <vector>
#include <string>
#include <sstream>

namespace Sine {

template<class T>
class AVLTree2 {

 public:

    struct Node {
        T data;
        Node *child[2];  // 0 for left, the smaller, 1 for right
        short high;
        Node(const T &d)
                : data(d) {
            child[0] = child[1] = 0;
            high = 1;
        }
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

    static int checkHeight(Node *, int &);

    /* another longer way to remove node */
    static bool removeFromTree1(Node *&, const T &);
    static void removeNode(Node *&, int &);
    static Node *removeTheBiggest(Node *&);

    Node *_root;

};

}

#include "AVLTreeTemplate2.h"

#endif /* AVLTREE2_H_ */
