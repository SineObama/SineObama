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
        int ar[100] = { 6,1,11,3,5,12,0,8,4,2,9,10,7 };
        for (int i = 0; i < 13; i++) {
//            ar[i] = rand() % 200;
            cerr << ',' << ar[i];
            t.insert(ar[i]);
        }
        for (int i = 0; i < 13; i++) {
            t.remove(ar[i]);
        }
//        t.remove(3);
        cout << '\n';
        t.print();
        t.inorder(print);
        cout << '\n';
        cout << "error: " << t.validator();
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
