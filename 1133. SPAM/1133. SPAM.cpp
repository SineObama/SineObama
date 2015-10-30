/*
 * 1133. SPAM.c
 *
 *  Created on: 2015Äê1ÔÂ14ÈÕ
 *      Author: Happy
 */

#include <stdio.h>
#include <string.h>

char comp[25600];
int len;

int judge(int i) {
    if (i < 0 || i >= len) {
        return 0;
    }
    if (comp[i] >= 'a' && comp[i] <= 'z') {
        return 1;
    } else if (comp[i] >= 'A' && comp[i] <= 'Z') {
        return 1;
    } else if (comp[i] >= '0' && comp[i] <= '9') {
        return 1;
    } else if (comp[i] == '_' || comp[i] == '-') {
        return 1;
    } else {
        return 0;
    }
}

void pbackward(int cur) {
    if (judge(cur) || (comp[cur] == '.' && judge(cur - 1))) {
        pbackward(cur - 1);
        printf("%c", comp[cur]);
    }
    return;
}

void pforward(int cur) {
    if (judge(cur) || (comp[cur] == '.' && judge(cur + 1))) {
        printf("%c", comp[cur]);
        pforward(cur + 1);
    }
    return;
}

int main() {
    int hehe = 0;
    while (!feof(stdin)) {
        gets(comp);
//        gets(&s[hehe]);
//        hehe = strlen(s);
//    }
        int found[100000] = { 0 }, i, count = 0;
        len = strlen(comp);
        for (i = 0; i < len; i++) {
            if (comp[i] == '@') {
                found[count++] = i;
            }
        }
        count = -1;
        while (found[++count]) {
            int cur = found[count];
            if ((judge(cur - 1) || (comp[cur - 1] == '.' && judge(cur - 2)))
                    && (judge(cur + 1) || (comp[cur + 1] == '.' && judge(cur + 2)))) {
                pbackward(cur - 1);
                putchar('@');
                pforward(cur + 1);
                putchar('\n');
            }
        }
}
    return 0;
}
