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
        double p[2][3];
        double *operator[](int i) {
            return p[i];
        }
    };

    A4Warpping();

    // ����ͼ���4���ǵ��xy����
    Img operator()(const Img &src, int *x, int *y, bool resample);
    Img operator()(const Img &src, int *x, int *y, int width, int height, bool resample);

    static void adjust(int *x, int *y);
    // ����3��������������α�
    static void warpping(Img &img, int *sx, int *sy, int *dx, int *dy, bool resample);
    // ��3��ӳ�䵽3��ı任������任����
    static Mat calcMat(int *x, int *y, int *dx, int *dy);
    // ��size��size+1�еľ�����⣬���һ���ǡ�����
    static void solve(double **, int size);

 private:

    static Img a4Warpping(const Img &src, int *x, int *y, int width,
                          int height, bool resample);

};

#endif /* A4WARPPING_H_ */
