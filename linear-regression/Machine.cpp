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
            max[j] = -DBL_MAX;
        min = new data_t[n + 1];
        for (size_t j = 0; j < n + 1; j++)
            min[j] = DBL_MAX;
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

    for (size_t count = 0; count < saveTimes; count++) {
#define CHOOSE  // choose alpha

        static const comp_t completion_gap = 0.01;
        static const comp_t completion_precision = completion_gap * 0.001;
#ifdef CHOOSE
        static const data_t smaller = 0.9;
        static const data_t bigger = 1.1;
#endif

        comp_t completion = 0;

        data_t * const item = new data_t[m];  // h(x(i)) - y(i)
        data_t J = 0;

        compute_item_and_J(J, item, theta);

#ifdef CHOOSE
        print(sys, "choosing alpha\tinitial alpha=%lf", alpha);
        data_t * const temp_item = new data_t[m];
        data_t * const temp_theta = new data_t[n + 1];
        data_t new_alpha = 0;
        data_t new_J = 0;
        data_t last_J = DBL_MAX;
        while (true) {
            new_alpha = alpha * smaller;
            memcpy(temp_theta, theta, sizeof(data_t) * (n + 1));
            compute_theta(temp_theta, item, new_alpha);
            compute_item_and_J(new_J, temp_item, temp_theta);
            print(sys, "\tsmaller alpha=%lf\tJ=%.12le", new_alpha, new_J);
            if (new_J >= last_J)
                break;
            alpha = new_alpha;
            last_J = new_J;
        }
        while (true) {
            new_alpha = alpha * bigger;
            memcpy(temp_theta, theta, sizeof(data_t) * (n + 1));
            compute_theta(temp_theta, item, new_alpha);
            compute_item_and_J(new_J, temp_item, temp_theta);
            print(sys, "\tbigger alpha=%lf\tJ=%.12le", new_alpha, new_J);
            if (new_J >= last_J)
                break;
            alpha = new_alpha;
            last_J = new_J;
        }
        delete[] temp_theta;
        delete[] temp_item;
#endif

        // iteration
        print(sys, "%.1f%%\tJ=%.12le\talpha=%lf", 0.0f, J, alpha);
        for (size_t t = 0; t < times; t++) {

            compute_theta(theta, item, alpha);
            compute_item_and_J(J, item, theta);

            // info: completeness and J
            comp_t new_completion = (comp_t) (t + 1) / times;
            if (new_completion - completion
                    >= completion_gap - completion_precision) {
                completion = new_completion;
                print(sys, "%.1f%%\tJ=%.12le\talpha=%lf", completion * 100, J,
                      alpha);
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
        fprintf(pFile, "J=%.12le|alpha=%lf\n", J, alpha);
#undef csvfprint
        fclose(pFile);
        stringstream ss;
        ss << (string("move ") + cache_filename + " " + thetaFilePrefix)
           << (count + 1) << "x" << times << postfix;
        system(ss.str().c_str());
        cout << "finish writing " << (count + 1) << endl;
    }

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
