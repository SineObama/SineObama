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

    A4Warpping();

    // ����ͼ���4���ǵ��xy����
    Img operator()(const Img &src, int *x, int *y);
    Img operator()(const Img &src, int *x, int *y, int width, int height);

};

#endif /* A4WARPPING_H_ */
