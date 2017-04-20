/*
 * ImageMorphing.h
 *
 *  Created on: 2017年4月20日
 *      Author: Sine
 */

#ifndef IMAGEMORPHING_H_
#define IMAGEMORPHING_H_

#include <vector>
#include "CImg.h"

namespace ImageMorphing {

typedef unsigned int size_t;

typedef cimg_library::CImg<unsigned char> Img;
typedef cimg_library::CImgList<unsigned char> Imgs;

struct Point {
    Point(int x, int y)
            : x(x),
              y(y) {
    }
    float operator*(Point o) {
        return x * o.x + y * o.y;
    }
    int x, y;
};
typedef std::vector<Point> Points;

// 要求两张图大小一致，点数量一致（不包含四角）。
Imgs deal(Img src, Points s, Img dst, Points d, int frames);

struct Triangle {
    Triangle(int i0, int i1, int i2) {
        i[0] = i0, i[1] = i1, i[2] = i2;
    }
    int &operator[](int index) {
        return i[index];
    }
    int i[3];
};
typedef std::vector<Triangle> Triangles;

// 三角剖分。初始状态由前4个点作为四个角组成。
Triangles divide(Points);

// 要求点不能重合
bool inCircle(Point, Point p1, Point p2, Point p3);

}

#endif /* IMAGEMORPHING_H_ */
