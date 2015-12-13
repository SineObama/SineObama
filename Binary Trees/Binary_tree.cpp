/*
 * Binary_tree.cpp
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: Sine
 */

#include "Binary_tree.h"

template<class Entry>
Binary_node<Entry>::Binary_node() {
    left = right = NULL;
}

template<class Entry>
Binary_node<Entry>::Binary_node(const Entry &x) {
    left = right = NULL;
    data = x;
}

template<class Entry>
Binary_tree<Entry>::Binary_tree() {
    root = NULL;
}

template<class Entry>
bool Binary_tree<Entry>::empty() {
    return root == NULL;
}

template<class Entry>
void Binary_tree<Entry>::preorder(void (*visit)(Entry *)) {
    recursive_preorder(root, visit);
}

template<class Entry>
void Binary_tree<Entry>::inorder(void (*visit)(Entry *)) {
    recursive_inorder(root, visit);
}

template<class Entry>
void Binary_tree<Entry>::postorder(void (*visit)(Entry *)) {
    recursive_postorder(root, visit);
}

template<class Entry>
int Binary_tree<Entry>::size() const {
// todo
    return 0;
}

template<class Entry>
void Delete(Entry *x) {
    delete x;
}

template<class Entry>
void Binary_tree<Entry>::clear() {
    postorder(Delete);
    root = NULL;
}

template<class Entry>
int Binary_tree<Entry>::height() const {
// todo
    return 0;
}

template<class Entry>
void Binary_tree<Entry>::insert(const Entry &x) {
// todo
}

template<class Entry>
Binary_tree<Entry>::Binary_tree(const Binary_tree<Entry> &t) {
    *this = t;
}

template<class Entry>
Binary_tree<Entry> &Binary_tree<Entry>::operator=(const Binary_tree<Entry> &t) {
    clear();
    root = new Binary_node<Entry>;
    copy_node_and_subtree(&root, t.root);
    return this;
}

template<class Entry>
Binary_tree<Entry>::~Binary_tree() {
    clear();
}

template<class Entry>
void Binary_tree<Entry>::recursive_preorder(Binary_node<Entry> *x,
                                            void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    (*visit)(x);
    recursive_preorder(x->left, visit);
    recursive_preorder(x->right, visit);
}

template<class Entry>
void Binary_tree<Entry>::recursive_inorder(Binary_node<Entry> *x,
                                           void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    recursive_preorder(x->left, visit);
    (*visit)(x);
    recursive_preorder(x->right, visit);
}

template<class Entry>
void Binary_tree<Entry>::recursive_postorder(Binary_node<Entry> *x,
                                             void (*visit)(Entry *)) {
    if (x == NULL)
        return;
    recursive_preorder(x->left, visit);
    recursive_preorder(x->right, visit);
    (*visit)(x);
}

template<class Entry>
void Binary_tree<Entry>::copy_node_and_subtree(Binary_node<Entry> **p,
                                               Binary_node<Entry> *x) {
    (*p)->data = x->data;
    if (x->left) {
        (*p)->left = new Binary_node<Entry>;
        copy_node_and_subtree((*p)->left, x->left);
    }
    if (x->right) {
        (*p)->right = new Binary_node<Entry>;
        copy_node_and_subtree((*p)->right, x->right);
    }
}
