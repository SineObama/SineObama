/*
 * RedBlackTreeTemplate.h
 *
 *  Created on: 2015Äê12ÔÂ13ÈÕ
 *      Author: Sine
 */

#ifndef REDBLACKTREETEMPLATE_H_
#define REDBLACKTREETEMPLATE_H_

#include "RedBlackTree.h"
#include <cstdlib>
#include <stack>
#include "SineDebug.h"

void visit(int *x) {
    printf("%d ", *x);
}

namespace Sine {

template<class Entry>
RedBlackTreeNode<Entry>::RedBlackTreeNode(const Entry &x, RedBlackColor c)
        : data(x),
          color(c) {
    child[1] = child[0] = NULL;
}

template<class Entry>
RedBlackTree<Entry>::RedBlackTree() {
    root = NULL;
}

template<class Entry>
RedBlackTree<Entry>::~RedBlackTree() {
    clear();
}

template<class Entry>
bool RedBlackTree<Entry>::insert(const Entry &x) {
    if (!root) {
        root = new Node(x, black);
        return true;
    }
    std::stack<NodePtr> ptrStack;
    std::stack<short> numStack;
    NodePtr cur = root;
    while (cur) {
        ptrStack.push(cur);
        if (cur->data == x)
            return false;
        int num = cur->data < x;
        cur = cur->child[num];
        numStack.push(num);
    }
    NodePtr newNode = new Node(x, red);
    ptrStack.top()->child[numStack.top()] = newNode;
    while (!ptrStack.empty()) {
        cur = ptrStack.top();
        short num = numStack.top();
        ptrStack.pop();
        numStack.pop();
        if (cur->color == red && cur->child[num]->color == red) {
            NodePtr parent = ptrStack.top();
            short parentNum = numStack.top();
            ptrStack.pop();
            numStack.pop();
            NodePtr brother = parent->child[1 - parentNum];
            if (brother == NULL || brother->color == black) {
                NodePtr newParent = NULL;
                if (cur->child[1 - parentNum]) {
                    newParent = cur->child[1 - parentNum];
                    cur->child[1 - parentNum] = newParent->child[parentNum];
                    newParent->child[parentNum] = cur;
                } else {
                    newParent = cur;
                }
                parent->child[parentNum] = newParent->child[1 - parentNum];
                newParent->child[1 - parentNum] = parent;
                parent->color = red;
                newParent->color = black;
                if (ptrStack.empty())
                    root = newParent;
                else
                    ptrStack.top()->child[numStack.top()] = newParent;
                break;
            } else {  // brother->color == red
                parent->color = red;
                brother->color = black;
                cur->color = black;
                if (!ptrStack.empty()) {
                    ptrStack.pop();
                    numStack.pop();
                }
            }
        }
    }
    root->color = black;
    return true;
}

template<class Entry>
bool RedBlackTree<Entry>::remove(const Entry &x) {
    return true;
}

template<class Entry>
void RedBlackTree<Entry>::clear() {
}

template<class Entry>
void RedBlackTree<Entry>::preorder(void (*visit)(Entry *)) {
    recursive_preorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::inorder(void (*visit)(Entry *)) {
    recursive_inorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::postorder(void (*visit)(Entry *)) {
    recursive_postorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursive_preorder(NodePtr x,
                                             void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    (*visit)(&x->data);
    recursive_preorder(x->child[0], visit);
    recursive_preorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursive_inorder(NodePtr x, void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    recursive_inorder(x->child[0], visit);
    (*visit)(&x->data);
    recursive_inorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursive_postorder(NodePtr x,
                                              void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    recursive_postorder(x->child[0], visit);
    recursive_postorder(x->child[1], visit);
    (*visit)(&x->data);
}

}

#endif /* REDBLACKTREETEMPLATE_H_ */
