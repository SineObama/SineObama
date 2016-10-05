/*
 * main.cpp
 *
 *  Created on: 2015Äê3ÔÂ13ÈÕ
 *      Author: Sine
 */

// this is quite low

#include <cstdio>
#include <iostream>

int main() {
    long long int t = 0;
    double Pi = 0.0;
    while (++t) {
        if (t % 2) {
            Pi += 1.0 / (2 * t - 1);
        } else {
            Pi -= 1.0 / (2 * t - 1);
        }
        if (t % 1000000l == 0)
        printf("%.20llf   t:%lld\n", 4 * Pi, t);
    }
    return 0;
}
