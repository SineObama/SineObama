/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#include "RedBlackTree.h"
#include "SimpleTiming.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

#define SIZE 1000

int main() {
    srand(time(NULL));
    Sine::RedBlackTree<int> tree, tree2;
    for (int i = 0; i < SIZE; i++)
        tree.insert(i);
    std::cerr << tree.testBlackTheory() << " black error(s)\n";
    std::cerr << tree.testRedTheory() << " red error(s)\n";
    tree.inorder(visitIntNode);
    std::cerr << visitIntNode(NULL);
//    if (tree.testRedTheory())
//        std::cerr << "!!!!";
//    Sine::SimpleTiming recursive, nonRecursive;
//    recursive();
//    for (int i = 0; i < SIZE; i++)
//        tree.insert(rand());
//    recursive();
//    nonRecursive();
//    for (int i = 0; i < SIZE; i++)
//        tree2.insert(rand(), false);
//    nonRecursive();
//    printf("%d\n", recursive.get());
//    tree.inorder(visit);
//    printf("\n%d\n", nonRecursive.get());
//    tree2.inorder(visit);
}
