/*
 * 1282. Computer Game.cpp
 *
 *  Created on: 2015Äê10ÔÂ11ÈÕ
 *      Author: Sine
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

#define MAX 60060
#define next(i) ((i + 1) % MAX)

int main() {
    int n, m;
    while (scanf("%d", &n) != EOF && n != 0) {
        int s[MAX] = { }, ss[MAX] = { };
        for (int i = 0; i < n; i++)
            scanf("%d", &s[i]);
        scanf("%d", &m);
        int i = 0, count = 0;
        for (i = 0; i < n; i++)
            scanf("%d", &ss[i]);
        count += n;
        bool found = true;
        for (; count < m; count++) {
            found = true;
            for (int j = 0, tem = (i - n + MAX) % MAX; j < n;
                    j++, tem = next(tem)) {
//                printf("check: %d %d\n", s[j], ss[tem]);
                if (s[j] != ss[tem]) {
                    found = false;
                    break;
                }
            }
            if (found) {
                printf("%d\n", count - n);
                for (; count < m; count++)
                    scanf("%*d");
                break;
            }
            scanf("%d", &ss[i]);
            i = next(i);
        }
        if (!found)
            printf("no solution\n");
    }
}

