/*
 * A4Warpping.cpp
 *
 *  Created on: 2017年4月4日
 *      Author: Sine
 */

#include <cmath>
#include<iostream>
#include "A4Warpping.h"

A4Warpping::A4Warpping() {
}

A4Warpping::Img A4Warpping::operator()(const Img &src, int *x, int *y,
                                       bool resample) {
    adjust(x, y);
#define dis(a, b) sqrt((x[a] - x[b])*(x[a] - x[b]) + (y[a] - y[b])*(y[a] - y[b]))
    int width = dis(0, 1) + 0.5;
    int height = dis(0, 3) + 0.5;
    return a4Warpping(src, x, y, width, height, resample);
}

A4Warpping::Img A4Warpping::operator()(const Img &src, int *x, int *y,
                                       int width, int height, bool resample) {
    adjust(x, y);
    return a4Warpping(src, x, y, width, height, resample);
}

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
void A4Warpping::adjust(int *x, int *y) {
#define swap(a, b) { temp = x[a]; x[a] = x[b]; x[b] = temp;\
                     temp = y[a]; y[a] = y[b]; y[b] = temp; }
#define checkSmaller(a, b) if (d##a > d##b) { swap(a, b) \
                      temp = d##a; d##a = d##b; d##b = temp; }
    // 以一个点为基准，根据对其他3个点的距离调整其他三个点的位置，使之有序
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

void A4Warpping::warpping(const Img &src, Img &img, int *sx, int *sy, int *dx,
                          int *dy, bool resample) {
    struct Line {
        // ax + by + c =0
        double a, b, c;
        bool pos;  // 第三个点带入直线方程，结果是否大于0。用于判断点是否在此直线的一侧
    } lines[3];
    for (int i = 0; i < 3; i++) {
        double x1 = dx[i] + 0.5, x2 = dx[(i + 1) % 3] + 0.5, x3 =
                dx[(i + 2) % 3] + 0.5;
        double y1 = dy[i] + 0.5, y2 = dy[(i + 1) % 3] + 0.5, y3 =
                dy[(i + 2) % 3] + 0.5;
        double de = x1 * y2 - x2 * y1;
        if (de == 0) {
            lines[i].a = y1;
            lines[i].b = -x1;
            lines[i].c = 0;
        } else {
            lines[i].a = (y1 - y2) / de;
            lines[i].b = (x2 - x1) / de;
            lines[i].c = 1;
        }
        lines[i].pos = lines[i].a * x3 + lines[i].b * y3 + lines[i].c > 0;
//        std::cout << lines[i].a << " " << lines[i].b << " " << lines[i].c
//                  << "\n";
    }
    Mat mat = calcMat(dx, dy, sx, sy);  // 采用逆向映射
//    std::cout << mat[0][0] << " " << mat[0][1] << " " << mat[0][2] << "\n" << mat[1][0] << " " << mat[1][1] << " " << mat[1][2] << "\n";
    cimg_forXY(img, x, y)  // 可优化为只遍历局部长方形区域，但当前题目A4纸占整个图
    {
        double fx = x + 0.5;
        double fy = y + 0.5;
        bool in = true;
        for (int i = 0; i < 3; i++) {
            double value = lines[i].a * fx + lines[i].b * fy + lines[i].c;
            if (((value > 0) ^ lines[i].pos) && value != 0) {
                in = false;
                break;
            }
        }
        if (!in)
            continue;
        double tx = mat[0][0] * fx + mat[0][1] * fy + mat[0][2];
        double ty = mat[1][0] * fx + mat[1][1] * fy + mat[1][2];
        int intx = tx + 0.4999;
        int inty = ty + 0.4999;
        if (resample) {

        } else {
            if (img.spectrum() == 1) {
                img(x, y) = src(intx, inty);
            } else {
                img(x, y, 0) = src(intx, inty, 0);
                img(x, y, 1) = src(intx, inty, 1);
                img(x, y, 2) = src(intx, inty, 2);
            }
        }
    }
}

A4Warpping::Mat A4Warpping::calcMat(int *sx, int *sy, int *dx, int *dy) {
    Mat mat;
    double mm[3][4] = { };
    double *m[] = { mm[0], mm[1], mm[2] };
    for (int i = 0; i < 3; i++) {
        m[i][0] = sx[i] + 0.5;
        m[i][1] = sy[i] + 0.5;
        m[i][2] = 1;
        m[i][3] = dx[i] + 0.5;
    }
    solve(m, 3);
    for (int i = 0; i < 3; i++)
        mat[0][i] = m[i][3];
    for (int i = 0; i < 3; i++) {
        m[i][0] = sx[i] + 0.5;
        m[i][1] = sy[i] + 0.5;
        m[i][2] = 1;
        m[i][3] = dy[i] + 0.5;
    }
    solve(m, 3);
    for (int i = 0; i < 3; i++)
        mat[1][i] = m[i][3];
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

// 把4个点分成2个三角形区域独立进行Warpping
A4Warpping::Img A4Warpping::a4Warpping(const Img &src, int *x, int *y,
                                       int width, int height, bool resample) {
    Img img(width, height, 1, src.spectrum());
    {
        int sx[3] = { x[0], x[1], x[3] };
        int sy[3] = { y[0], y[1], y[3] };
        int dx[3] = { 0, width - 1, 0 };
        int dy[3] = { height - 1, height - 1, 0 };
        warpping(src, img, sx, sy, dx, dy, resample);
    }
    {
        int sx[3] = { x[1], x[2], x[3] };
        int sy[3] = { y[1], y[2], y[3] };
        int dx[3] = { width - 1, width - 1, 0 };
        int dy[3] = { height - 1, 0, 0 };
        warpping(src, img, sx, sy, dx, dy, resample);
    }
    return img;
}
