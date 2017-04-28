/*
 * SVM.cpp
 *
 *  Created on: 2017年4月18日
 *      Author: Sine
 */

#include "SVM.h"
#include <cstdio>
#include <iostream>
#include <cstring>
#include <ctime>
#include <windows.h>

SVM::SVM(size_t n)
        : n(n) {
    bw = new data_t[n + 1];
    memset(bw, 0, sizeof(data_t) * (n + 1));
    min = NULL;
    width = NULL;
}

SVM::SVM(const char *filename) {
    FILE *pFile = fopen(filename, "r");
    char s[11] = { }, version[11] = { };
    // line 1
    fscanf(pFile, "%7s,%s", s, version);
    if (strcmp(s, "version") || strcmp(version, VERSION))
        throw;
    // line 2
    fscanf(pFile, "\nJ,%*"FORMAT);
    // line 3
    fscanf(pFile, "\nn,%d", &n);
    // line 4
    bw = new data_t[n + 1];
    fscanf(pFile, "\nbw,%"FORMAT, &bw[0]);
    for (size_t j = 1; j < n + 1; j++)
        fscanf(pFile, ",%"FORMAT, &bw[j]);
    // line 5
    min = new data_t[n + 1];
    fscanf(pFile, "\nmin,%"FORMAT, &min[0]);
    for (size_t j = 1; j < n + 1; j++)
        fscanf(pFile, ",%"FORMAT, &min[j]);
    // line 6
    width = new data_t[n + 1];
    fscanf(pFile, "\nwidth,%"FORMAT, &width[0]);
    for (size_t j = 1; j < n + 1; j++)
        fscanf(pFile, ",%"FORMAT, &width[j]);
    fclose(pFile);
}

SVM::~SVM() {
    delete[] bw;
    delete[] min;
    delete[] width;
}

void SVM::train(data_t **yx, const size_t l, const data_t C,
                const size_t iterations, const char *modelFile) {

    // scaling

    if (min == NULL) {
        data_t *max = new data_t[n + 1];
        min = new data_t[n + 1];
        width = new data_t[n + 1];
        for (size_t j = 1; j < n + 1; j++) {
            max[j] = -DATA_MAX;
            min[j] = DATA_MAX;
            for (size_t i = 0; i < l; i++) {
                if (max[j] < yx[i][j])
                    max[j] = yx[i][j];
                if (min[j] > yx[i][j])
                    min[j] = yx[i][j];
            }
            width[j] = max[j] - min[j];
            if (width[j] == 0)
                width[j] = 1;
        }
        delete[] max;
    }
    for (size_t j = 1; j < n + 1; j++)
        for (size_t i = 0; i < l; i++)
            yx[i][j] = (yx[i][j] - min[j]) / width[j];

    // Linear SVM

    data_t *a = new data_t[l];
    memset(a, 0, sizeof(data_t) * l);
    data_t *Q = new data_t[l];
    for (size_t i = 0; i < l; i++) {
        data_t sum = 0;
        for (size_t j = 1; j < n + 1; j++)
            sum += yx[i][j] * yx[i][j];
        Q[i] = sum;
    }

    bool showed = false;
    size_t iteration = 1, precentage = 0;
    while (iteration <= iterations) {
        int count = 0;
        DWORD start = GetTickCount();
        for (size_t i = 0; i < l; i++) {  // 顺序更新
            data_t wx = bw[0];
            for (size_t j = 1; j < n + 1; j++)
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
            for (size_t j = 1; j < n + 1; j++)
                bw[j] += gap * yx[i][0] * yx[i][j];
        }
        if (!showed) {
            showed = true;
            DWORD end = GetTickCount();
            std::cout << "iteration: " << iteration << "\n";
            std::cout << "too big rate: " << (float) count / l << "\n";
            std::cout << "milliseconds: " << end - start << "\n";
        }
        size_t newprecentage = (float) iteration / iterations * 100;
        if (precentage < newprecentage) {
            precentage = newprecentage;
            std::cout << ".";
        }
        iteration++;
    }
    std::cout << "\n";

    // 保存

    // 计算估价函数？
    data_t J = 0;
    for (size_t j = 0; j < n + 1; j++)
        J += bw[j] * bw[j];
    J /= 2;
    for (size_t i = 0; i < l; i++) {
        data_t wx = bw[0];
        for (size_t j = 1; j < n + 1; j++)
            wx += bw[j] * yx[i][j];
        J += C * (1 - yx[i][0] * wx);
    }

    FILE *pFile = fopen(modelFile, "w");
    // line 1
    fprintf(pFile, "%s,%s\n", "version", VERSION);
    // line 2
    fprintf(pFile, "J,%"FORMAT"\n", J);
    // line 3
    fprintf(pFile, "n,%d\n", n);
    // line 4
    fprintf(pFile, "bw,");
    for (size_t i = 0; i < n; i++)
        fprintf(pFile, "%"FORMAT",", bw[i]);
    fprintf(pFile, "%"FORMAT"\n", bw[n]);
    // line 5
    fprintf(pFile, "min,");
    for (size_t i = 0; i < n; i++)
        fprintf(pFile, "%"FORMAT",", min[i]);
    fprintf(pFile, "%"FORMAT"\n", min[n]);
    // line 6
    fprintf(pFile, "width,");
    for (size_t i = 0; i < n; i++)
        fprintf(pFile, "%"FORMAT",", width[i]);
    fprintf(pFile, "%"FORMAT"\n", width[n]);
    fclose(pFile);
}

void SVM::predict(const data_t * const *yx, const size_t l,
                  const char *outputFile) {

    // 预测
    data_t *y = new data_t[l];
    for (size_t i = 0; i < l; i++) {
        data_t wx = bw[0];
        for (size_t j = 1; j < n + 1; j++)
            wx += bw[j] * ((yx[i][j] - min[j]) / width[j]);
        y[i] = wx;
    }

    // 计算准确率。只按正负判断。
    int count = 0;
    for (size_t i = 0; i < l; i++)
        if (y[i] * yx[i][0] > 0)
            count++;
    data_t correct = (data_t) count / l * 100;
    std::cout << correct << "% correct\n";

    // 保存到文件
    FILE *pFile = fopen(outputFile, "w");
    fprintf(pFile, "id,label,%.2"FORMAT"%% correct\n", correct);
    for (size_t i = 0; i < l; i++) {
        if (y[i] < -1)
            y[i] = 0;
        else if (y[i] > 1)
            y[i] = 1;
        else
            y[i] = (y[i] + 1) / 2;
        fprintf(pFile, "%d,%"FORMAT"\n", i, y[i]);
    }
    fclose(pFile);

    delete[] y;
}
