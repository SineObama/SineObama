/*
 * ImageMorphing.cpp
 *
 *  Created on: 2017年4月20日
 *      Author: Sine
 */

#include <cmath>
#include "ImageMorphing.h"

namespace ImageMorphing {

// 变换矩阵
struct Mat {
    double p[3][3];
    double *operator[](int i) {
        return p[i];
    }
};
Mat affineMat(int *sx, int *sy, int *dx, int *dy);

Imgs deal(Img src, Points s, Img dst, Points d, int frames) {

    // 三角剖分
    const Triangles t = divide(s);

    // 展示结果
    Img pointed = drawPoint(src, s, 4);
    drawTriangle(pointed, s, t).display("三角剖分结果");

    // 计算最终warping，采用逆向相对映射"backward-relative"
    const int width = src.width();
    const int height = src.height();
    const int depth = src.depth();
    const int spectrum = src.spectrum();
    Img swarp(width, height, depth, spectrum);
    Img dwarp(width, height, depth, spectrum);
    for (Triangles::const_iterator it = t.begin(); it != t.end(); it++) {

#define p(l, n) Point l##p##n = l[it->i[n]]
#define array(l, i) int l##i[3] = { (l##p##0).i, (l##p##1).i, (l##p##2).i }

        p(s, 0);
        p(s, 1);
        p(s, 2);
        p(d, 0);
        p(d, 1);
        p(d, 2);
        array(s, x);
        array(s, y);
        array(d, x);
        array(d, y);

#undef array
#undef p

        Mat sm = affineMat(dx, dy, sx, sy);
        cimg_forXY(src, x, y)
        {  // todo 优化：缩小检测范围

        }

//        Mat dm = affineMat(sx, sy, dx, dy);

    }

    Imgs imgs;
    return imgs;
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

#define calc_vi(s, d, i) const double v##s##d##i = (p##d).i - (p##s).i
#define calc_v(s, d) calc_vi(s,d,x); calc_vi(s,d,y);
#define calc_cross(a, b, c) const double cross##a##b##c = \
    (v##b##a##x * v##b##c##y - v##b##c##x * v##b##a##y)

bool inCircle(Point p0, Point p1, Point p2, Point p3) {

#define calc_len(s, d) const double len##s##d = \
    sqrt(v##s##d##x * v##s##d##x + v##s##d##y * v##s##d##y)
#define dot(a, b) (v##a##x * v##b##x + v##a##y * v##b##y)
#define calc_angle(a, b, c) double cos##a##b##c = \
    dot(b##a, b##c) / len##b##a / len##b##c;\
    if (cos##a##b##c < -1 && cos##a##b##c > -1 - precision)\
        cos##a##b##c = -1;\
    else if (cos##a##b##c > 1 && cos##a##b##c < 1 + precision)\
        cos##a##b##c = 1;\
    const double angle##a##b##c = acos(cos##a##b##c);
// 计算角abc相关的东西，包括最终用到的弧度值和两向量叉积(baXbc)，中间变量有向量的xy值和长度
#define calc_all(a, b, c) \
    calc_vi(b, a, x);\
    calc_vi(b, a, y);\
    calc_vi(b, c, x);\
    calc_vi(b, c, y);\
    calc_len(b, a);\
    calc_len(b, c);\
    calc_angle(a, b, c);\
    calc_cross(a, b, c);

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
#undef calc_angle
#undef dot
#undef calc_len

}

bool inTriangle(Point p0, Point p1, Point p2, Point p3) {

#define dcalc_v(s, d) calc_v(s, d); calc_v(d, s);
// 计算点i和j是否在直线ab同侧
#define calc_side(i, j, a, b) \
    calc_cross(a, i, b);\
    calc_cross(a, j, b);\
    const bool side##i##j = cross##a##i##b * cross##a##j##b > 0;

    calc_v(0, 1);
    calc_v(0, 2);
    calc_v(0, 3);
    dcalc_v(1, 2);
    dcalc_v(1, 3);
    dcalc_v(2, 3);
    calc_side(0, 1, 2, 3);
    calc_side(0, 2, 3, 1);
    calc_side(0, 3, 1, 2);
    return side01 && side02 && side03;

#undef calc_side
#undef dcalc_v

}

#undef calc_cross
#undef calc_v
#undef calc_vi

Img drawTriangle(Img img, const Points points, const Triangles triangles,
                 const unsigned char *inputColor) {
    static const unsigned char defaultColor[] = { 255, 255, 255 };
    const unsigned char *color = inputColor == NULL ? defaultColor : inputColor;
    for (Triangles::const_iterator it = triangles.begin();
            it != triangles.end(); it++) {
        for (int i = 0; i < 3; i++) {
            Point p1 = points[it->i[i]];
            Point p2 = points[it->i[(i + 1) % 3]];
            img.draw_line(p1.x, p1.y, p2.x, p2.y, color);
        }
    }
    return img;
}

Img drawPoint(Img img, const Points p, int radius,
              const unsigned char *inputColor) {
    static const unsigned char defaultColor[] = { 255, 255, 255 };
    const unsigned char *color = inputColor == NULL ? defaultColor : inputColor;
    for (Points::const_iterator it = p.begin(); it != p.end(); it++) {
        img.draw_circle(it->x, it->y, radius, color);
    }
    return img;
}

// 对row*row的矩阵求解，最后一列是。。。
void solve(double **m, const int row, const int column) {
    for (int i = 0; i < row; i++) {
        if (m[i][i] == 0) {
            int i2;
            for (i2 = i + 1; i2 < row; i2++)
                if (m[i2][i] != 0)
                    break;
            if (i2 == row)
                return;  // todo no solution
            for (int j = 0; j < column; j++) {  // 交换两行
                double temp = m[i][j];
                m[i][j] = m[i2][j];
                m[i2][j] = temp;
            }
        }
        for (int j = column - 1; j >= i; j--)
            m[i][j] /= m[i][i];
        for (int i2 = i + 1; i2 < row; i2++)
            if (m[i2][i] != 0)
                for (int j = column - 1; j >= i; j--)
                    m[i2][j] -= m[i2][i] * m[i][j];
    }
    for (int i = row - 2; i >= 0; i--)
        for (int j = i + 1; j < row; j++) {
            for (int k = row; k < column; k++)
                m[i][k] -= m[i][j] * m[j][k];
            m[i][j] = 0;
        }
}

// 输入3对点，算出仿射变换矩阵
Mat affineMat(int *sx, int *sy, int *dx, int *dy) {
    Mat mat;
    double mm[3][4] = { };
    double *m[] = { mm[0], mm[1], mm[2] };
    for (int i = 0; i < 3; i++) {
        m[i][0] = sx[i];
        m[i][1] = sy[i];
        m[i][2] = 1;
        m[i][3] = dx[i];
    }
    solve(m, 3, 4);
    for (int i = 0; i < 3; i++)
        mat[0][i] = m[i][3];
    for (int i = 0; i < 3; i++) {
        m[i][0] = sx[i];
        m[i][1] = sy[i];
        m[i][2] = 1;
        m[i][3] = dy[i];
    }
    solve(m, 3, 4);
    for (int i = 0; i < 3; i++)
        mat[1][i] = m[i][3];
    mat[2][0] = 0;
    mat[2][1] = 0;
    mat[2][2] = 1;
    return mat;
}

}
