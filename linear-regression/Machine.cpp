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
#include <sstream>
#include <float.h>
#include "Machine.h"
#include <cmath>

using namespace std;

#define print(sys, args...) {\
GetLocalTime(&sys);\
printf("%02d/%02d %02d:%02d:%02d.%03d\t", sys.wMonth, sys.wDay, sys.wHour,\
       sys.wMinute, sys.wSecond, sys.wMilliseconds);\
printf(args);\
printf("\n");}
#define compute_theta(theta, item, alpha) \
for (size_t j = 0; j < n + 1; j++) {\
    data_t sum = 0;\
    if (j != 0)\
        for (size_t i = 0; i < m; i++)\
            sum += item[i] * train[i][j - 1];\
    else\
        for (size_t i = 0; i < m; i++)\
            sum += item[i];\
    theta[j] -= alpha / m * sum;\
}
#define compute_item_and_J(J, item, theta) \
J = 0;\
for (size_t i = 0; i < m; i++) {\
    item[i] = theta[0];\
    for (size_t j = 1; j < n + 1; j++)\
        item[i] += theta[j] * train[i][j - 1];\
    item[i] -= train[i][n];\
    J += item[i] * item[i];\
}\
J /= 2 * m;
// 输入特征数（列数）n
Machine::Machine(size_t n, const char *filename)
        : n(n) {
    theta = new data_t[n + 1];
    if (filename == NULL) {
        memset(theta, 0, sizeof(data_t) * (n + 1));
        min = NULL;
        gap = NULL;
    } else {
        FILE *pFile = fopen(filename, "r");
        fscanf(pFile, "%lf", &theta[0]);
        for (size_t j = 1; j < n + 1; j++)
            fscanf(pFile, ",%lf ", &theta[j]);
        fscanf(pFile, "%lf", &min[0]);
        for (size_t j = 1; j < n; j++)
            fscanf(pFile, ",%lf ", &min[j]);
        fscanf(pFile, "%lf", &gap[0]);
        for (size_t j = 1; j < n; j++)
            fscanf(pFile, ",%lf ", &gap[j]);
        fclose(pFile);
    }
}

Machine::~Machine() {
    delete[] gap;
    delete[] min;
    delete[] theta;
}

