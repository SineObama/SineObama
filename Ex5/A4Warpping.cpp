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
                                       bool resample, bool affine) {
    adjust(x, y);
#define dis(a, b) sqrt((x[a] - x[b])*(x[a] - x[b]) + (y[a] - y[b])*(y[a] - y[b]))
    int width = dis(0, 1) + 0.5;  // 四舍五入
    int height = dis(0, 3) + 0.5;
    if (affine)
        return a4Affine(src, x, y, width, height, resample);
    else
        return a4Perspective(src, x, y, width, height, resample);
}

A4Warpping::Img A4Warpping::operator()(const Img &src, int *x, int *y,
                                       int width, int height, bool resample,
                                       bool affine) {
    adjust(x, y);
    if (affine)
        return a4Affine(src, x, y, width, height, resample);
    else
        return a4Perspective(src, x, y, width, height, resample);
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

void A4Warpping::affine(const Img &src, Img &img, int *sx, int *sy, int *dx,
                        int *dy, bool resample) {
    struct Line {
        // ax + by + c =0
        double a, b, c;
        bool pos;  // 第三个点带入直线方程，结果是否大于0。用于判断点是否在此直线的一侧
    } lines[3];
    for (int i = 0; i < 3; i++) {
        double x1 = dx[i], x2 = dx[(i + 1) % 3], x3 = dx[(i + 2) % 3];
        double y1 = dy[i], y2 = dy[(i + 1) % 3], y3 = dy[(i + 2) % 3];
        double de = x1 * y2 - x2 * y1;
        if (de == 0) {
            if (y1 != 0 || x1 != 0) {
                lines[i].a = y1;
                lines[i].b = -x1;
            } else {
                lines[i].a = y2;
                lines[i].b = -x2;
            }
            lines[i].c = 0;
        } else {
            lines[i].a = (y1 - y2) / de;
            lines[i].b = (x2 - x1) / de;
            lines[i].c = 1;
        }
        lines[i].pos = lines[i].a * x3 + lines[i].b * y3 + lines[i].c > 0;
    }
    Mat mat = affineMat(dx, dy, sx, sy);  // 采用逆向映射
    cimg_forXY(img, x, y)  // 可优化为只遍历局部长方形区域，但当前题目A4纸占整个图
    {
        bool in = true;
        for (int i = 0; i < 3; i++) {
            double value = lines[i].a * x + lines[i].b * y + lines[i].c;
            if (((value > 0) ^ lines[i].pos) && fabs(value) > precision) {
                in = false;
                break;
            }
        }
        if (!in)
            continue;
        double tx = mat[0][0] * x + mat[0][1] * y + mat[0][2];
        double ty = mat[1][0] * x + mat[1][1] * y + mat[1][2];
        int intx = tx + 0.5;
        int inty = ty + 0.5;
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

A4Warpping::Mat A4Warpping::affineMat(int *sx, int *sy, int *dx, int *dy) {
    Mat mat;
    double mm[3][4] = { };
    double *m[] = { mm[0], mm[1], mm[2] };
    for (int i = 0; i < 3; i++) {
        m[i][0] = sx[i];
        m[i][1] = sy[i];
        m[i][2] = 1;
        m[i][3] = dx[i];
    }
    solve(m, 3, 4);
    for (int i = 0; i < 3; i++)
        mat[0][i] = m[i][3];
    for (int i = 0; i < 3; i++) {
        m[i][0] = sx[i];
        m[i][1] = sy[i];
        m[i][2] = 1;
        m[i][3] = dy[i];
    }
    solve(m, 3, 4);
    for (int i = 0; i < 3; i++)
        mat[1][i] = m[i][3];
    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
    return mat;
}

A4Warpping::Mat A4Warpping::perspectiveMat(int *sx, int *sy, int *dx, int *dy) {
    Mat mat;
    double A[8][8] = { 0 };
    double At_[8][8] = { 0 };
    double AtA_nAtA_[8][16] = { 0 };
    double v1[8] = { 0 };
    double v2[8] = { 0 };
    double *At[8];
    double *nAtA[8];
    double *AtA_nAtA[8];
    for (int i = 0; i < 8; i++) {
        At[i] = At_[i];
        AtA_nAtA_[i][i + 8] = 1;
        AtA_nAtA[i] = AtA_nAtA_[i];
        nAtA[i] = AtA_nAtA_[i] + 8;
    }
    for (int i = 0; i < 4; i++) {
        A[i * 2][0] = sx[i];
        A[i * 2][1] = sy[i];
        A[i * 2][2] = 1;
        A[i * 2 + 1][3] = sx[i];
        A[i * 2 + 1][4] = sy[i];
        A[i * 2 + 1][5] = 1;
        A[i * 2][6] = -(sx[i]) * (dx[i]);
        A[i * 2][7] = -(sy[i]) * (dx[i]);
        A[i * 2 + 1][6] = -(sx[i]) * (dy[i]);
        A[i * 2 + 1][7] = -(sy[i]) * (dy[i]);
        v1[i * 2] = dx[i];
        v1[i * 2 + 1] = dy[i];
    }
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            At[i][j] = A[j][i];
    multiply(At, v1, v2, 8);
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            for (int k = 0; k < 8; k++)
                AtA_nAtA_[i][j] += At[i][k] * A[k][j];
    solve(AtA_nAtA, 8, 16);
    multiply(nAtA, v2, v1, 8);
    for (int i = 0; i < 8; i++)
        mat[i / 3][i % 3] = v1[i];
    mat[2][2] = 1;
    return mat;
}

void A4Warpping::solve(double **m, const int row, const int column) {
    for (int i = 0; i < row; i++) {
        if (m[i][i] == 0) {
            int i2;
            for (i2 = i + 1; i2 < row; i2++)
                if (m[i2][i] != 0)
                    break;
            if (i2 == row) {
                std::cout << "no solution\n";
                return;  // todo no solution
            }
            for (int j = 0; j < column; j++) {  // 交换两行
                double temp = m[i][j];
                m[i][j] = m[i2][j];
                m[i2][j] = temp;
            }
        }
        for (int j = column - 1; j >= i; j--)
            m[i][j] /= m[i][i];
        for (int i2 = i + 1; i2 < row; i2++)
            if (m[i2][i] != 0)
                for (int j = column - 1; j >= i; j--)
                    m[i2][j] -= m[i2][i] * m[i][j];
    }
    for (int i = row - 2; i >= 0; i--)
        for (int j = i + 1; j < row; j++) {
            for (int k = row; k < column; k++)
                m[i][k] -= m[i][j] * m[j][k];
            m[i][j] = 0;
        }
}

void A4Warpping::multiply(const double * const *m, const double *src,
                          double *dst, int size) {
    for (int i = 0; i < size; i++) {
        dst[i] = 0;
        for (int j = 0; j < size; j++)
            dst[i] += m[i][j] * src[j];
    }
}

// 把4个点分成2个三角形区域分别进行仿射
A4Warpping::Img A4Warpping::a4Affine(const Img &src, int *x, int *y, int width,
                                     int height, bool resample) {
    Img img(width, height, 1, src.spectrum());
    {
        int sx[3] = { x[0], x[1], x[3] };
        int sy[3] = { y[0], y[1], y[3] };
        int dx[3] = { 0, width - 1, 0 };
        int dy[3] = { height - 1, height - 1, 0 };
        affine(src, img, sx, sy, dx, dy, resample);
    }
    {
        int sx[3] = { x[1], x[2], x[3] };
        int sy[3] = { y[1], y[2], y[3] };
        int dx[3] = { width - 1, width - 1, 0 };
        int dy[3] = { height - 1, 0, 0 };
        affine(src, img, sx, sy, dx, dy, resample);
    }
    return img;
}

// 由4个点直接进行透视
A4Warpping::Img A4Warpping::a4Perspective(const Img &src, int *x, int *y,
                                          int width, int height,
                                          bool resample) {
    const int srcWidth = src.width(), srcHeight = src.height();
    Img img(width, height, 1, src.spectrum());
    img.fill(0);
    int dx[4] = { 0, width - 1, width - 1, 0 };
    int dy[4] = { height - 1, height - 1, 0, 0 };
    double v1[3], v2[3];
    Mat mat = perspectiveMat(dx, dy, x, y);
    double *m[3] = { mat[0], mat[1], mat[2] };
    cimg_forXY(img, x, y)
    {
        v1[0] = x;
        v1[1] = y;
        v1[2] = 1;
        multiply(m, v1, v2, 3);
        int intx = v2[0] / v2[2] + 0.5;
        int inty = v2[1] / v2[2] + 0.5;
        if (intx >= srcWidth || intx < 0 || inty >= srcHeight || inty < 0)
            continue;
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
    return img;
}
