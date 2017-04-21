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

// todo 不需要可见
// 以下标形式保存三角形的三个点
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

// 三角剖分。初始状态由前4个点视为四个角（顺时针或逆时针）组成。
Triangles divide(Points);

// 判断点是否在三角形外接圆内。要求点不能重合
bool inCircle(Point, Point p1, Point p2, Point p3);

// 判断点是否在三角形内。
bool inTriangle(Point, Point p1, Point p2, Point p3);

// 在图上画出三角剖分结果
Img drawTriangle(Img, const Points, const Triangles,
                 const unsigned char *inputColor = NULL);

Img drawPoint(Img, const Points, int radius, const unsigned char *inputColor =
NULL);

Img drawPointAndTriangle(Img, const Points, int radius, const Triangles,
                         const unsigned char *inputColor = NULL);

}

#endif /* IMAGEMORPHING_H_ */
