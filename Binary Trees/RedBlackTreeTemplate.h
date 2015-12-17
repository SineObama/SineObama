/*
 * RedBlackTreeTemplate.h
 *
 *  Created on: 2015年12月13日
 *      Author: Sine
 */

#ifndef REDBLACKTREETEMPLATE_H_
#define REDBLACKTREETEMPLATE_H_

#include "RedBlackTree.h"
#include <cstdlib>
#include <stack>
#include <stdexcept>
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
bool RedBlackTree<Entry>::insert(const Entry &x, bool recursive) {
    return recursive ? recursiveInsert(x) : nonRecursiveInsert(x);
}

template<class Entry>
bool RedBlackTree<Entry>::remove(const Entry &x, bool recursive) {
    if (recursive)
        return recursiveRemove(x);
    return nonRecursiveRemove(x);
}

template<class Entry>
void RedBlackTree<Entry>::clear() {
}

template<class Entry>
void RedBlackTree<Entry>::preorder(void (*visit)(Entry *)) {
    recursivePreorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::inorder(void (*visit)(Entry *)) {
    recursiveInorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::postorder(void (*visit)(Entry *)) {
    recursivePostorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursivePreorder(NodePtr x, void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    (*visit)(&x->data);
    recursivePreorder(x->child[0], visit);
    recursivePreorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursiveInorder(NodePtr x, void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    recursiveInorder(x->child[0], visit);
    (*visit)(&x->data);
    recursiveInorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursivePostorder(NodePtr x,
                                             void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    recursivePostorder(x->child[0], visit);
    recursivePostorder(x->child[1], visit);
    (*visit)(&x->data);
}

template<class Entry>
bool RedBlackTree<Entry>::recursiveInsert(const Entry &x) {
    if (!insertToNodeAndFix(root, x))
        return false;
    root->color = black;
    return true;
}

template<class Entry>
bool RedBlackTree<Entry>::nonRecursiveInsert(const Entry &x) {
    if (!root) {
        root = new Node(x, black);
        return true;
    }

    // insert and record the path
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

    // fix the tree
    // the number of black node is always guaranteed, to fix the continuous red
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
                parent->color = red;
                parent = rotateForInsert(parent, parentNum, num);
                parent->color = black;
                if (ptrStack.empty())
                    root = parent;
                else
                    ptrStack.top()->child[numStack.top()] = parent;
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
bool RedBlackTree<Entry>::recursiveRemove(const Entry &x) {
    root->color = red;
    if (!removeFromNodeAndFix(root, x))
        return false;
    root->color = black;
    return true;
}

template<class Entry>
bool RedBlackTree<Entry>::nonRecursiveRemove(const Entry &x) {
    return false;
}

template<class Entry>
void RedBlackTree<Entry>::fixRedTheory(NodePtr parent) {

}

template<class Entry>
typename RedBlackTree<Entry>::NodePtr RedBlackTree<Entry>::rotateForInsert(
        NodePtr parent, int parentNum, int num) {
    NodePtr newParent, cur = parent->child[parentNum];
    if (parentNum != num) {
        newParent = cur->child[1 - parentNum];
        cur->child[1 - parentNum] = newParent->child[parentNum];
        newParent->child[parentNum] = cur;
    } else {
        newParent = cur;
    }
    parent->child[parentNum] = newParent->child[1 - parentNum];
    newParent->child[1 - parentNum] = parent;
    return newParent;
}

template<class Entry>
bool RedBlackTree<Entry>::rotate(NodePtr &parent, int num) {
    if (!parent || !parent->child[num])
        return false;
    NodePtr cur = parent->child[num];
    parent->child[num] = cur->child[1 - num];
    cur->child[1 - num] = parent;
    parent = cur;
    return true;
}

template<class Entry>
bool RedBlackTree<Entry>::insertToNodeAndFix(NodePtr &parent, const Entry &x) {
    if (!parent) {
        parent = new Node(x, red);
        return true;
    }
    if (parent->data == x)
        return false;
    // divide into smaller problem
    int parentNum = parent->data < x;
    if (!insertToNodeAndFix(parent->child[parentNum], x))
        return false;
    // fix
    NodePtr cur = parent->child[parentNum], leftChild = cur->child[0],
            rightChild = cur->child[1];
    if (cur->color == red) {
        int num = -1;
        if (leftChild && leftChild->color == red)
            num = 0;
        else if (rightChild && rightChild->color == red)
            num = 1;
        if (num != -1) {
            NodePtr brother = parent->child[1 - parentNum];
            if (brother == NULL || brother->color == black) {
                parent->color = red;
                parent = rotateForInsert(parent, parentNum, num);
                parent->color = black;
            } else {  // brother->color == red
                parent->color = red;
                brother->color = black;
                cur->color = black;
            }
        }
    }
    return true;
}

/*
 * 初步思路：每进入一个子树(一层函数)之前，会把子树的根改成红色
 * (提前保存这个根的颜色。不保证子树根节点处的红定理)
 * 这个红色的根让子树在删除节点的时候有一定的“余地”可用。
 * 退出子树后根据子树根的颜色分情况处理。
 * 子树内部会认为自己是合理的，必须保证自己内部的黑定理，但不保证红定理(至少在根节点处)
 * 找到要删除的节点后(假定两层以内不会全为黑)，保证黑定理的条件下
 * 与可用的红节点进行交换，使要删除的节点向下移动。
 * 移动至“根节点”处进行删除，返回。
 */
template<class Entry>
bool RedBlackTree<Entry>::removeFromNodeAndFix(NodePtr &parent,
                                               const Entry &x) {
    dc(parent->color == red, "");
    if (parent->data == x) {  // be bound to return in here
        if (!parent->child[0] && !parent->child[1]) {  // reach bottom, no child
            delete parent;
            parent = NULL;
            return true;
        }
        // exchanging downstairs
        int num = 0;
        if (parent->child[0] && parent->child[0]->color == red) {
            num = 0;
        } else if (parent->child[1] && parent->child[1]->color == red) {
            num = 1;
        } else {  // todo hint double black ?
            dc(parent->child[0], "");
            dc(parent->child[1], "");
            dc(parent->child[0]->color == black, "");
            dc(parent->child[1]->color == black, "");
            num = 0;
            parent = black;
            parent->child[0]->color = red;
            parent->child[1]->color = red;
        }
        dc(rotate(parent, num), "rotate fail\n")
        dc(parent->child[1 - num]->data == x, "error after found x")
        return removeFromNodeAndFix(parent->child[1 - num], x);
    }
    int parentNum = parent->data < x;  // todo efficiency
    NodePtr &cur = parent->child[parentNum];
    if (!cur)  // searching fail (the bottom false return)
        return false;
    RedBlackColor src = cur->color;
    cur->color = red;  // cheat the subtree
    if (!removeFromNodeAndFix(cur, x))
        return false;
    if (cur == NULL) {  // a leaf node was deleted
        if (src == black) {
            parent->color = black;
            dc(parent->child[1 - parentNum]->color == black,
               "after delete, node illegal\n");
            parent->child[1 - parentNum]->color = red;
        }
        return true;
    }
    if (src == red)
        return true;
    if (cur->color == red && src == black)
        return cur->color = black || 1;  // kidding

    // src == black && cur->color == black, guarantees the parent have another child
    dc(src == black && cur->color == black, "");
    NodePtr brother = parent->child[1 - parentNum];
    if (brother->color == red) {
    } else {  // brother->color == black
        parent->color = black;
        brother->color = red;
    }
}

}

#endif /* REDBLACKTREETEMPLATE_H_ */
