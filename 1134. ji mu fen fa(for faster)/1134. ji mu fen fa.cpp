/*
 * 1134. ji mu fen fa.c
 *
 *  Created on: 2015Äê2ÔÂ15ÈÕ
 *      Author: Sine
 */

#include <stdio.h>

int main() {
    int n, s;
    while (1) {
        scanf("%d %d", &n, &s);
        if (n == 0) {
            break;
        }
        int i, j, a[10001] = { 0 }, yes = 1, b[10001] = {0};
        for (i = 1; i <= n; i++) {
            scanf("%d %d", &a[i], &b[i]);
        }
        for (i = 1; i < n; i++) {
            for (j = i + 1; j <= n; j++) {
                if (b[i] > b[j]) {
                    int tem = a[i];
                    a[i] = a[j];
                    a[j] = tem;
                    tem = b[i];
                    b[i] = b[j];
                    b[j] = tem;
                }
            }
        }
        for (i = 1; i <= n; i++) {
            if (s >= b[i]) {
                s += a[i];
            } else {
                yes = 0;
                break;
            }
        }
        if (yes) {
            puts("YES");
        } else {
            puts("NO");
        }
    }
    return 0;
}
