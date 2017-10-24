/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ctime>
#include <set>
#include <map>
#include "RedBlackTree.h"
#include "SimpleTiming.h"
#include "AVLTree2.h"
#include "BinaryTree.h"
#include "Counter.h"
#include "helpers.h"
#include "TestDataGenerator.h"

//template<class Entry>
//class Tree {
//    struct Node {
//        Node *child[2];
//        Entry data;
//        Node() {
//            child[0] = child[1] = NULL;
//        }
//    };
// public:
//    Tree()
//            : tree(left, right) {
//    }
// private:
//    static Node *&left(Node *n) {
//        return n->child[0];
//    }
//    static Node *&right(Node *n) {
//        return n->child[1];
//    }
//    Sine::BinaryTree<Node> tree;
//};

Sine::Counter counter;

int main() {

    {  // ºìºÚÊ÷²âÊÔ´úÂë
        srand(0);
        srand(time(NULL));
        Sine::RedBlackTree<int> tree;
        std::set<int> set;
        std::map<int, int> map;

        int n = 100000;
        Sine::RandomNatural data(n), data2(n);
//        Sine::RandomInt data(n, 0, n), data2(n, 0, n);

        std::cout << "--- insert " << n << " ---\n";

        int count = 0;
        counter.start();
        for (int i = 0; i < n; i++)
            count += tree.insert(data[i]);
        counter.stop();
        std::cout << "tree " << counter.time() << " second\n";
        std::cout << "tree size " << count << "\n";
        std::cout << tree.testBlackTheory() << " black error\n";
        std::cout << tree.testRedTheory() << " red error\n";

        counter.start();
        for (int i = 0; i < n; i++)
            set.insert(data[i]);
        counter.stop();
        std::cout << "set " << counter.time() << " second\n";

        counter.start();
        for (int i = 0; i < n; i++)
            map.insert(std::make_pair(data[i], 0));
        counter.stop();
        std::cout << "map " << counter.time() << " second\n";

        std::cout << "--- remove " << n << " ---\n";

        counter.start();
        for (int i = 0; i < n; i++)
            tree.remove(data2[i]);
        counter.stop();
        std::cout << "tree " << counter.time() << " second\n";
        std::cout << tree.testBlackTheory() << " black error\n";
        std::cout << tree.testRedTheory() << " red error\n";

        counter.start();
        for (int i = 0; i < n; i++)
            set.erase(data2[i]);
        counter.stop();
        std::cout << "set " << counter.time() << " second\n";

        counter.start();
        for (int i = 0; i < n; i++)
            map.erase(data2[i]);
        counter.stop();
        std::cout << "map " << counter.time() << " second\n";

//        tree.print();
    }

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

}
