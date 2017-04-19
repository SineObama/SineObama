/*
 * SVM.cpp
 *
 *  Created on: 2017年4月18日
 *      Author: Sine
 */

#include "SVM.h"
#include <iostream>
#include <cstring>
#include <ctime>
#include <windows.h>

SVM::SVM() {
}

void SVM::train(const data_t * const *yx, const int l, const int n,
                const data_t C, data_t *bw, const int iterations) {
    int times = 0, precentage = 0;
    data_t *a = new data_t[l];
    memset(a, 0, sizeof(data_t) * l);
    data_t *Q = new data_t[l];
    for (int i = 0; i < l; i++) {
        data_t sum = 0;
        for (int j = 1; j < n + 1; j++)
            sum += yx[i][j] * yx[i][j];
        Q[i] = sum;
    }
    bool showed = false;
    while (times < iterations) {
        int count = 0;
        DWORD start = GetTickCount();
        for (int i = 0; i < l; i++) {  // 顺序更新
            data_t wx = bw[0];
            for (int j = 1; j < n + 1; j++)
                wx += bw[j] * yx[i][j];
            data_t newa = a[i] - (yx[i][0] * wx - 1) / Q[i];
            if (newa < 0)
                newa = 0;
            else if (newa > C) {
                newa = C;
                count++;
            }
            data_t gap = newa - a[i];
            a[i] = newa;
            bw[0] += gap * yx[i][0];
            for (int j = 1; j < n + 1; j++)
                bw[j] += gap * yx[i][0] * yx[i][j];
        }
        if (!showed) {
            showed = true;
            DWORD end = GetTickCount();
            std::cout << "iteration: " << times + 1 << "\n";
            std::cout << "too big rate: " << (float) count / l << "\n";
            std::cout << "milliseconds: " << end - start << "\n";
        }
        int newprecentage = (float) (times + 1) / iterations * 100;
        if (precentage < newprecentage) {
            precentage = newprecentage;
            std::cout << ".";
        }
        times++;
    }
    std::cout << "\n";
}

void SVM::predict(data_t **yx, const int l, const int n, data_t *bw) {
    for (int i = 0; i < l; i++) {
        data_t wx = bw[0];
        for (int j = 1; j < n + 1; j++)
            wx += bw[j] * yx[i][j];
        yx[i][0] = wx;
    }
}
