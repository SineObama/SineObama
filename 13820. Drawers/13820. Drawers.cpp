/*
 * 0003.c
 *
 *  Created on: 2015Äê3ÔÂ7ÈÕ
 *      Author: Sine
 */

#include <stdio.h>

int n, l, i, j, k, a[300100] = { 0 }, b[300100] = { 0 },
        d[300100] = { 0 }, passed[300100] = { 0 };

int move(int i) {
    if (passed[i] == 1) {
        passed[i] = 0;
        return 0;
    }
    printf("try to move %d\n", i);
    passed[i] = 1;
    if (d[a[i]] == 0) {
        d[a[i]] = i;
        passed[i] = 0;
        return 1;
    } else if (d[b[i]] == 0) {
        d[b[i]] = i;
        passed[i] = 0;
        return 1;
    } else if (move(d[a[i]])) {
        d[a[i]] = i;
        passed[i] = 0;
        return 1;
    } else if (move(d[b[i]])) {
        d[b[i]] = i;
        passed[i] = 0;
        return 1;
    } else {
        passed[i] = 0;
        return 0;
    }
}

int main() {
    scanf("%d %d", &n, &l);
    for (i = 1; i <= n; i++) {
        scanf("%d %d", &a[i], &b[i]);
        if (move(i)) {
            printf("LADICA\n");
        } else {
            printf("SMECE\n");
        }
    }
    return 0;
}
