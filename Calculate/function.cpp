/*
 * Calculate_function.cpp
 *
 *  Created on: 2015��3��24��
 *      Author: Sine
 */

#include "Header.h"

void print_messsage() {
    printf("Please input your equation:\n");
    return;
}

void get_input(char *input, unsigned int maxlen) {
    register unsigned int count = 0;
    for (count = 0; count < maxlen; count++) {
        scanf("%c", &input[count]);
        if (input[count] == '\n') {
            // ����һֱ��0
            for (; count < maxlen; count++) {
                input[count] = 0;
            }
            return;
        }
    }

    // ���е��˴�˵��������maxlen���ַ���ȴ��û�л��з�
    if (maxlen - 1 >= 0) {
        input[maxlen - 1] = 0;
    }
    while (getchar() != '\n') {
    }
    return;
}

void Calculate(char *s, char *result) {

    return;
}

void output(char *result) {
    printf("result: %s\n", result);
    return;
}

template<typename T>
T sqrt(T m, unsigned int ci = 2, unsigned int t = 250) {
    T re = m, n = 1.0;
    while (t--) {
        T tem = (m - n) / static_cast<double>(ci) + n;
        if (pow(tem, ci) == re) {
            return tem;
        }
        if (pow(tem, ci) > re) {
            m = tem;
        } else {
            n = tem;
        }
    }
    return n;
}

