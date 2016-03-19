/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#include "RedBlackTree.h"
#include "SimpleTiming.h"
#include "AVLTree2.h"
#include "BinaryTree.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ctime>
using namespace std;

void print(const int &a) {
    cout << ' ' << a;
}

template<class Entry>
class Tree {
    struct Node {
        Node *child[2];
        Entry data;
        Node() {
            child[0] = child[1] = NULL;
        }
    };
 public:
    Tree()
            : tree(left, right) {
    }
 private:
    static Node *&left(Node *n) {
        return n->child[0];
    }
    static Node *&right(Node *n) {
        return n->child[1];
    }
    Sine::BinaryTree<Node> tree;
};

int main() {

//    {  // AVLÊ÷²âÊÔ´úÂë
//        Sine::AVLTree2<int> t;
//        srand(time(NULL));
//        int ar[100000] = {18,1,14,6,1,17,8,2,9,5 };
//        int n = 10000, i;
//        for (int i = 0; i < n; i++) {
//            ar[i] = rand() % (n * 2);
//            cerr << ',' << ar[i];
//            t.insert(ar[i]);
//        }
//        cout << "insert:error: " << t.validator() << endl;
//        for (int i = 0; i < 2 * n; i++)
//            t.remove(rand() % (n * 2));
//        cout << "random remove:error: " << t.validator() << endl;
//        for (i = 0; i < n; i++)
//            t.remove(ar[i]);
//        cout << '\n';
//        t.print();
//        cout << '\n';
//        cout << "all remove:error: " << t.validator() << endl;
//
////        t.remove(18);
//
//        cout << '\n';
//        t.print();
//        cout << '\n';
//        cout << "last:error: " << t.validator() << endl;
//
//        t.inorder(print);
//    }

//    {  // ºìºÚÊ÷²âÊÔ´úÂë
//        srand(time(NULL));
//        Sine::RedBlackTree<int> tree, tree2;
//        int n = 30000, ar[100000] = { 5, 3, 1, 5, 12, 3, 14, 9, 9, 3, 0 };
//        for (int i = 0; i < n; i++) {
//            ar[i] = rand() % (n * 2);
//            cerr << ',' << ar[i];
//        }
//        cerr << '\n';
//        for (int i = 0; i < n; i++) {
//            tree.insert(ar[i]);
//        }
//        cout << tree.testBlackTheory() << " black error(s)\n";
//        cout << tree.testRedTheory() << " red error(s)\n";
//        for (int i = 0; i < n; i++) {
//            tree.remove(ar[i]);
//        }
//        cout << tree.testBlackTheory() << " black error(s)\n";
//        cout << tree.testRedTheory() << " red error(s)\n";
//        tree.print();
//    }
}
