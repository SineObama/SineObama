/*
 * Machine.cpp
 *
 *  Created on: 2017年3月14日
 *      Author: Sine
 */

#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <windows.h>
#include <iostream>
#include "Machine.h"

#define print_time(sys) GetLocalTime(&sys);\
    printf("%02d/%02d %02d:%02d:%02d.%03d", sys.wMonth, sys.wDay, sys.wHour,\
           sys.wMinute, sys.wSecond, sys.wMilliseconds);

// 输入特征数（列数）n
Machine::Machine(size_t n)
        : n(n) {
    theta = new data_t[n + 1];
    memset(theta, 0, sizeof(data_t) * (n + 1));
}

Machine::~Machine() {
    delete[] theta;
}

// 输入“特征与reference”矩阵和行数m
Machine::data_t Machine::learn(data_t ** const data, const size_t m,
                               data_t alpha, const size_t times, data_t scale) {

    static const comp_t completion_gap = 0.1;
    static const comp_t completion_narrow = completion_gap * 0.001;

    SYSTEMTIME sys;
    comp_t completion = 0;
    data_t * const item = new data_t[m];  // hθ(x(i)) - y(i)
    data_t * const temp_item = new data_t[m];
    data_t * const temp_theta = new data_t[n + 1];

    data_t last_J = 0;
    for (size_t i = 0; i < m; i++) {
        item[i] = theta[0];
        for (size_t j = 0; j < n; j++)
            item[i] += theta[j + 1] * data[i][j];
        item[i] -= data[i][n];
        last_J += item[i] * item[i];
    }
    last_J /= 2 * m;
    print_time(sys);
    printf("\t%.1f%%\tJ=%.12le\talpha=%lf\n", (float) completion * 100, last_J,
           alpha);

    for (size_t t = 0; t < times; t++) {

        for (size_t j = 0; j < n + 1; j++) {
            data_t sum = 0;
            if (j != 0)
                for (size_t i = 0; i < m; i++)
                    sum += item[i] * data[i][j - 1];
            else
                for (size_t i = 0; i < m; i++)
                    sum += item[i];
            temp_theta[j] -= alpha / m * sum;
        }

        data_t J = 0;
        for (size_t i = 0; i < m; i++) {
            temp_item[i] = temp_theta[0];
            for (size_t j = 1; j < n + 1; j++)
                temp_item[i] += temp_theta[j] * data[i][j - 1];
            temp_item[i] -= data[i][n];
            J += temp_item[i] * temp_item[i];
        }
        J /= 2 * m;
        if (last_J < J) {
            alpha /= scale;
            t--;
            memcpy(temp_theta, theta, sizeof(data_t) * (n + 1));
            print_time(sys);
            printf("\tJ=%.12le\talpha=%lf\tJ0=%.12le\n", J, alpha, last_J);
            continue;
        }
        memcpy(item, temp_item, sizeof(data_t) * (m));
        memcpy(theta, temp_theta, sizeof(data_t) * (n + 1));
        last_J = J;

        // output completeness and the J function
        comp_t new_completion = (comp_t) (t + 1) / times;
        if (new_completion - completion >= completion_gap - completion_narrow) {
            completion = new_completion;
            print_time(sys);
            printf("\t%.1f%%\tJ=%.12le\talpha=%lf\n", (float) completion * 100,
                   last_J, alpha);
        }
    }

    delete[] item;
    delete[] temp_item;
    delete[] temp_theta;

    return alpha;
}

Machine::data_t Machine::guess(data_t *value) {
    data_t sum = theta[0];
    for (size_t i = 1; i < n + 1; i++)
        sum += theta[i] * value[i - 1];
    return sum;
}
