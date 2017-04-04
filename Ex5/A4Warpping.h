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

    A4Warpping();

    // 输入图像和4个角点的xy坐标
    Img operator()(const Img &src, int *x, int *y);
    Img operator()(const Img &src, int *x, int *y, int width, int height);

};

#endif /* A4WARPPING_H_ */
