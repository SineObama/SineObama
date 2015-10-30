#include <stdio.h>
typedef struct hehe {
    int v, p;
    struct hehe *next;
} data;
int main() {
    int n, m, i, j;
    data tem, buy[30];
    while (!feof(stdin)) {
        scanf("%d %d", &n, &m);
        for (i = 0; i < m; i++) {
            scanf("%d %d", &buy[i].v, &buy[i].p);
        }
        for (i = 0; i < m - 1; i++) {
            for (j = i + 1; j < m; j++) {
                if (buy[i].p < buy[j].p) {
                    tem = buy[i];
                    buy[i] = buy[j];
                    buy[j] = tem;
                }
            }
        }
        for (i = 0; i < m - 1; i++) {
            for (j = i + 1; j < m; j++) {
                if (buy[i].p == buy[j].p) {
                    if (buy[i].v < buy[j].v) {
                        tem = buy[i];
                        buy[i] = buy[j];
                        buy[j] = tem;
                    }
                } else {
                    break;
                }
            }
        }
        data chose[30], *end = NULL, *cur = NULL;
        int sumv = 0, cm = 0;
        for (i = 0; i < m; i++) {
            if (n >= sumv + buy[i].v) {
                if (cur == NULL) {
                    end = &buy[i];
                    cur = &buy[i];
                } else {
                    buy.next = cur;
                    cur = &buy[i];
                }
            } else {

            }
        }
    }
    return 0;
}
