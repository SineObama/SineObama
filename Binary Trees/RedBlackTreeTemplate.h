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
#include <cassert>
#include <stdexcept>
#include "SineDebug.h"

void visitIntEntry(const int *x) {
    printf("%d ", *x);
}

//int visitIntNode(Sine::RedBlackTree<int>::constPtr x) {
//    static int count = 0;
//    if (x == NULL)
//        return count;
//    if (x->color == Sine::RedBlackTree<int>::black) {
//        if (x->child[0] && x->child[0]->color == Sine::RedBlackTree<int>::black)
//            if (x->child[1]
//                    && x->child[1]->color == Sine::RedBlackTree<int>::black)
//                count++;
//    }
//    return count;
//}

namespace Sine {

template<class Entry>
RedBlackTree<Entry>::Node::Node(const Entry &x, Color c)
        : data(x),
          color(c) {
    child[1] = child[0] = NULL;
}

template<class Entry>
typename RedBlackTree<Entry>::Node *&RedBlackTree<Entry>::Node::operator[](
        int index) {
    return child[index];
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
void RedBlackTree<Entry>::preorder(void (*visit)(const Entry *)) {
    recursivePreorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::inorder(void (*visit)(const Entry *)) {
    recursiveInorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::postorder(void (*visit)(const Entry *)) {
    recursivePostorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::preorder(void (*visit)(constPtr)) {
    recursivePreorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::inorder(void (*visit)(constPtr)) {
    recursiveInorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::postorder(void (*visit)(constPtr)) {
    recursivePostorder(root, visit);
}

template<class Entry>
int RedBlackTree<Entry>::testRedTheory() {
    int found = 0;
    inorderTestForRedTheory(root, found);
    return found;
}

template<class Entry>
int RedBlackTree<Entry>::testBlackTheory() {
    int found = 0;
    inorderTestForBlackTheory(root, found);
    return found;
}

template<class Entry>
void RedBlackTree<Entry>::print() const {
    printTree(root);
}

template<class Entry>
void RedBlackTree<Entry>::printTree(NodePtr _root) const {
    std::vector<std::stringstream *> v;
    inorderPrint(v, root, 0);
    for (unsigned int i = 0; i < v.size(); i++) {
        std::cout << v[i]->str() << std::endl;
        delete v[i];
    }
}

template<class Entry>
void RedBlackTree<Entry>::recursivePreorder(NodePtr x,
                                            void (*visit)(const Entry *)) {
    if (x == NULL)
        return;
    (*visit)(&x->data);
    recursivePreorder(x->child[0], visit);
    recursivePreorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursiveInorder(NodePtr x,
                                           void (*visit)(const Entry *)) {
    if (x == NULL)
        return;
    recursiveInorder(x->child[0], visit);
    (*visit)(&x->data);
    recursiveInorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursivePostorder(NodePtr x,
                                             void (*visit)(const Entry *)) {
    if (x == NULL)
        return;
    recursivePostorder(x->child[0], visit);
    recursivePostorder(x->child[1], visit);
    (*visit)(&x->data);
}

template<class Entry>
void RedBlackTree<Entry>::recursivePreorder(NodePtr x,
                                            void (*visit)(constPtr)) {
    if (x == NULL)
        return;
    (*visit)(x);
    recursivePreorder(x->child[0], visit);
    recursivePreorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursiveInorder(NodePtr x, void (*visit)(constPtr)) {
    if (x == NULL)
        return;
    recursiveInorder(x->child[0], visit);
    (*visit)(x);
    recursiveInorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursivePostorder(NodePtr x,
                                             void (*visit)(constPtr)) {
    if (x == NULL)
        return;
    recursivePostorder(x->child[0], visit);
    recursivePostorder(x->child[1], visit);
    (*visit)(x);
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
    if (!root)
        return (root = new Node(x, black));

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
    ptrStack.top()->child[numStack.top()] = new Node(x, red);

    // fix the tree
    // the number of black node is always guaranteed, to fix the red theory
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
            }
        }
    }
    root->color = black;
    return true;
}

template<class Entry>
bool RedBlackTree<Entry>::recursiveRemove(const Entry &x) {
    root && (root->color = red);
    bool result = removeFromNodeAndFix(root, x);
    root && (root->color = black);
    return result;
}

template<class Entry>
bool RedBlackTree<Entry>::nonRecursiveRemove(const Entry &x) {
    // todo
    return false;
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
void RedBlackTree<Entry>::rotate(NodePtr &parent, bool left) {
    int num = left;
    assert(parent && parent->child[num]);
    NodePtr cur = parent->child[num];
    parent->child[num] = cur->child[1 - num];
    cur->child[1 - num] = parent;
    parent = cur;
}

template<class Entry>
bool RedBlackTree<Entry>::insertToNodeAndFix(NodePtr &parent, const Entry &x) {
    if (!parent)
        return (parent = new Node(x, red));
    if (parent->data == x)
        return false;
    // divide into smaller problem
    int parentNum = parent->data < x;
    if (!insertToNodeAndFix(parent->child[parentNum], x))
        return false;
    // fix
    NodePtr cur = parent->child[parentNum], leftChild = cur->child[0],
            rightChild = cur->child[1];
    if (cur->color == black)
        return true;
    int num = -1;
    if (leftChild && leftChild->color == red)
        num = 0;
    else if (rightChild && rightChild->color == red)
        num = 1;
    if (num == -1)
        return true;
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
    return true;
}

/*
 * 思想：采用“替换左子树最大节点”的方法。首先把问题转化成删除叶子节点。
 * 现在主要问题在于删除黑节点及删除后引发的问题，就是黑节点不够，即“不平衡”。
 * ……
 * 大概思路：进入函数之前把根改成红色(保存原本的颜色)
 * 这个红色的根让子树在删除节点的时候可以周转一下。
 * 处理过程可能改变根的颜色，退出函数后根据根的颜色分情况处理。
 * 子树内部会认为自己是合理的，处理过程不保证根节点处的红定理
 * 找到要删除的节点后，以左子树最大节点代替，细节不说了。
 */
template<class Entry>
bool RedBlackTree<Entry>::removeFromNodeAndFix(NodePtr &parent,
                                               const Entry &x) {
    if (!parent)
        return false;
    assert(parent->color == red);
    int num;
    bool isBlack;
    if (parent->data == x) {
        if (!parent->child[0]) {
            NodePtr tem = parent;
            parent = parent->child[1];
            delete tem;
            return true;
        }
        num = 0;
        isBlack = parent->child[0]->color == black;
        parent->child[0]->color = red;
        replace(parent, getBiggestAndFix(parent->child[0]));
    } else {
        num = parent->data < x;
        if (!parent->child[num])
            return false;
        isBlack = parent->child[num]->color == black;
        parent->child[num]->color = red;
        if (!removeFromNodeAndFix(parent->child[num], x)) {
            parent->child[num]->color = isBlack ? black : red;
            return false;
        }
    }
    if (!isBlack)
        return true;
    if (parent->child[num] && parent->child[num]->color == red)
        parent->child[num]->color = black;
    else
        fixLackBlack(parent, num);
    return true;
}

template<class Entry>
void RedBlackTree<Entry>::replace(NodePtr &parent, NodePtr x) {
    x->child[0] = parent->child[0];
    x->child[1] = parent->child[1];
    x->color = parent->color;
    delete parent;
    parent = x;
}

template<class Entry>
typename RedBlackTree<Entry>::NodePtr RedBlackTree<Entry>::getBiggestAndFix(
        NodePtr &parent) {
    assert(parent->color == red);
    NodePtr back = NULL;
    NodePtr &child = parent->child[1];
    if (child) {
        bool isBlack = child->color == black;
        child->color = red;
        back = getBiggestAndFix(child);
        if (!isBlack)
            return back;
        if (child && child->color == red) {
            child->color = black;
            return back;
        }
        fixLackBlack(parent, 1);
        return back;
    }
    back = parent;
    parent = parent->child[0];
    return back;
}

template<class Entry>
void RedBlackTree<Entry>::fixLackBlack(NodePtr &parent, int num) {
    NodePtr &another = parent->child[1 - num];
    assert(another);
    assert(!parent->child[num] || parent->child[num]->color == black);
    if (another->color == red) {
        rotate(parent, !num);
        fixLackBlack(parent->child[num], num);
        return;
    }
    NodePtr &same = another->child[num], &nosame = another->child[1 - num];
    if (!same && !nosame) {
        another->color = red;
        parent->color = black;
        return;
    }
    if (!same || !nosame) {
        if (!nosame)
            rotate(another, num);
    } else {  // have two child
        if (same->color == black && nosame->color == black) {
            another->color = red;
            parent->color = black;
            return;
        }
        if (same->color == red && nosame->color == black) {
            another->color = red;
            same->color = black;
            rotate(another, num);
        }
    }
    another->color = red;
    another->child[1 - num]->color = black;
    parent->color = black;
    rotate(parent, !num);
}

template<class Entry>
bool RedBlackTree<Entry>::inorderTestForRedTheory(NodePtr cur, int &found) {
    if (!cur)
        return false;
    if (cur->color == black) {
        inorderTestForRedTheory(cur->child[0], found);
        inorderTestForRedTheory(cur->child[1], found);
        return false;
    }
    if (inorderTestForRedTheory(cur->child[0], found))
        found++;
    if (inorderTestForRedTheory(cur->child[1], found))
        found++;
    return true;
}

template<class Entry>
int RedBlackTree<Entry>::inorderTestForBlackTheory(NodePtr cur, int &found) {
    if (!cur)
        return 1;
    int count = inorderTestForBlackTheory(cur->child[0], found);
    int count2 = inorderTestForBlackTheory(cur->child[1], found);
    if (count != count2)
        found++;
    if (cur->color == black)
        count++;
    return count;
}

template<class Entry>
void RedBlackTree<Entry>::inorderPrint(std::vector<std::stringstream *> &v,
                                       const Node *_root, int n) {
    if (!_root)
        return;
    if (v.size() <= n) {
        v.push_back(new std::stringstream());
    }
    inorderPrint(v, _root->child[0], n + 1);
    (*v[n]) << ' ' << _root->data << (_root->color == red ? "r" : "");
    inorderPrint(v, _root->child[1], n + 1);
}

}

#endif /* REDBLACKTREETEMPLATE_H_ */
