#ifndef AVL_NODE
#define AVL_NODE

template<class Entry>
struct AvlNode {
    Entry entry;
    AvlNode<Entry> *left;
    AvlNode<Entry> *right;
    int bf;
};

#endif

#include <cstdlib>

template<class Entry>
AvlNode<Entry> *AVL_search(AvlNode<Entry> *&root, const Entry entry) {
    if (!root)
        return NULL;
    if (entry == root->entry)
        return root;
    if (entry < root->entry)
        return AVL_search(root->left, entry);
    return AVL_search(root->right, entry);
}

template<class Entry>
void rotate(AvlNode<Entry> *&root, bool left) {
    if (left) {
        AvlNode<Entry> *newRoot = root->right;
        root->right = newRoot->left;
        newRoot->left = root;
        root = newRoot;
    } else {
        AvlNode<Entry> *newRoot = root->left;
        root->left = newRoot->right;
        newRoot->right = root;
        root = newRoot;
    }
}

template<class Entry>
void AVL_insert(AvlNode<Entry> *&root, const Entry entry) {
    if (!root) {
        root = new AvlNode<Entry>;
        root->entry = entry;
        root->left = root->right = NULL;
        root->bf = 0;
        return;
    }
    if (entry == root->entry)
        return;
    AvlNode<Entry> *&next = entry < root->entry ? root->left : root->right;
    bool newchild = !next;
    int source = next ? next->bf : 0;
    AVL_insert(next, entry);
    if (newchild) {
        root->bf += entry < root->entry ? -1 : 1;
        return;
    }
    if (next->bf == 0)
        return;
    if (next->bf == source)
        return;
    if (entry < root->entry) {
        root->bf--;
        if (root->bf < -1) {
            next->bf = 0;
            root->bf = 0;
            if (next->bf == 1)
                rotate(next, true);
            rotate(root, false);
        }
    } else {
        root->bf++;
        if (root->bf > 1) {
            next->bf = 0;
            root->bf = 0;
            if (next->bf == -1)
                rotate(next, false);
            rotate(root, true);
        }
    }
}
