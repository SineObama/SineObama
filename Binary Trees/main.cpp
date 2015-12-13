/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#include "RedBlackTree.h"
#include <cstdio>
#include <cstdlib>
#include <ctime>

int main() {
    srand(time(NULL));
    Sine::RedBlackTree<int> tree;
    int count = 0;
    for (int i = 0; i < 5000; i++) {
        if (tree.insert(rand()))
            count++;
    }
    printf("%d\n", count);
    tree.inorder(visit);
}
