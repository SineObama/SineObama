/*
 * AVLTree.h
 *
 *  Created on: 2016��1��9��
 *      Author: Sine
 */

// todo 2016��1��9��21:42:43 �ո�д�ֻ꣬���˼򵥲��ԣ�˼·�ϲ�֪��Ϊʲô�����������
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
    static bool fixNode(Node *&);
    static bool fixNode2(Node *&);
 private:
    AVLTree(const Node *);
    static bool insertToTree(Node *&, const T &);
    static bool removeFromTree(Node *&, const T &);
    static bool removeFromTree2(Node *&, const T &);
    static Node *removeTheBiggest(Node *&);
    static const Node *searchInTree(const Node *, const T &);
    static bool rotate(Node *&, bool left);
    static void inorderPrint(std::vector<std::stringstream *> &, const Node *, int);
    static int getHeightAndCheck(Node *, int &);
    static void fixNodeAfterRemove(Node *&, int source, int num);
    static void removeNode(Node *&, int &);
    static void removeTree(Node *&);
    static void recursiveCopy(Node *&, const Node *);
    static void print(const Node *, std::string);
    Node *_root;
};

}

#include "AVLTreeTemplate.h"

#endif /* AVLTREE_H_ */
