/*
 * 13857. Combination Lock.cpp
 *
 *  Created on: 2015Äê3ÔÂ17ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <cmath>
using namespace std;

int main() {
    int n;
    scanf("%d", &n);
    int a[3], b[3], i;
    for (i = 0; i < 3; i++) {
        scanf("%d", &a[i]);
    }
    for (i = 0; i < 3; i++) {
        scanf("%d", &b[i]);
    }
    if (n <= 5) {
        printf("%d\n", n * n * n);
        return 0;
    }
    int sub1 = 1;
    for (i = 0; i < 3; i++) {
        int tem = abs(a[i] - b[i]);
        if (tem <= 4) {
            sub1 *= 5 - tem;
        } else {
            sub1 = 0;
            break;
        }
    }
    int sub2 = 1;
    for (i = 0; i < 3; i++) {
        int bigger = a[i] < b[i] ? b[i] : a[i];
        int smaller = a[i] + b[i] - bigger;
        int tem = smaller + n - bigger;
        if (tem <= 4) {
            sub2 *= 5 - tem;
        } else {
            sub2 = 0;
            break;
        }
    }
    printf("%d\n", 250 - sub1 - sub2);
    return 0;
}
