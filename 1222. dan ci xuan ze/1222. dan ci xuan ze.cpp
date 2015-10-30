/*
 * 1222. dan ci xuan ze.c
 *
 *  Created on: 2015Äê3ÔÂ3ÈÕ
 *      Author: Sine
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define wordlen 11
#define p(t, x) printf(#x": %"#t"\n", x)

short *myindex;
int min = 0, max = 0;

int in_the_scope(int num) {
    int i;
    for (i = min; i <= max; i++) {
        if (myindex[i] == num) {
            return 1;
        }
    }
    return 0;
}

int refresh(int tem_max) {
    int next = min, cur, flag = 1;
    while (flag) {
        int j;
        cur = next++;
        while (myindex[next] == 0 && next < tem_max) {
            next++;
        }
        flag = 0;
        for (j = next; j <= tem_max; j++) {
            if (myindex[j] == myindex[cur]) {
                flag = 1;
                break;
            }
        }
    }
    return cur;
}

int main() {
    int n, m, i, j;
    char (*bei)[wordlen];
    scanf("%d", &n);
    bei = (char (*)[wordlen]) calloc(sizeof(**bei), n * wordlen);
    for (i = 0; i < n; i++) {
        scanf("%s", bei[i]);
    }
    scanf("%d", &m);
    myindex = (short *) calloc(sizeof(*myindex), m);
    for (i = 0; i < m; i++) {
        char tem[wordlen];
        scanf("%s", tem);
        for (j = 0; j < n; j++) {
            if (strcmp(tem, bei[j]) == 0) {
                myindex[i] = j + 1;
                break;
            }
        }
    }
    while (myindex[min] == 0 && min < m) {
        min++;
    }
    if (min == m) {
        printf("0\n0\n");
        return 0;
    }
    int check = 1 + (max = min);
    while (check < m) {
        if (myindex[check] == 0) {
        } else if (in_the_scope(myindex[check])) {
            int tem_max = check, tem_min = refresh(tem_max);
            if (tem_max - tem_min < max - min) {
                max = tem_max;
                min = tem_min;
            }
        } else {
            max = check;
        }
        check++;
    }
    int sum = 0;
    for (i = 0; i < m; i++) {
        if (myindex[i]) {
            for (j = 0; j < i; j++) {
                if (myindex[i] == myindex[j]) {
                    sum--;
                    break;
                }
            }
            sum++;
        }
    }
    printf("%d %d\n", sum, max - min + 1);
    return 0;
}
