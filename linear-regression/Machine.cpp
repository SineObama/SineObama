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
#include <float.h>
#include "Machine.h"
#include <cmath>

using namespace std;

#define postfix ".csv"
#define print(sys, args...) \
GetLocalTime(&sys);\
printf("%02d/%02d %02d:%02d:%02d.%03d\t", sys.wMonth, sys.wDay, sys.wHour,\
       sys.wMinute, sys.wSecond, sys.wMilliseconds);\
printf(args);
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
    data_t *max = new data_t[n + 1];
    for (size_t j = 0; j < n + 1; j++)
        max[j] = -DBL_MAX;
    if (scaling) {
        min = new data_t[n + 1];
        for (size_t j = 0; j < n + 1; j++)
            min[j] = DBL_MAX;
        gap = new data_t[n];
    }

    print(sys, "start\n");

    // read training set
    FILE *pFile = fopen(trainFile, "r");
    fscanf(pFile, "%*s");
    for (size_t j = 0; j < n + 1; j++)
        fscanf(pFile, ",%*s");
    for (size_t i = 0; i < m; i++) {
        fscanf(pFile, " %*d");
        for (size_t j = 0; j < n + 1; j++) {
            fscanf(pFile, ",%lf", &train[i][j]);
            if (scaling) {
                if (max[j] < train[i][j])
                    max[j] = train[i][j];
                if (min[j] > train[i][j])
                    min[j] = train[i][j];
            }
        }
    }
    if (scaling)
        for (size_t j = 0; j < n; j++) {
            gap[j] = max[j] - min[j];
            if (gap[j] == 0)
                gap[j] = 1;
        }
    for (size_t i = 0; i < m; i++)
        for (size_t j = 0; j < n; j++)
            train[i][j] = (train[i][j] - min[j]) / gap[j];
    fclose(pFile);
    print(sys, "finish reading train\n");

    for (size_t count = 0; count < saveTimes; count++) {
//#define CHANGE

        static const comp_t completion_gap = 0.01;
        static const comp_t completion_precision = completion_gap * 0.001;
#ifdef CHANGE
        static const data_t smaller = 0.99;
        static const data_t bigger = 1.01;
#endif

        comp_t completion = 0;

        data_t * const item = new data_t[m];  // h(x(i)) - y(i)
//        data_t * const new_item = new data_t[m];
//        data_t * const new_theta = new data_t[n + 1];
        data_t J = 0;
#ifdef CHANGE
        data_t * const der = new data_t[n + 1];  // derivative
        memset(der, 0, sizeof(data_t) * (n + 1));
        data_t * const new_der = new data_t[n + 1];
        data_t product = 0;
#endif

        compute_item_and_J(J, item, theta);
        print(sys, "%.1f%%\tJ=%.12le\talpha=%lf\n", 0.0f, J, alpha);

        // iteration
        for (size_t t = 0; t < times; t++) {

#ifdef CHANGE
            data_t new_product = 0;
            for (size_t j = 0; j < n + 1; j++) {
                data_t sum = 0;
                if (j != 0)
                for (size_t i = 0; i < m; i++)
                sum += item[i] * train[i][j - 1];
                else
                for (size_t i = 0; i < m; i++)
                sum += item[i];
                new_der[j] = sum / m;
                theta[j] -= alpha * new_der[j];
                new_product += new_der[j] * der[j];
            }
            memcpy(der, new_der, sizeof(data_t) * (n + 1));
#else
            compute_theta(theta, item, alpha);
#endif
            compute_item_and_J(J, item, theta);

            // info: completeness and J
#ifdef CHANGE
            data_t product_gap = fabs(new_product) - fabs(product);
#endif
            comp_t new_completion = (comp_t) (t + 1) / times;
            if (new_completion - completion
                    >= completion_gap - completion_precision) {
                completion = new_completion;
                print(sys,
#ifdef CHANGE
                      "%.1f%%\tJ=%.12le\talpha=%lf\tproduct=%lf\tgap=%lf\n",
#else
                      "%.1f%%\tJ=%.12le\talpha=%lf\n",
#endif
                      completion * 100,
                      J, alpha
#ifdef CHANGE
                      ,
                      new_product, product_gap
#endif
                      );
            }
#ifdef CHANGE
            if (gap > 0 && product != 0) {
                if (product < 0)
                alpha *= smaller;
                else
                alpha *= bigger;
            }
//            if (new_product < 0 && t > 1 && product_gap > 0)
//                alpha *= 0.99;
            product = new_product;
#endif
        }

        delete[] item;
//        delete[] new_item;
//        delete[] new_theta;
#ifdef CHANGE
        delete[] der;
        delete[] new_der;
#endif

        cout << "finish training" << endl;

        // writing
        cout << "finish writing " << (count + 1) << endl;
    }

    cout << "finish learning" << endl;

    delete[] gap;
    delete[] min;
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
    for (int j = 0; j < column + 1; j++)
        fscanf(pFile, ",%*s");
    for (int i = 0; i < row; i++) {
        fscanf(pFile, " %*d");
        for (int j = 0; j < column + 1; j++)
            fscanf(pFile, ",%lf", &data[i][j]);
    }
    fclose(pFile);
}

#undef print
#undef compute_theta
#undef compute_item_and_J
