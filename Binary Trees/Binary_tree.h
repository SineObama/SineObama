/*
 * Binary_tree.h
 *
 *  Created on: 2015Äê10ÔÂ4ÈÕ
 *      Author: Sine
 */

#ifndef BINARY_TREE_H_
#define BINARY_TREE_H_

#include <iostream>
#include <cstdlib>

template <class Entry>
struct Binary_node {
    Entry data;
    Binary_node<Entry> *left;
    Binary_node<Entry> *right;
    Binary_node();
    Binary_node(const Entry &);
};

template <class Entry>
class Binary_tree {

 public:

    Binary_tree();
    bool empty();
    void preorder(void (*)(Entry *));
    void inorder(void (*)(Entry *));
    void postorder(void (*)(Entry *));

    int size() const;
    void clear();
    int height() const;
    void insert(const Entry &);

    Binary_tree(const Binary_tree<Entry> &);
    Binary_tree &operator=(const Binary_tree<Entry> &);
    ~Binary_tree();

 protected:

    void recursive_preorder(Binary_node<Entry> *, void (*)(Entry *));
    void recursive_inorder(Binary_node<Entry> *, void (*)(Entry *));
    void recursive_postorder(Binary_node<Entry> *, void (*)(Entry *));

    void copy_node_and_subtree(Binary_node<Entry> **, Binary_node<Entry> *);

    Binary_node<Entry> *root;

};

#endif /* BINARY_TREE_H_ */
