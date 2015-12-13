/*
 * main.cpp
 *
 *  Created on: 2015Äê12ÔÂ12ÈÕ
 *      Author: Sine
 */

#include "RedBlackTree.h"
#include <cstdio>
#include <cstdlib>

int main() {
    Sine::RedBlackTree<int> tree;
    for (int i = 0; i < 1000; i++) {
        tree.insert(rand());
    }
    tree.inorder(visit);
}
