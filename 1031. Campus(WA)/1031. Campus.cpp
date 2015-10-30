#include <stdio.h>
#include <string.h>
#include <string>
#include <map>
#include <set>
#include <iostream>
using namespace std;

struct line {
    line() {
        d = -1;
    }
    string name;
    unsigned int d;
    map<line*, int> road;
};

void check(line * a) {
//    cerr << "checking: " << a->name << "\n";
    map<line*, int>::iterator it;
    for (it = a->road.begin(); it != a->road.end(); it++) {
        if (it->first->d > it->second + a->d) {
            it->first->d = it->second + a->d;
//            cerr << "enter to " << it->first->name << "\n";
            check(it->first);
//            cerr << "exit " << it->first->name << "\n";
        }
    }
}

int main() {
    int test = 0;
    scanf("%d", &test);
    while (test--) {
        line r[201];
        int n = 0, max = 0;
        scanf("%d", &n);
        while (n--) {
            string p1, p2;
            int d = 0;
            cin >> p1 >> p2 >> d;

            line *ptr1 = NULL, *ptr2 = NULL;
            for (int j = 0; j < max; j++)
                if (r[j].name == p1) {
                    ptr1 = &r[j];
                    break;
                }
            for (int j = 0; j < max; j++)
                if (r[j].name == p2) {
                    ptr2 = &r[j];
                    break;
                }

            if (ptr1 == NULL) {
                r[max].name = p1;
                ptr1 = &r[max++];
            }
            if (ptr2 == NULL) {
                r[max].name = p2;
                ptr2 = &r[max++];
            }

            ptr1->road.insert(make_pair(ptr2, d));
            ptr2->road.insert(make_pair(ptr1, d));
        }
        string start, dest;
        cin >> start >> dest;

        line *ps = NULL, *pd = NULL;
        for (int j = 0; j < max; j++)
            if (r[j].name == start) {
                ps = &r[j];
                break;
            }
        for (int j = 0; j < max; j++)
            if (r[j].name == dest) {
                pd = &r[j];
                break;
            }
//        cerr << "1\n";
        if (ps != NULL) {
            ps->d = 0;
            check(ps);
        }
//        cerr << "2\n";
        if (pd == NULL || pd->d == (unsigned int) (-1) || ps == NULL)
            printf("-1\n");
        else
            printf("%u\n", pd->d);
    }
}
