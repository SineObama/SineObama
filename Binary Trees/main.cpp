/*
 * main.cpp
 *
 *  Created on: 2015��12��12��
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
    {  // AVL�����Դ���
        Sine::AVLTree<int> t;
        srand(time(NULL));
        int ar[100] = {155,30,167,91,164,106,167,132,51,93,164,89,21,168,71,21,132,62,94,60,74,9,12,72,140,94};
        for (int i = 0; i < 30; i++) {
            ar[i] = rand() % 200;
            cerr << ',' << ar[i];
            t.insert(ar[i]);
        }
        for (int i = 0; i < 30; i++) {
            t.remove(ar[i]);
        }
//        t.remove(30);
        cout << '\n';
        t.print();
        t.inorder(print);
        cout << '\n';
        cout << "error: " << t.validator();
    }

    {  // ��������Դ���
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
