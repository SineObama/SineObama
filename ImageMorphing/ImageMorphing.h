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

// todo ����Ҫ�ɼ�
// ���±���ʽ���������ε�������
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

// �����ʷ֡���ʼ״̬��ǰ4������Ϊ�ĸ��ǣ�˳ʱ�����ʱ�룩��ɡ�
Triangles divide(Points);

// �жϵ��Ƿ������������Բ�ڡ�Ҫ��㲻���غ�
bool inCircle(Point, Point p1, Point p2, Point p3);

// �жϵ��Ƿ����������ڡ�
bool inTriangle(Point, Point p1, Point p2, Point p3);

// ��ͼ�ϻ��������ʷֽ��
Img drawTriangle(Img, const Points, const Triangles,
                 const unsigned char *inputColor = NULL);

Img drawPoint(Img, const Points, int radius, const unsigned char *inputColor =
NULL);

Img drawPointAndTriangle(Img, const Points, int radius, const Triangles,
                         const unsigned char *inputColor = NULL);

}

#endif /* IMAGEMORPHING_H_ */
