/*
 * A4Warpping.h
 *
 *  Created on: 2017年4月4日
 *      Author: Sine
 */

#ifndef A4WARPPING_H_
#define A4WARPPING_H_

#include "CImg.h"

class A4Warpping {

 public:

    typedef cimg_library::CImg<unsigned char> Img;
    // 变换矩阵
    struct Mat {
        double p[3][3];
        double *operator[](int i) {
            return p[i];
        }
    };

    A4Warpping();

    // 输入图像和4个角点的xy坐标
    Img operator()(const Img &src, int *x, int *y, bool resample, bool affine =
            false);
    Img operator()(const Img &src, int *x, int *y, int width, int height,
                   bool resample, bool affine = false);

    // 调整顺序
    static void adjust(int *x, int *y);
    // 仿射
    static void affine(const Img &src, Img &img, int *sx, int *sy, int *dx,
                       int *dy, bool resample = false);
    // 输入3对点，算出仿射变换矩阵
    static Mat affineMat(int *sx, int *sy, int *dx, int *dy);
    // 输入3对点，算出仿射变换矩阵
    static Mat perspectiveMat(int *sx, int *sy, int *dx, int *dy);
    // 对row*row的矩阵求解，最后一列是。。。
    static void solve(double **, const int row, const int column);
    // 正方形矩阵乘以向量
    static void multiply(const double * const *m, const double *src,
                         double *dst, int size);

 private:

    static const double precision = 0.0001;

    static Img a4Affine(const Img &src, int *x, int *y, int width, int height,
                        bool resample);
    static Img a4Perspective(const Img &src, int *x, int *y, int width,
                             int height, bool resample);

};

#endif /* A4WARPPING_H_ */
