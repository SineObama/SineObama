/*
 * A4Warpping.cpp
 *
 *  Created on: 2017��4��4��
 *      Author: Sine
 */

#include <cmath>
#include "A4Warpping.h"

A4Warpping::A4Warpping() {
}

A4Warpping::Img A4Warpping::operator()(const Img &src, int *x, int *y) {
    return src;
}

A4Warpping::Img A4Warpping::operator()(const Img &src, int *x, int *y,
                                       int width, int height) {
    adjust(x, y);
    return src;
}

void A4Warpping::adjust(int *x, int *y) {
    // ����4�����˳�򣬴����Ͻǿ�ʼ��˳ʱ�룬��Ӧ��ͼ������
    /**
     * y
     * |0     1
     * |
     * |        } height
     * |
     * |3     2
     * 0------------x
     *   width
     */
#define dis(a, b) sqrt((x[a] - x[b])*(x[a] - x[b]) + (y[a] - y[b])*(y[a] - y[b]))
#define swap(a, b) { temp = x[a]; x[a] = x[b]; x[b] = temp;\
                     temp = y[a]; y[a] = y[b]; y[b] = temp; }
#define checkSmaller(a, b) if (d##a > d##b) { swap(a, b) \
                      temp = d##a; d##a = d##b; d##b = temp; }
    // ��һ����Ϊ��׼�����ݶ�����3����ľ�����������������λ�ã��γ�˳ʱ�롣���п��ܴ����������ҵķ�ת
    int temp;
    double d1 = dis(0, 1);
    double d2 = dis(0, 2);
    double d3 = dis(0, 3);
    checkSmaller(1, 2)
    checkSmaller(3, 2)
    checkSmaller(1, 3)

    // ��x/y�����ж�����/���·��򣬽��з�ת
    if (x[0] > x[1]) {
        swap(0, 1);
        swap(2, 3);
    }
    if (y[0] < y[3]) {
        swap(0, 3);
        swap(1, 2);
    }
#undef checkSmaller
#undef swap
#undef dis
}

A4Warpping::Mat A4Warpping::calcMat(int *x, int *y, int *dx, int *dy) {

}

void A4Warpping::solve(double **m, int size, double *s) {
    for (int i = 0; i < size; i++) {
        if (m[i][i] == 0) {
            int j;
            for (j = i + 1; j < size; j++) {
                if (m[j][i] != 0)
                    break;
            }
            if (j == size)
                return;  // todo no solution
            for (int k = 0; k < size; k++) {  // ����ij��
                double temp = m[i][k];
                m[i][k] = m[j][k];
                m[j][k] = temp;
            }
        }
        for (int k = size; k >= i; k--)
            m[i][k] /= m[i][i];
        for (int j = i + 1; j < size; j++) {
            if (m[j][i] == 0)
                continue;
            for (int k = size; k >= i; k--)
                m[j][k] -= m[j][i] * m[i][k];
        }
    }
}
