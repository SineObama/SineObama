/*
 * main.cpp
 *
 *  Created on: 2017Äê3ÔÂ14ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>
#include <string>
#include "Machine.h"

#define m 25000
#define n 384

typedef double data_t;

int main(int argc, char **argv) {
    if (argc < 4)
        return -1;

    setbuf(stdout, NULL);

    const char *thetaFilename = argc == 5 ? argv[4] : NULL;

    // machine learning
    Machine machine(n, thetaFilename);
    machine.learn(argv[1], "theta", 1, 10, 0.075);
}
