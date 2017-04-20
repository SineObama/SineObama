/*
 * ImageMorphing.h
 *
 *  Created on: 2017��4��20��
 *      Author: Sine
 */

#ifndef IMAGEMORPHING_H_
#define IMAGEMORPHING_H_

#include <vector>
#include "CImg.h"

namespace ImageMorphing {

struct Point {
    Point(int x, int y)
            : x(x),
              y(y) {
    }
    int x, y;
};

typedef cimg_library::CImg<unsigned char> Img;
typedef cimg_library::CImgList<unsigned char> Imgs;
typedef std::vector<Point> Points;

// Ҫ������ͼ��Сһ�£�������һ�¡�
Imgs deal(Img src, Points s, Img dst, Points d, int frames);

}

#endif /* IMAGEMORPHING_H_ */
