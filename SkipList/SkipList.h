/*
 * SkipList.h
 *
 *  Created on: 2017��10��22��
 *      Author: Sine
 */

#ifndef SKIPLIST_H_
#define SKIPLIST_H_

#include <utility>
#include <cstdlib>
#include <cmath>

namespace Sine {

// ���ڵ�������ص�ʵ�����Լ���ģ�δ�غ�
template<class Key, class Value>
class SkipList {

    friend class SkipListTest;

    class Node;
    typedef Node * ptr;
    typedef Node ** ptrs;
    typedef unsigned int size_t;
//    typedef std::pair<Key, Value> data_t;

 public:

    typedef Key key_t;
    typedef Value value_t;

    class iterator {
        friend class SkipList;
     public:
        iterator(const iterator &o) {
            p = o.p;
        }
        std::pair<Key, Value> operator*() {
            return *p->data;
        }
        std::pair<Key, Value> *operator->() {
            return p->data;
        }
        friend bool operator==(iterator a, iterator b) {  // ���� bool ���� int
            return a.p == b.p;
        }
        friend bool operator!=(iterator a, iterator b) {  // ���� bool ���� int
            return a.p != b.p;
        }
        const iterator &operator++(int) {
            p = p->next[0];
            return *this;
        }
     private:
        iterator(ptr p)
                : p(p) {
        }
        ptr p;
    };

    SkipList();
    ~SkipList();

    // ����true��ʾ�ɹ����룬false��ʾ�滻��ԭ����ֵ
    bool insert(const std::pair<Key, Value> &);
    iterator find(const Key &);
    // �����Ƿ����
    bool remove(const Key &);

    iterator begin();
    iterator end();

 private:

    static const size_t max_level = 32;
    static size_t randomLevel();
    // ����keyֵ��λ�� k <= p������p == k
    bool locate(ptr &p, const Key &k);

    struct Node {
        Node() {
            next = NULL;
            data = NULL;
        }
        ~Node() {
            delete[] next;
            delete data;  // todo ���
        }
        ptrs next;
        std::pair<Key, Value> *data;
    };

    size_t height;
    ptr header;
    ptrs update;

};

template<class Key, class Value>
SkipList<Key, Value>::SkipList() {
    height = 1;
    header = new Node();
    header->next = new ptr[max_level];
    header->next[0] = NULL;
    update = new ptr[max_level];
}

template<class Key, class Value>
SkipList<Key, Value>::~SkipList() {
    delete header;
    delete[] update;
}

template<class Key, class Value>
bool SkipList<Key, Value>::insert(const std::pair<Key, Value> &data) {
    ptr p = header;
    size_t level = randomLevel();
    if (locate(p, data.first)) {  // �Ѵ���
        p->data->second = data.second;
        return false;
    }

    if (level > height) {
        header->next[height] = NULL;
        update[height] = header;
        level = height++;
    }

    ptr n = new Node();
    n->data = new std::pair<Key, Value>(data);
    n->next = new ptr[level];
    for (size_t i = 0; i < level; i++) {
        n->next[i] = update[i]->next[i];
        update[i]->next[i] = n;
    }

    return true;
}

template<class Key, class Value>
typename SkipList<Key, Value>::iterator SkipList<Key, Value>::find(
        const Key &k) {
    ptr p = header;
    return locate(p, k) ? p : (ptr) NULL;  // ��ʽת��Ҫд�����
}

template<class Key, class Value>
bool SkipList<Key, Value>::remove(const Key &k) {
    ptr p = header;
    if (!locate(p, k))  // ������
        return false;

    for (size_t i = 0; i < height && update[i]->next[i] == p; i++)
        update[i]->next[i] = p->next[i];
    delete p;

    while (height > 2 && header->next[height - 1] == NULL)
        height--;
    return false;
}

template<class Key, class Value>
typename SkipList<Key, Value>::iterator SkipList<Key, Value>::begin() {
    return header->next[0];
}

template<class Key, class Value>
typename SkipList<Key, Value>::iterator SkipList<Key, Value>::end() {
    return NULL;
}

template<class Key, class Value>
typename SkipList<Key, Value>::size_t SkipList<Key, Value>::randomLevel() {
    static const float log05 = log(0.5), logran = log(RAND_MAX);
    size_t level = (int) ((log(rand()) - logran) / log05) + 1;
    return level > max_level ? max_level : level;
}

template<class Key, class Value>
bool SkipList<Key, Value>::locate(ptr &p, const Key &k) {
    size_t cur = height - 1;
    ptr q;
    do {
        while (q = p->next[cur], q != NULL && q->data->first < k)
            p = q;
        update[cur] = p;
    } while (cur-- != 0);
    p = q;
    return q != NULL && !(k < q->data->first);
}

} /* namespace Sine */

#endif /* SKIPLIST_H_ */
