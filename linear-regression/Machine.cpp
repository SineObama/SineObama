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
#include <cmath>

#define print(sys, format, args...) \
GetLocalTime(&sys);\
printf("%02d/%02d %02d:%02d:%02d.%03d", sys.wMonth, sys.wDay, sys.wHour,\
       sys.wMinute, sys.wSecond, sys.wMilliseconds);\
printf(format, args);

#define compute_theta(theta, item, alpha) \
for (size_t j = 0; j < n + 1; j++) {\
    data_t sum = 0;\
    if (j != 0)\
        for (size_t i = 0; i < m; i++)\
            sum += item[i] * data[i][j - 1];\
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
        item[i] += theta[j] * data[i][j - 1];\
    item[i] -= data[i][n];\
    J += item[i] * item[i];\
}\
J /= 2 * m;

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
                               data_t alpha, const size_t times,
                               const data_t bigger, const data_t smaller) {

    static const comp_t completion_gap = 0.01;
    static const comp_t completion_precision = completion_gap * 0.001;

    SYSTEMTIME sys;
    comp_t completion = 0;

    data_t * const item = new data_t[m];  // h(x(i)) - y(i)
    data_t * const new_item = new data_t[m];
    data_t * const new_theta = new data_t[n + 1];
    data_t * const der = new data_t[n + 1];  // derivative
    memset(der, 0, sizeof(data_t) * (n + 1));
    data_t * const new_der = new data_t[n + 1];  // derivative
    data_t J = 0;
    data_t product = 0;

    compute_item_and_J(J, item, theta);
    print(sys, "\t%.1f%%\tJ=%.12le\talpha=%lf\n", 0.0f, J, alpha);

    // iteration
    for (size_t t = 0; t < times; t++) {

        // change alpha
//        if (false) {
//#define FACTOR 0.5
//            data_t alpha1 = 0;
//            data_t alpha2 = alpha * FACTOR;
//            data_t alpha3 = alpha;
//            data_t J1 = J;
//            data_t J2 = 0;
//            data_t J3 = 0;
//            memcpy(new_theta, theta, sizeof(data_t) * (n + 1));
//            compute_theta(new_theta, item, alpha2);
//            compute_item_and_J(J2, new_item, new_theta);
//            memcpy(new_theta, theta, sizeof(data_t) * (n + 1));
//            compute_theta(new_theta, item, alpha3);
//            compute_item_and_J(J3, new_item, new_theta);
//            for (size_t count = 0; count < 3; count++) {
//                print(sys, "\t%lf\t%lf\t%lf 0\n", alpha1, alpha2, alpha3);
//                print(sys, "\t%lf\t%lf\t%lf\n", J1, J2, J3);
//                data_t new_alpha =
//                        J1 > J3 ?
//                                ((alpha3 - alpha2) * FACTOR + alpha2) :
//                                ((alpha2 - alpha1) * FACTOR + alpha1);
//                data_t new_J;
//                memcpy(new_theta, theta, sizeof(data_t) * (n + 1));
//                compute_theta(new_theta, item, new_alpha);
//                compute_item_and_J(new_J, new_item, new_theta);
//                if (J1 > J3) {
//                    if (J2 > J3 || J2 > new_J) {
//                        J1 = J2;
//                        alpha1 = alpha2;
//                        J2 = new_J;
//                        alpha2 = new_alpha;
//                    } else {
//                        J3 = new_J;
//                        alpha3 = new_alpha;
//                    }
//                } else {
//                    if (J1 < J2 || new_J < J2) {
//                        J1 = J2;
//                        alpha1 = alpha2;
//                        J2 = new_J;
//                        alpha2 = new_alpha;
//                    } else {
//                        J3 = new_J;
//                        alpha3 = new_alpha;
//                    }
//                }
//            }
//            alpha = alpha2;
//        }
//        compute_theta(theta, item, alpha);
        data_t new_product = 0;
        for (size_t j = 0; j < n + 1; j++) {
            data_t sum = 0;
            if (j != 0)
                for (size_t i = 0; i < m; i++)
                    sum += item[i] * data[i][j - 1];
            else
                for (size_t i = 0; i < m; i++)
                    sum += item[i];
            new_der[j] = sum / m;
            theta[j] -= alpha * new_der[j];
            new_product += new_der[j] * der[j];
        }
        memcpy(der, new_der, sizeof(data_t) * (n + 1));
        compute_item_and_J(J, item, theta);

        // info: completeness and J
        data_t gap = fabs(new_product) - fabs(product);
        comp_t new_completion = (comp_t) (t + 1) / times;
        if (new_completion - completion
                >= completion_gap - completion_precision) {
            completion = new_completion;
            print(sys, "\t%.1f%%\tJ=%.12le\talpha=%lf\tproduct=%lf\tgap=%lf\n",
                  completion * 100, J, alpha, new_product, gap);
        }
        if (gap > 0 && product != 0) {
            if (product < 0)
                alpha *= smaller;
            else
                alpha *= bigger;
        }
        product = new_product;
    }

    delete[] item;
    delete[] new_item;
    delete[] new_theta;
    delete[] der;
    delete[] new_der;

    return alpha;
}

Machine::data_t Machine::guess(data_t *value) {
    data_t sum = theta[0];
    for (size_t i = 1; i < n + 1; i++)
        sum += theta[i] * value[i - 1];
    return sum;
}

#undef print
#undef compute_theta
#undef compute_item_and_J
