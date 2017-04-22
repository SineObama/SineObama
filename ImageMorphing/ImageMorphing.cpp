/*
 * ImageMorphing.cpp
 *
 *  Created on: 2017��4��20��
 *      Author: Sine
 */

#include <cmath>
#include "ImageMorphing.h"

namespace ImageMorphing {

typedef float calc_t;

// �任����
struct Mat {
    calc_t p[3][3];
    calc_t *operator[](int i) {
        return p[i];
    }
    const calc_t *operator[](int i) const {
        return p[i];
    }
};
Mat affineMat(const int *sx, const int *sy, const int *dx, const int *dy);

typedef cimg_library::CImg<calc_t> Warp;

Imgs deal(Img src, Points s, Img dst, Points d, int frames) {

    const int width = src.width();
    const int height = src.height();
    const int spectrum = src.spectrum();

    Points four;  // �ĸ���
    four.push_back(Point(0, 0));
    four.push_back(Point(width - 1, 0));
    four.push_back(Point(width - 1, height - 1));
    four.push_back(Point(0, height - 1));
    s.insert(s.begin(), four.begin(), four.end());
    d.insert(d.begin(), four.begin(), four.end());

    // �����ʷ�
    const Triangles triangles = divide(s);

    // չʾ���
    drawPointAndTriangle(src, s, 4, triangles).display("�����ʷֽ��", 0).save(
            "triangulation1.bmp");
    drawPointAndTriangle(dst, d, 4, triangles).display("��ӦĿ��ͼ�ʷ�", 0).save(
            "triangulation2.bmp");

    // �������յ�2D warping�������������ӳ��"backward-relative"
    Warp swarp(width, height, 1, 2);
    Warp dwarp(width, height, 1, 2);
    for (Triangles::const_iterator it = triangles.begin();
            it != triangles.end(); it++) {

#define calc_p(l, n) const Point l##p##n = l[it->i[n]]
#define calc_array(l, i) const int l##i[3] = { (l##p##0).i, (l##p##1).i, (l##p##2).i }

        calc_p(s, 0);
        calc_p(s, 1);
        calc_p(s, 2);
        calc_p(d, 0);
        calc_p(d, 1);
        calc_p(d, 2);
        calc_array(s, x);
        calc_array(s, y);
        calc_array(d, x);
        calc_array(d, y);

#undef calc_array
#undef calc_p

#define update(type, op, name, i) \
    if (type##i op (name).i)\
        type##i = (name).i;
#define update_point(type, op, name) \
    update(type, op, name, x);\
    update(type, op, name, y);
#define update_3point(type, op, prefix) \
    update_point(type, op, prefix##0);\
    update_point(type, op, prefix##1);\
    update_point(type, op, prefix##2);
#define calc_warp(f, t) {\
    const Mat f##m = affineMat(t##x, t##y, f##x, f##y);\
    int maxx = 0, minx = width, maxy = 0, miny = height;\
    update_3point(max, <, t##p);\
    update_3point(min, >, t##p);\
    for (int x = minx; x <= maxx; x++)\
        for (int y = miny; y <= maxy; y++)\
    {\
        if (inTriangle(Point(x, y), t##p0, t##p1, t##p2)) {\
            f##warp(x, y, 0, 0) = x - (f##m[0][0] * x + f##m[0][1] * y + f##m[0][2]);\
            f##warp(x, y, 0, 1) = y - (f##m[1][0] * x + f##m[1][1] * y + f##m[1][2]);\
        }\
    }\
}

        calc_warp(s, d);
        calc_warp(d, s);

#undef calc_warp
#undef update_3point
#undef update_point
#undef update

    }  // end for (triangles)

    // ̫��Ͳ�չʾ��
//    drawPointAndTriangle(src.get_warp(swarp, 1), d, 4, triangles).display(
//            "ԭͼ�����α�").save("w1.bmp");
//    drawPointAndTriangle(dst.get_warp(dwarp, 1), s, 4, triangles).display(
//            "Ŀ��ͼ����α�").save("w2.bmp");

    // �����м�֡
    Imgs imgs;
    for (int i = 0; i < frames; i++) {
        const calc_t sscale = (calc_t) (i + 1) / (frames + 1);
        const calc_t dscale = 1 - sscale;
        Img frame(width, height, 1, spectrum);
        Warp swarp_cur(width, height, 1, 2);
        Warp dwarp_cur(width, height, 1, 2);
        cimg_forXY(swarp, x, y)
            cimg_forC(swarp, c)
            {
                swarp_cur(x, y, 0, c) = swarp(x, y, 0, c) * sscale;
                dwarp_cur(x, y, 0, c) = dwarp(x, y, 0, c) * dscale;
            }
        Img sWarpped = src.get_warp(swarp_cur, 1);
        Img dWarpped = dst.get_warp(dwarp_cur, 1);
        cimg_forXY(frame, x, y)
            cimg_forC(frame, c)
                frame(x, y, 0, c) = int(
                        sWarpped(x, y, c) * dscale
                                + dWarpped(x, y, c) * sscale);

        imgs.insert(frame);
    }

    return imgs;
}

// ��Ǳ����ıߣ��±�0/1/2�ֱ�����01/12/20
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
    // ÿ�β���һ����
    for (size_t pi = 4; pi < points.size(); pi++) {
        Triangles found;

        // �ҡ�Ӱ�������Ρ��浽found��
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

        // �������������������޹����ߣ������
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

        // todo ȱ���㷨�еġ��Ż������裬���о�����Ҫ

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
#define calc_v(s, d) calc_vi(s, d, x); calc_vi(s, d, y);
#define calc_cross(a, b, c) const double cross##a##b##c = \
    (v##b##a##x * v##b##c##y - v##b##c##x * v##b##a##y)

// �жϸ��ݣ�Բ�ڽ��ı��εĽǶ����ʡ�
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
// ������������õ��Ļ���ֵ�����������(baXbc)���м������������xyֵ�ͳ���
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

// �������ж�Ŀ���������ε�ÿһ���㣬�Ƿ������������������ֱ��ͬ�ࡣ
bool inTriangle(Point p0, Point p1, Point p2, Point p3) {

#define dcalc_v(s, d) calc_v(s, d); calc_v(d, s);
// �����i��j�Ƿ���ֱ��abͬ��
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

Img drawPoint(Img img, const Points points, int radius,
              const unsigned char *inputColor) {
    static const unsigned char defaultColor[] = { 255, 255, 255 };
    const unsigned char *color = inputColor == NULL ? defaultColor : inputColor;
    for (Points::const_iterator it = points.begin(); it != points.end(); it++)
        img.draw_circle(it->x, it->y, radius, color);
    return img;
}

Img drawPointAndTriangle(Img img, const Points points, int radius,
                         const Triangles triangles,
                         const unsigned char *inputColor) {
    return drawTriangle(drawPoint(img, points, radius, inputColor), points,
                        triangles, inputColor);
}

// ��row*row�ľ�����⣬���һ���ǡ�����
void solve(double **m, const int row, const int column) {
    for (int i = 0; i < row; i++) {
        if (m[i][i] == 0) {
            int i2;
            for (i2 = i + 1; i2 < row; i2++)
                if (m[i2][i] != 0)
                    break;
            if (i2 == row)
                return;  // todo no solution
            for (int j = 0; j < column; j++) {  // ��������
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

// ����3�Ե㣬�������任����
Mat affineMat(const int *sx, const int *sy, const int *dx, const int *dy) {
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
