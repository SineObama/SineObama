/*
 * main.cpp
 *
 *  Created on: 2015Äê3ÔÂ31ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <cmath>


int main() {
    int f[1000] = {0};
    f['-'] = 1;
    f['+'] = 1;
    f['*'] = 2;
    f['/'] = 2;
    f['^'] = 3;
    int d[1000] = { 0 }, *dd = d;
    char s[1000] = { 0 }, *ss = s;
    scanf("%d", dd++);
    while (!feof(stdin)) {
        if (scanf("%c", ss++) != EOF) {
            scanf("%d", dd++);
        } else {
                printf("EOF found while scanf, break\n");
                break;
        }
        if (ss[-1] != '+' && ss[-1] != '-' && ss[-1] != '*' && ss[-1] != '/' && ss[-1] != '^') {
            printf("c: %c (num: %d) invalid\n", ss[-1], ss[-1]);
            ss--;
            dd--;
            if (ss[-1] == EOF) {
                printf("EOF found\n");
                break;
            }
        }
        printf("scnaf passed next: s[%d] d[%d]\n", ss - s, dd - d);
        if (ss > s + 1) {
            printf("more than one operator\n");
            if (f[ss[-1]] > f[ss[-2]]) {
                printf("next switch s[%d]: %c (num: %d) operation on d[%d]: %d d[%d]: %d\n", ss - s - 1, ss[-1], ss[-1], dd - d - 1, dd[-1], dd - d -2, dd[-2]);
                switch (ss[-1]) {
                    case '+':
                        dd[-2] = dd[-1] + dd[-2];
                        break;
                    case '-':
                        dd[-2] = dd[-1] - dd[-2];
                        break;
                    case '*':
                        dd[-2] = dd[-1] * dd[-2];
                        break;
                    case '/':
                        dd[-2] = dd[-2] / dd[-1];
                        break;
                    case '^':
                        dd[-2] = (int)pow(dd[-2], dd[-1]);
                        break;
                }
                dd--;
                ss--;
            }
        }
    }

        printf("\nscanf end\n");
    while (dd > d + 1) {
        printf("left %d number in d\n", dd - d);
        printf("next switch s[%d]: %c (num: %d) operation on d[%d]: %d d[%d]: %d\n", ss - s - 1, ss[-1], ss[-1], dd - d - 1, dd[-1], dd - d -2, dd[-2]);
        switch (ss[-1]) {
                    case '+':
                        dd[-2] = dd[-1] + dd[-2];
                        break;
                    case '-':
                        dd[-2] = dd[-1] - dd[-2];
                        break;
                    case '*':
                        dd[-2] = dd[-1] * dd[-2];
                        break;
                    case '/':
                        dd[-2] = dd[-1] / dd[-2];
                        break;
                    case '^':
                        dd[-2] = (int)pow(dd[-2], dd[-1]);
                        break;
                }
                dd--;
                ss--;
    }
    printf("%d\n", d[0]);
}

