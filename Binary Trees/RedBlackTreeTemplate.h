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

void visitIntEntry(const int *x) {
    printf("%d ", *x);
}

int visitIntNode(const Sine::RedBlackTreeNode<int> *x) {
    static int count = 0;
    if (x == NULL)
        return count;
    if (x->color == Sine::black) {
        if (x->child[0] && x->child[0]->color == Sine::black)
            if (x->child[1] && x->child[1]->color == Sine::black)
                count++;
    }
    return count;
}

namespace Sine {

template<class Entry>
RedBlackTreeNode<Entry>::RedBlackTreeNode(const Entry &x, RedBlackColor c)
        : data(x),
          color(c) {
    child[1] = child[0] = NULL;
}

template<class Entry>
RedBlackTreeNode<Entry> *&RedBlackTreeNode<Entry>::operator[](int index) {
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
template<class T>
void RedBlackTree<Entry>::inorder(T (*visit)(constPtr)) {
    recursiveInorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::postorder(void (*visit)(constPtr)) {
    recursivePostorder(root, visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursivePreorder(NodePtr x, void (*visit)(const Entry *)) {
    if (x == NULL)
        return;
    (*visit)(&x->data);
    recursivePreorder(x->child[0], visit);
    recursivePreorder(x->child[1], visit);
}

template<class Entry>
void RedBlackTree<Entry>::recursiveInorder(NodePtr x, void (*visit)(const Entry *)) {
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
void RedBlackTree<Entry>::recursivePreorder(NodePtr x, void (*visit)(constPtr)) {
    if (x == NULL)
        return;
    (*visit)(x);
    recursivePreorder(x->child[0], visit);
    recursivePreorder(x->child[1], visit);
}

template<class Entry>
template<class T>
void RedBlackTree<Entry>::recursiveInorder(NodePtr x, T (*visit)(constPtr)) {
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
    bool hehe = false;
    if (!removeFromNodeAndFix(root, x, hehe))
        return false;
    root->color = black;
    return true;
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
 * 初步思路：每进入一个子树(一层函数)之前，会把子树的根改成红色
 * (提前保存这个根的颜色。不保证子树根节点处的红定理)
 * 这个红色的根让子树在删除节点的时候有一定的“余地”可用。
 * 退出子树后根据子树根的颜色分情况处理。
 * 子树内部会认为自己是合理的，必须保证自己内部的黑定理，但不保证红定理(至少在根节点处)
 * 找到要删除的节点后(假定两层以内不会全为黑)，保证黑定理的条件下
 * 与可用的红节点进行交换，使要删除的节点向下移动。
 * 移动至“根节点”处进行删除，返回。
 */
//template<class Entry>
//bool RedBlackTree<Entry>::removeFromNodeAndFix(NodePtr &parent,
//                                               const Entry &x) {
//    dc(parent->color == red, "");
//    if (parent->data == x) {  // be bound to return in here
//        if (!parent->child[0] && !parent->child[1]) {  // reach bottom, no child
//            delete parent;
//            parent = NULL;
//            return true;
//        }
//        // exchanging downstairs
//        int num = 0;
//        if (parent->child[0] && parent->child[0]->color == red) {
//            num = 0;
//        } else if (parent->child[1] && parent->child[1]->color == red) {
//            num = 1;
//        } else {  // todo hint double black ?
//            dc(parent->child[0], "");
//            dc(parent->child[1], "");
//            dc(parent->child[0]->color == black, "");
//            dc(parent->child[1]->color == black, "");
//            num = 0;
//            parent = black;
//            parent->child[0]->color = red;
//            parent->child[1]->color = red;
//        }
//        dc(rotate(parent, num), "rotate fail\n")
//        dc(parent->child[1 - num]->data == x, "error after found x")
//        return removeFromNodeAndFix(parent->child[1 - num], x);
//    }
//    int parentNum = parent->data < x;  // todo efficiency
//    NodePtr &cur = parent->child[parentNum];
//    if (!cur)  // searching fail (the bottom false return)
//        return false;
//    RedBlackColor src = cur->color;
//    cur->color = red;  // cheat the subtree
//    if (!removeFromNodeAndFix(cur, x))
//        return false;
//    if (cur == NULL) {  // a leaf node was deleted
//        if (src == black) {
//            parent->color = black;
//            dc(parent->child[1 - parentNum]->color == black,
//               "after delete, node illegal\n");
//            parent->child[1 - parentNum]->color = red;
//        }
//        return true;
//    }
//    if (src == red)
//        return true;
//    if (cur->color == red && src == black)
//        return cur->color = black || true;  // kidding
//
//    // src == black && cur->color == black, guarantees the parent have another child
//    dc(src == black && cur->color == black, "");
//    NodePtr brother = parent->child[1 - parentNum];
//    if (brother->color == red) {
//    } else {  // brother->color == black
//        parent->color = black;
//        brother->color = red;
//    }
//}
/*
 * 初步思想：删除过程中应该只有可能出现一种麻烦的情况，就是子树删除节点之后不能保持
 * 原本的黑定理，就是说子树的黑节点数少了一，这样就需要上一层的树进行处理（帮忙）
 * 如果上一层的树任然不能保持原本的黑节点数，就需要再上一层的树处理……直到根。
 */
template<class Entry>
bool RedBlackTree<Entry>::removeFromNodeAndFix(NodePtr &parent, const Entry &x,
                                               bool &INeedHelp) {
    if (parent->data == x) {
        int parentNum = -1;
        bool lendBlack = false, curNeedHelp = false;
        if (parent->color == black) {
            if (parent->child[0] && parent->child[0]->color == red) {
                parent->color = red;
                rotate(parent, 0);
                parent->color = black;
                parentNum = 1;
            } else if (parent->child[1] && parent->child[1]->color == red) {
                parent->color = red;
                rotate(parent, 1);
                parent->color = black;
                parentNum = 0;
            } else {  // todo hint two black ?
                lendBlack = true;
                parent->color = red;
                rotate(parent, 1);
                parent->color = black;
                parentNum = 0;
            }
        } else {  // parent->color == red
            if (parent->child[0] && parent->child[0]->color == red) {
                rotate(parent, 0);
                parentNum = 1;
            } else if (parent->child[1] && parent->child[1]->color == red) {
                rotate(parent, 1);
                parentNum = 0;
            } else {  // todo hint two black ?
                parent->child[0]->color = black;
                rotate(parent, 1);
                parentNum = 0;
            }
        }
        if (!removeFromNodeAndFix(parent->child[parentNum], x, curNeedHelp))
            return false;
    } else {  // parent->data != x
        int parentNum = -1;
        bool curNeedHelp = false;
        parentNum = parent->data < x;
        NodePtr cur = parent->child[parentNum];
        if (!cur)
            return false;
        if (!removeFromNodeAndFix(cur, x, curNeedHelp))
            return false;
        if (!curNeedHelp) {
            dc(parent->color != red || cur->color != red, "");
            return true;
        }
        // cur need help
        dc(!cur || cur->color == black, "you sure cur need help?");
        NodePtr &brother = parent->child[1 - parentNum];
        dc(brother != NULL, "no brother ?!");
        if (parent->color == red) {  // hint brother->color == black
            if (tryReduceBlack(brother))
                return parent->color = black || true;
            // reduce black fail -> have child in red
            if (brother->child[parentNum]) {  // this is red
                brother->color = red;
                rotate(brother, parentNum);
                brother->color = black;
            }
            // so here, brother->child[1 - parentNum] is red
            parent->color = black;
            brother->color = red;
            rotate(parent, 1 - parentNum);
            brother->color = black;
            return true;
        } else {  // parent->color == black
            if (tryReduceBlack(brother))
                return INeedHelp = true || true;
            if (brother->color == red) {
                dc(brother->child[0] && brother->child[1], "brother no child ?");
                NodePtr &farChild = brother->child[1 - parentNum];
                NodePtr &nearChild = brother->child[parentNum];
                dc(farChild->color == black, "red under red ?");
                dc(nearChild->color == black, "red under red ?");
                if (nearChild[1 - parentNum]
                        && nearChild[1 - parentNum]->color == red) {
                } else if (nearChild[parentNum]
                        && nearChild[parentNum]->color == red) {
                    nearChild->color = red;
                    rotate(nearChild, parentNum);
                    nearChild->color = black;
                }
                rotate(brother, parentNum);
                rotate(parent, 1 - parentNum);
                brother->color = black;
                farChild->color = red;
                if (!nearChild || nearChild->color == black) {
                    if (farChild[parentNum]->color == red) {
                        rotate(farChild, parentNum);
                        rotate(brother, 1 - parentNum);

                    } else {
                        dc(farChild[1 - parentNum]->color == red, "no red ?");

                    }
                }
                brother->color = red;
                farChild->color = black;
                nearChild->color = black;

                // todo
            } else {  // brother->color == black
                // todo
            }
        }
    }
}

/*
 * check if the node itself can be changed into red
 * to reduce 'one black' in the tree
 */
template<class Entry>
bool RedBlackTree<Entry>::tryReduceBlack(NodePtr &parent) {
    if (parent->color == red)
        return false;
    if (!parent->child[0] && !parent->child[1])
        return parent->color = red || true;
    if (!parent->child[0] || !parent->child[1])
        return false;
    if (parent->child[0]->color == black && parent->child[1]->color == black)
        return parent->color = red || true;
    return false;
}

template<class Entry>
int RedBlackTree<Entry>::testRedTheory() {
    int found = 0;
    inorderTestForRedTheory(root, found);
    return found;
}

template<class Entry>
int RedBlackTree<Entry>::testBlackTheory() {
    int count = 0, length = -1, found = 0;
    inorderTestForBlackTheory(root, count, length, found);
    return found;
}

template<class Entry>
void RedBlackTree<Entry>::inorderTestForRedTheory(NodePtr cur, int &found) {
    if (cur->child[0])
        inorderTestForRedTheory(cur->child[0], found);
    if (cur->child[1])
        inorderTestForRedTheory(cur->child[1], found);
    if (cur->color == red) {
        if (cur->child[0] && cur->child[0]->color == red)
            found++;
        if (cur->child[1] && cur->child[1]->color == red)
            found++;
    }
}

template<class Entry>
void RedBlackTree<Entry>::inorderTestForBlackTheory(NodePtr cur, int &count,
                                                    int &length, int &found) {
    if (cur->color == black)
        count++;
    if (!cur->child[0] && !cur->child[1]) {
        if (length == -1)
            length = count;
        else if (length != count)
            found++;
        if (cur->color == black)
            count--;
        return;
    }
    if (cur->child[0])
        inorderTestForBlackTheory(cur->child[0], count, length, found);
    if (cur->child[1])
        inorderTestForBlackTheory(cur->child[1], count, length, found);
    if (cur->color == black)
        count--;
}

}

#endif /* REDBLACKTREETEMPLATE_H_ */
