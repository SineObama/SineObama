#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cstdlib>
using namespace std;

int main() {
    int n, i, j, *s = (int *) calloc(sizeof(int), 400400), t[200200] = { 0 };
    scanf("%d", &n);
    for (i = 0; i < n; i++) {
        scanf("%d", &s[i]);
    }
    for (i = 0; i < n; i++) {
        scanf("%d", &t[i]);
    }
//    sort(s, s + n);
    int jian[] = { 1, 3, 8, 21, 56, 145, 405, 1033, 3056, 12345, 45678 };
    for (i = 10; i >= 0; i--) {
        int ji = jian[i];
        int k;
        for (k = ji; k < n; k += ji) {
            int j = k;
            while (j >= ji) {
                if (s[j - ji] > s[j]) {
                    int tem = s[j - ji];
                    s[j - ji] = s[j];
                    s[j] = tem;
                }
                j -= ji;
            }
        }
    }
    for (i = 10; i >= 0; i--) {
        int ji = jian[i];
        int k;
        for (k = ji; k < n; k += ji) {
            int j = k;
            while (j >= ji) {
                if (t[j - ji] > t[j]) {
                    int tem = t[j - ji];
                    t[j - ji] = t[j];
                    t[j] = tem;
                }
                j -= ji;
            }
        }
    }
//    sort(t, t + n);
//    for (i = 0; i < n; i++) {
//        s[n + i] = s[i] + 360000;
//    }
    bool find = false;
    j = 0;
    for (i = 0; i < n; i++) {
        int cha = s[i] - t[0];
        find = true;
        for (; j < n; j++) {
            if (s[i + j] - t[j] != cha) {
                find = false;
                break;
            }
        }
        if (find) {
            printf("possible\n");
            break;
        }
    }
    if (!find) {
        printf("impossible\n");
    }
    return 0;
}
