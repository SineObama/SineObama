/*
 * A4Warpping.cpp
 *
 *  Created on: 2017年4月4日
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
    // 调整4个点的顺序，从左上角开始，顺时针，对应下图。。。
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
    // 以一个点为基准，根据对其他3个点的距离调整其他三个点的位置，形成顺时针。但有可能存在上下左右的翻转
    int temp;
    double d1 = dis(0, 1);
    double d2 = dis(0, 2);
    double d3 = dis(0, 3);
    checkSmaller(1, 2)
    checkSmaller(3, 2)
    checkSmaller(1, 3)

    // 以x/y坐标判断左右/上下方向，进行翻转
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
    Mat mat;
    double mm[3][4] = {};
    double *m[] = {mm[0], mm[1], mm[2]};
    for (int i = 0; i < 3; i++) {
        m[i][0] = x[i];
        m[i][1] = y[i];
        m[i][2] = 1;
        m[i][3] = dx[i];
    }
    solve(m, 3);
    for (int i = 0; i < 3; i++) {
        mat[0][i] = m[i][3];
    }
    for (int i = 0; i < 3; i++) {
        m[i][0] = x[i];
        m[i][1] = y[i];
        m[i][2] = 1;
        m[i][3] = dy[i];
    }
    solve(m, 3);
    for (int i = 0; i < 3; i++) {
        mat[1][i] = m[i][3];
    }
    return mat;
}

void A4Warpping::solve(double **m, int size) {
    for (int i = 0; i < size; i++) {
        if (m[i][i] == 0) {
            int i2;
            for (i2 = i + 1; i2 < size; i2++)
                if (m[i2][i] != 0)
                    break;
            if (i2 == size)
                throw;  // todo no solution
            for (int j = 0; j < size + 1; j++) {  // 交换两行
                double temp = m[i][j];
                m[i][j] = m[i2][j];
                m[i2][j] = temp;
            }
        }
        for (int j = size; j >= i; j--)
            m[i][j] /= m[i][i];
        for (int i2 = i + 1; i2 < size; i2++)
            if (m[i2][i] != 0)
                for (int j = size; j >= i; j--)
                    m[i2][j] -= m[i2][i] * m[i][j];
    }
    for (int i = size - 2; i >= 0; i--)
        for (int j = i + 1; j < size; j++) {
            m[i][size] -= m[i][j] * m[j][size];
            m[i][j] = 0;
        }
}
