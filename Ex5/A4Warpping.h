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
        double p[2][3];
        double *operator[](int i) {
            return p[i];
        }
    };

    A4Warpping();

    // 输入图像和4个角点的xy坐标
    Img operator()(const Img &src, int *x, int *y, bool resample);
    Img operator()(const Img &src, int *x, int *y, int width, int height, bool resample);

    static void adjust(int *x, int *y);
    // 基于3点三角形区域的形变
    static void warpping(Img &img, int *sx, int *sy, int *dx, int *dy, bool resample);
    // 由3点映射到3点的变换计算出变换矩阵
    static Mat calcMat(int *x, int *y, int *dx, int *dy);
    // 对size行size+1列的矩阵求解，最后一列是。。。
    static void solve(double **, int size);

 private:

    static Img a4Warpping(const Img &src, int *x, int *y, int width,
                          int height, bool resample);

};

#endif /* A4WARPPING_H_ */
