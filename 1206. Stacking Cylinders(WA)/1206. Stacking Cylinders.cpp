/*
 * 1206. Stacking Cylinders.cpp
 *
 *  Created on: 2015Äê10ÔÂ10ÈÕ
 *      Author: Sine
 */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>

int main() {
    int test;
    scanf("%d", &test);
    for (int j = 1; j <= test; j++) {
        int n;
        scanf("%d", &n);
        double t1, sum = 1, first, last;
        scanf("%lf", &t1);
        first = t1;
        double t2 = t1;
        for (int i = 1; i < n; i++) {
            scanf("%lf", &t2);
            sum += sqrt(4 - ((t2 - t1) / 2) * ((t2 - t1) / 2));
            t1 = t2;
        }
        last = t2;
        printf("%d: %.4lf %.4lf\n", j, (first + last) / 2, sum);
    }
}
