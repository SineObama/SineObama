/*
 * main.cpp
 *
 *  Created on: 2015Äê3ÔÂ13ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>

int main() {
    int t = 0;
    double Pi = 0.0;
    while (++t) {
        if (t % 2) {
            Pi += 1.0 / (2 * t - 1);
        } else {
            Pi -= 1.0 / (2 * t - 1);
        }
        printf("%llf\n", 4 * Pi);
    }
    return 0;
}