void Machine::learn(const char *trainFile, const char *thetaFilePrefix,
                    const size_t saveTimes, const size_t times, data_t alpha) {

    // 训练集的行列数
    static const size_t m = 25000;
    static const size_t n = 384;

    SYSTEMTIME sys;
    bool scaling = min == NULL;

    data_t **train = new data_t*[m];
    for (size_t i = 0; i < m; i++)
        train[i] = new data_t[n + 1];
    data_t *max = NULL;
    if (scaling) {
        max = new data_t[n + 1];
        for (size_t j = 0; j < n + 1; j++)
            max[j] = -DATA_MAX;
        min = new data_t[n + 1];
        for (size_t j = 0; j < n + 1; j++)
            min[j] = DATA_MAX;
        gap = new data_t[n];
    }

    print(sys, "start");

    // read training set
    readcsv(trainFile, train, m, n + 1);
    if (scaling) {
        for (size_t i = 0; i < m; i++)
            for (size_t j = 0; j < n + 1; j++) {
                if (max[j] < train[i][j])
                    max[j] = train[i][j];
                if (min[j] > train[i][j])
                    min[j] = train[i][j];
            }
        for (size_t j = 0; j < n; j++) {
            gap[j] = max[j] - min[j];
            if (gap[j] == 0)
                gap[j] = 1;
        }
    }
    for (size_t i = 0; i < m; i++)
        for (size_t j = 0; j < n; j++)
            train[i][j] = (train[i][j] - min[j]) / gap[j];
    print(sys, "finish reading train");
    print(sys, "begin training");

//#define CHANGE

    data_t * const temp_item = new data_t[m];
    data_t * const temp_theta = new data_t[n + 1];
    data_t temp_J = 0;
#ifdef CHANGE
    static const data_t smaller = 0.98;
    static const data_t bigger = 1.02;
    data_t narrow = 0.99;
#endif
    for (size_t count = 0; count < saveTimes; count++) {

        static const comp_t completion_gap = 0.01;
        static const comp_t completion_precision = completion_gap * 0.001;

        data_t * const item = new data_t[m];  // h(x(i)) - y(i)
        data_t J = 0;
#ifndef CHANGE
        bool stopped = false;
#endif
        compute_item_and_J(J, item, theta);
        print(sys, "%.1f%%\tJ=%.12le\talpha=%lf", 0.0f, J, alpha);

        // iteration
        comp_t completion = 0;
        for (size_t t = 0; t < times; t++) {
            compute_theta(temp_theta, item, alpha);
            compute_item_and_J(temp_J, temp_item, temp_theta);
#ifdef CHANGE
            if (temp_J > J * narrow) {
                print(sys, "\told alpha=%lf\tJ=%.12le", alpha, temp_J);
                data_t min_J = temp_J;
                data_t temp_alpha = alpha * smaller;
                memcpy(temp_theta, theta, sizeof(data_t) * (n + 1));
                compute_theta(temp_theta, item, temp_alpha);
                compute_item_and_J(temp_J, temp_item, temp_theta);
                data_t scale = smaller;
                if (temp_J > min_J) {
                    scale = bigger;
                    temp_alpha = alpha * bigger;
                    memcpy(temp_theta, theta, sizeof(data_t) * (n + 1));
                    compute_theta(temp_theta, item, temp_alpha);
                    compute_item_and_J(temp_J, temp_item, temp_theta);
                    if (temp_J > min_J) {
                        narrow = 1 - (J - min_J) / J / 2;
                        print(sys, "\n\nkeep alpha=%lf\tnarrow=%le", alpha,
                                narrow);
                        t--;
                        continue;
                    }
                }
                print(sys, "\tnew alpha=%lf\tJ=%.12le", temp_alpha, temp_J);
                while (temp_J >= J * narrow) {
                    temp_alpha *= scale;
                    memcpy(temp_theta, theta, sizeof(data_t) * (n + 1));
                    compute_theta(temp_theta, item, temp_alpha);
                    compute_item_and_J(temp_J, temp_item, temp_theta);
                    print(sys, "\tnew alpha=%lf\tJ=%.12le", temp_alpha, temp_J);
                    if (temp_J > min_J) {
                        narrow = 1 - (J - min_J) / J / 2;
                        temp_alpha /= scale;
                        print(sys, "\n\nback alpha=%lf\tnarrow=%.12le",
                                temp_alpha, narrow);
                        break;
                    }
                    min_J = temp_J;
                }
                alpha = temp_alpha;
            }
#else
            if (temp_J > J) {
                print(sys, "\tJ=%.12le increased, training stopped", temp_J);
                stopped = true;
                break;
            }
#endif
            memcpy(item, temp_item, sizeof(data_t) * (m));
            memcpy(theta, temp_theta, sizeof(data_t) * (n + 1));
            J = temp_J;
            // output info
            comp_t new_completion = (comp_t) (t + 1) / times;
            if (new_completion - completion
                    >= completion_gap - completion_precision) {
                completion = new_completion;
                print(sys, "%.1f%%\tJ=%.12le\talpha=%lf",
#ifdef CHANGE
                      "\tnarrow=%le",
#endif
                      completion * 100, temp_J, alpha
#ifdef CHANGE
                      ,
                      narrow
#endif
                      );
            }
        }
        delete[] item;
        cout << "finish training" << endl;

        // writing
        static const char *cache_filename = "temp.csv";
        static const char *postfix = ".csv";
        FILE *pFile = fopen(cache_filename, "w");
#define csvfprint(pFile, format, array, len) \
for (size_t _index = 0; _index < (len) - 1; _index++)\
    fprintf(pFile, format",", array[_index]);\
fprintf(pFile, format"\n", array[(len) - 1]);
        csvfprint(pFile, "%lf", theta, n + 1);
        csvfprint(pFile, "%lf", min, n + 1);
        csvfprint(pFile, "%lf", gap, n + 1);
#undef csvfprint
        fclose(pFile);
        char str[100] = { };
        sprintf(str, "J=%.12le,alpha=%lf", J, alpha);
        stringstream ss;
        ss << (string("move ") + cache_filename + " \"" + thetaFilePrefix)
           << str << "x" << (count + 1) << "x" << times << postfix << "\"";
        system(ss.str().c_str());
        cout << "finish writing " << (count + 1) << endl;
#ifndef CHANGE
        if (stopped)
            break;
#endif
    }
#ifdef CHANGE
    delete[] temp_theta;
    delete[] temp_item;
#endif

    cout << "finish learning" << endl;

    delete[] max;
    for (size_t i = 0; i < m; i++)
        delete[] train[i];
    delete[] train;
}

void Machine::guess(const char *testFilename, const char *output) {

    // 测试集的行列数
    static const size_t m = 25000;
    static const size_t n = 384;

    data_t **test = new data_t*[m];
    for (size_t i = 0; i < m; i++)
        test[i] = new data_t[n];

    readcsv(testFilename, test, m, n);

    FILE *pFile = fopen(output, "w");
    fprintf(pFile, "Id,reference\n");
    for (size_t i = 0; i < m; i++) {
        data_t sum = theta[0];
        for (size_t j = 1; j < n + 1; j++)
            sum += theta[j] * test[i][j - 1];
        fprintf(pFile, "%d,%lf\n", i, sum);
    }
    fclose(pFile);

    for (size_t i = 0; i < m; i++)
        delete[] test[i];
    delete[] test;
}

void Machine::readcsv(const char *filename, data_t **data, int row,
                      int column) {
    FILE *pFile = fopen(filename, "r");
    fscanf(pFile, "%*s");
    for (int j = 0; j < column; j++)
        fscanf(pFile, ",%*s");
    for (int i = 0; i < row; i++) {
        fscanf(pFile, " %*d");
        for (int j = 0; j < column; j++)
            fscanf(pFile, ",%lf", &data[i][j]);
    }
    fclose(pFile);
}

#undef print
#undef compute_theta
#undef compute_item_and_J
