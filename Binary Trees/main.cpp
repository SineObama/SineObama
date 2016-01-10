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
//        int ar[100] = { 6,29,165,69,65,3,196,195,143,63,46,178,137,162,96,189,199,103,108,16,71,93,109,139,192,50,194,134,11,118 };
        int ar[100] = {100,113,141,79,73,160,4,84,26,193,134,142,156,171,143,121,187,5,33,117,58,154,155,93,140,101,51,186,74,166};
        for (int i = 0; i < 26; i++) {
//            ar[i] = rand() % 200;
            cerr << ',' << ar[i];
            t.insert(ar[i]);
        }
        for (int i = 0; i < 10; i++) {
            t.remove(ar[i]);
        }
//        t.remove(162);
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
