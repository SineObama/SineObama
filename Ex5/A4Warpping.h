/*
 * A4Warpping.h
 *
 *  Created on: 2017��4��4��
 *      Author: Sine
 */

#ifndef A4WARPPING_H_
#define A4WARPPING_H_

#include "CImg.h"

class A4Warpping {

 public:

    typedef cimg_library::CImg<unsigned char> Img;
    // �任����
    struct Mat {
        double p[3][3];
        double *operator[](int i) {
            return p[i];
        }
    };

    A4Warpping();

    // ����ͼ���4���ǵ��xy����
    Img operator()(const Img &src, int *x, int *y, bool resample, bool affine =
            false);
    Img operator()(const Img &src, int *x, int *y, int width, int height,
                   bool resample, bool affine = false);

    // ����˳��
    static void adjust(int *x, int *y);
    // ����
    static void affine(const Img &src, Img &img, int *sx, int *sy, int *dx,
                       int *dy, bool resample = false);
    // ����3�Ե㣬�������任����
    static Mat affineMat(int *sx, int *sy, int *dx, int *dy);
    // ����3�Ե㣬�������任����
    static Mat perspectiveMat(int *sx, int *sy, int *dx, int *dy);
    // ��row*row�ľ�����⣬���һ���ǡ�����
    static void solve(double **, const int row, const int column);
    // �����ξ����������
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
