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

// Ҫ������ͼ��Сһ�£�������һ�£��������Ľǣ���
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

// �����ʷ֡���ʼ״̬��ǰ4������Ϊ�ĸ�����ɡ�
Triangles divide(Points);

// Ҫ��㲻���غ�
bool inCircle(Point, Point p1, Point p2, Point p3);

}

#endif /* IMAGEMORPHING_H_ */
