#include <iostream>
#include <vector>
using namespace std;

struct dian {
    unsigned int dis;
    vector<int> e, r;
} d[10010];

struct road {
    int a, b;
    int e;
} r[10010];

    int n, m;

void tryy(int i) {
    cout << "tryy: " << i << '\n';
    for (int iii = 0; iii < n - 1; iii++) {
        cout << "begin: " << iii << '\n';
        if (r[iii].a == i) {
            if (d[r[iii].b].dis > d[i].dis + r[iii].e) {
                cout << "!!!";
                d[r[iii].b].dis = d[i].dis + r[iii].e;
                d[r[iii].b].e = d[i].e;
                d[r[iii].b].e.push_back(r[iii].e);
                d[r[iii].b].r = d[i].r;
                d[r[iii].b].r.push_back(r[iii].b);
                tryy(r[iii].b);
            }
        }
        cout << "123\n";
        if (r[iii].b == i) {
            if (d[r[iii].a].dis > d[i].dis + r[iii].e) {
                cout << "!!!";
                d[r[iii].a].dis = d[i].dis + r[iii].e;
                d[r[iii].a].e = d[i].e;
                d[r[iii].a].e.push_back(r[iii].e);
                d[r[iii].a].r = d[i].r;
                d[r[iii].a].r.push_back(r[iii].a);
                tryy(r[iii].a);
            }
        }
        cout << "end: " << iii << '\n';
    }
}

int main() {
    cin >> n >> m;
    int w[10010] = {};
    for (int i = 1; i <= n; i++) {
        cin >> w[i];
    }
    for (int i = 0; i < n - 1; i++) {
        cin >> r[i].a;
        cin >> r[i].b;
        cin >> r[i].e;
    }
    for (int iii = 0; iii < m; iii++) {
        for (int jjj = 0; jjj < n; jjj++) {
            d[jjj].dis = -1;
            d[jjj].e.clear();
            d[jjj].r.clear();
        }
        int s, t;
        cin >> s >> t;
        d[s].dis = 0;
        d[s].r.push_back(s);
        tryy(s);
        vector<int>::iterator it1 = d[t].r.begin(), it2 = it1 + 1;
        *it1 = w[*it1];
        cerr << "111\n";
        for (; it2 != d[t].r.end(); it1++, it2++) {
            *it2 = w[*it2];
            if (*it2 > *it1) {
                *it2 = *it1;
            }
        }
        cerr << "111\n";
        it1 = d[t].r.begin(), it2 = d[t].e.begin();
        int yi = 0, ge = 0;
        for (; it2 != d[t].e.end(); it1++, it2++) {
            cout << "mul: " << *it1 << ' ' << *it2 << '\n';
            ge += *it1 * *it2;
            yi += ge / 10000000000;
            ge %= 10000000000;
        }
        if (yi)
        cout << yi;
        cout << ge << '\n';
    }
}
