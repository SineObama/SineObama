/*
 * 1201. 01000001.c
 *
 *  Created on: 2015Äê2ÔÂ26ÈÕ
 *      Author: Sine
 */

#include <stdio.h>
#include <string.h>

int main() {
    int test, curtest;
    scanf("%d", &test);
    for (curtest = 1; curtest <= test; curtest++) {
        char s1[1000] = { }, s2[1000] = { };
        scanf("%s %s", s1, s2);
        int i = strlen(s1) - 1, j = strlen(s2) - 1, count = 0, out[1000] = { };
        while (i >= 0 || j >= 0) {
            out[count] += (i >= 0 ? (s1[i--] - '0') : 0)
                    + (j >= 0 ? (s2[j--] - '0') : 0);
            while (out[count] > 1) {
                out[count + 1]++;
                out[count] -= 2;
            }
            count++;
        }
        printf("%d ", curtest);
        i = count;
        while (out[i] == 0)
            i--;
        while (i >= 0)
            printf("%d", out[i--]);
        putchar('\n');
    }
    return 0;
}
