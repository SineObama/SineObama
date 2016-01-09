/*
 * main.cpp
 *
 *  Created on: 2015年12月12日
 *      Author: Sine
 */

#include "RedBlackTree.h"
#include "SimpleTiming.h"
#include "AVLTree.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <iostream>
using namespace std;

#define SIZE 1000

void print(const int &a) {
    cout << ' ' << a;
}

int main() {
    Sine::AVLTree<int> t;
    t.insert(1);
    t.insert(3);
    t.insert(2);
    t.insert(-1);
    t.insert(-2);
    t.remove(2);
    t.remove(3);
    t.remove(1);
    t.remove(-1);
    t.remove(-2);
    t.inorder(print);

    // 红黑树测试代码
//    srand(time(NULL));
//    Sine::RedBlackTree<int> tree, tree2;
//    for (int i = 0; i < SIZE; i++)
//        tree.insert(i);
//    std::cerr << tree.testBlackTheory() << " black error(s)\n";
//    std::cerr << tree.testRedTheory() << " red error(s)\n";
//    tree.inorder(visitIntNode);
//    std::cerr << visitIntNode(NULL);
}
