/*
 * ImageMorphing.cpp
 *
 *  Created on: 2017年4月20日
 *      Author: Sine
 */

#include <cmath>
#include "ImageMorphing.h"

namespace ImageMorphing {

Imgs deal(Img src, Points s, Img dst, Points d, int frames) {
    return Imgs();
}

// 标记保留的边，下标0/1/2分别代表边01/12/20
struct Mark {
    Mark() {
        m[0] = true, m[1] = true, m[2] = true;
    }
    bool &operator[](int index) {
        return m[index];
    }
    bool m[3];
};

Triangles divide(Points points) {
    Triangles triangles;
    triangles.push_back(Triangle(0, 1, 2));
    triangles.push_back(Triangle(0, 2, 3));
    // 每次插入一个点
    for (size_t pi = 4; pi < points.size(); pi++) {
        Triangles found;

        // 找“影响三角形”存到found里
        for (Triangles::iterator tit = triangles.begin();
                tit != triangles.end();) {
            if (inCircle(points[pi], points[tit->i[0]], points[tit->i[1]],
                         points[tit->i[2]])) {
                found.push_back(*tit);
                tit = triangles.erase(tit);
            } else {
                tit++;
            }
        }

        // 对三角形两两查找有无公共边，并标记
        Mark *mark = new Mark[found.size()];
        for (size_t i = 0; i < found.size(); i++) {
            for (size_t j = i + 1; j < found.size(); j++) {
                int i1 = -1, i2 = -1, j1 = -1, j2 = -1;
                for (int ii = 0; ii < 3; ii++) {
                    for (int jj = 0; jj < 3; jj++) {
                        if (found[i][ii] == found[j][jj]) {
                            if (i1 == -1) {
                                i1 = ii;
                                j1 = jj;
                            } else {
                                i2 = ii;
                                j2 = jj;
                            }
                        }
                    }
                }
                if (i2 != -1) {
                    mark[i][(i1 + 1) % 3 == i2 ? i1 : i2] = false;
                    mark[j][(j1 + 1) % 3 == j2 ? j1 : j2] = false;
                }
            }
        }

        // todo “优化”步骤感觉不需要

        for (size_t i = 0; i < found.size(); i++)
            for (size_t j = 0; j < 3; j++)
                if (mark[i][j])
                    triangles.push_back(
                            Triangle(found[i][j], found[i][(j + 1) % 3], pi));
        delete[] mark;
    }
    return triangles;
}

bool inCircle(Point p0, Point p1, Point p2, Point p3) {

#define calc(s, d, x) const double v##s##d##x = p##d.x - p##s.x
#define len(s, d) const double v##s##d = \
    sqrt(v##s##d##x * v##s##d##x + v##s##d##y * v##s##d##y)
#define dot(a, b) (v##a##x * v##b##x + v##a##y * v##b##y)
#define cross(a, b) (v##a##x * v##b##y - v##b##x * v##a##y)
#define angle(a, b, c) double cos##a##b##c = \
    dot(b##a, b##c) / v##b##a / v ##b##c;\
    if (cos##a##b##c < -1 && cos##a##b##c > -1 - precision)\
        cos##a##b##c = -1;\
    else if (cos##a##b##c > 1 && cos##a##b##c < 1 + precision)\
        cos##a##b##c = 1;\
    const double angle##a##b##c = acos(cos##a##b##c);
// 计算角abc相关的东西，包括最终用到的弧度值和两向量叉积(baXbc)，中间变量有向量的xy值和长度
#define calc_all(a, b, c) \
    calc(b, a, x);\
    calc(b, a, y);\
    calc(b, c, x);\
    calc(b, c, y);\
    len(b, a);\
    len(b, c);\
    angle(a, b, c);\
    const double cross##a##b##c = cross(b##a, b##c);

    static const double precision = 0.001;
    static const double pi = acos(-1.0);
    calc_all(1, 0, 2);
    if (angle102 < 0 + precision)
        return false;
    else if (angle102 > pi - precision)
        return true;
    calc_all(1, 3, 2);

    if (cross102 * cross132 > 0)
        return angle102 >= angle132 - precision;
    else
        return angle102 + angle132 >= pi - precision;

#undef calc_all
#undef angle
#undef cross
#undef dot
#undef len
#undef calc

    return true;
}

Img drawTriangle(Img img, Points points, Triangles triangles,
                 const unsigned char *inputColor) {
    static const unsigned char defaultColor[] = { 255, 255, 255 };
    const unsigned char *color = inputColor == NULL ? defaultColor : inputColor;
    for (Triangles::iterator it = triangles.begin(); it != triangles.end();
            it++) {
        for (int i = 0; i < 3; i++) {
            Point p1 = points[it->i[i]];
            Point p2 = points[it->i[(i + 1) % 3]];
            img.draw_line(p1.x, p1.y, p2.x, p2.y, color);
        }
    }
    return img;
}

}
