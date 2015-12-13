#include <cstdlib>
#include <cstdio>

struct Node {
    int data;
    Node *next;
};

class LinkedList {
 public:
    LinkedList() {
        head = NULL;
        size = 0;
    }
    ~LinkedList() {
        while (head) {
            Node *tem = head;
            head = head->next;
            delete tem;
        }
    }

    void insert(int p, int c) {
        if (p < 0)
            return;
        Node *pre = head, *cur = new Node(), *next;
        int count = 0;
        while (count < p - 1 && count++ < size - 1)
            pre = pre->next;
        if (p == 0 || !head) {
            next = head;
            head = cur;
        } else  {
            next = pre->next;
            pre->next = cur;
        }
        cur->data = c;
        cur->next = next;
        size++;
    }

    void deleteByPosition(int p) {
        if (p < 0 || p >= size)
            return;
        Node *pre = head, *cur;
        int count = 0;
        while (count++ < p - 1)
            pre = pre->next;
        if (p == 0) {
            cur = head;
            head = cur->next;
        } else {
            cur = pre->next;
            pre->next = cur->next;
        }
        delete cur;
        size--;
    }

    void print() {
        Node *tem = head;
        while (tem) {
            printf("%d\t", tem->data);
            tem = tem->next;
        }
        printf("\n");
    }
 private:
    Node *head;
    int size;
};
