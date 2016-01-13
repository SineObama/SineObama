/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#include "RedBlackTree.h"
#include "SimpleTiming.h"
#include "AVLTree.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <ctime>
using namespace std;

#define SIZE 1000

void print(const int &a) {
    cout << ' ' << a;
}

int main() {
    {  // AVLÊ÷²âÊÔ´úÂë
        Sine::AVLTree<int> t;
        srand(time(NULL));
        int ar[10000] = { };
        int n = 2000, i;
        for (int i = 0; i < n; i++) {
            ar[i] = rand() % (n * 2);
            cerr << ',' << ar[i];
            t.insert(ar[i]);
        }
        cout << "error: " << t.validator() << endl;
        for (int i = 0; i < 2 * n; i++)
            t.remove(rand() % (n * 2));
        cout << "error: " << t.validator() << endl;
        for (i = 0; i < n; i++)
            t.remove(ar[i]);
        cout << '\n';
        t.print();
        cout << '\n';
        cout << "error: " << t.validator() << endl;

        t.remove(ar[i]);

        cout << '\n';
        t.print();
        cout << '\n';
        cout << "error: " << t.validator() << endl;

        t.inorder(print);
    }

    {  // ºìºÚÊ÷²âÊÔ´úÂë
//        srand(time(NULL));
//        Sine::RedBlackTree<int> tree, tree2;
//        for (int i = 0; i < SIZE; i++)
//            tree.insert(i);
//        std::cerr << tree.testBlackTheory() << " black error(s)\n";
//        std::cerr << tree.testRedTheory() << " red error(s)\n";
//        tree.inorder(visitIntNode);
//        std::cerr << visitIntNode(NULL);
    }
}
