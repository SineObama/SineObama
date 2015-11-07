/*
 * 1193. Up the Stairs.cpp
 *
 *  Created on: 2015Äê11ÔÂ6ÈÕ
 *      Author: Sine
 */

#include <cstdio>

int main() {
    int test;
    scanf("%d", &test);
    while (test--) {
        int n, f, b, fw[1010] = { }, bw[1010] = { };
        scanf("%d %d %d", &n, &f, &b);
        bool get = false;
        int min = f, countf = 0, countb = 0;
        for (int i = 0; i < n; i++) {
            int t1, t2;
            scanf("%d %d", &t1, &t2);
            if (t2)
                bw[countb++] = f * 2 - t1;
             else
                fw[countf++] = t1;
            if (!get && t2) {
                get = true;
                min = t1;
            }
        }
        int s[1010] = { }, count = 0;
        s[count++] = - min;
        for (int i = 0; i < countf; i++)
            s[count++] = fw[i];
        for (int i = countb - 1; i >= 0; i--)
            s[count++] = bw[i];
        printf("%d\n", b / n * f * 2 + f + s[b % n]);
    }
}
