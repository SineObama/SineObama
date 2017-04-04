/*
 * A4.cpp
 *
 *  Created on: 2017年3月27日
 *      Author: Sine
 */

#include <iostream>
#include "A4.h"

#define PI 3.14159265359

using namespace std;

A4::A4(bool showHough, bool showLocalMax, bool showEquation)
        : showHough(showHough),
          showLocalMax(showLocalMax),
          showEquation(showEquation),
          srcWidth(1),
          srcHeight(1),
          y2p(1),
          x2theta(1) {
}

A4::Hough A4::houghSpace(const Img &edge, double precision) {
    int maxp = sqrt(edge.width() * edge.width() + edge.height() * edge.height())
            + 1;
    y2p = (double) 1 / precision;
    x2theta = 2 * PI / precision / maxp;
    return houghSpace(edge, maxp * precision, maxp * precision, 1);
}

A4::Hough A4::houghSpace(const Img &edge, int width, int height) {
    int maxp = sqrt(edge.width() * edge.width() + edge.height() * edge.height())
            + 1;
    y2p = (double) maxp / height;
    x2theta = 2 * PI / width;
    return houghSpace(edge, width, height, 1);
}

void A4::displayHough() {
    hough.display("hough", false);
}

A4::Img A4::operator()(const Img &edge, double precision, double scale,
                       const Img &src) {
    houghSpace(edge, precision);
    if (showHough)
        displayHough();
    findLines(scale);
    if (showEquation)
        printLinesEquations();
    if (showLocalMax)
        displayLocalMax();
    return drawLinesAndPoints(src);
}

A4::Params A4::findLines(double scale) {
    hough_t maxVote = -1;
    cimg_forXY(hough, x, y)
        if (maxVote < hough(x, y))
            maxVote = hough(x, y);
    const hough_t threshold = maxVote * scale;  // 开始搜索的的阈值
    const int width = hough.width();
    const int height = hough.height();
    // 对横纵坐标都在固定范围内的点视为一条直线
    const int xgap = width * 0.05;
    const int ygap = height * 0.05;
    HoughPointInfo *houghpoints = new HoughPointInfo[width * height / 4];
    bool ** const checked = new bool*[width];  // 标记检测过的点
    for (int i = 0; i < width; i++)
        checked[i] = new bool[height];
    int count = 0;
    cimg_forXY(hough, x, y)
        if (hough(x, y) >= threshold && !checked[x][y]) {
            houghpoints[count] = getLocalMax(hough, width, height, x, y,
                                             threshold, checked);
            bool newpoint = true;
            for (int i = 0; i < count; i++) {
                if ((abs(houghpoints[i].x - houghpoints[count].x) < xgap
                        || abs(abs(houghpoints[i].x - houghpoints[count].x)
                                - width) < xgap)
                        && abs(houghpoints[i].y - houghpoints[count].y)
                                < ygap) {
                    newpoint = false;
                    if (houghpoints[i].v < houghpoints[count].v)
                        houghpoints[i] = houghpoints[count];
                    break;
                }
            }
            if (newpoint)
                count++;
        }
    for (int i = 0; i < width; i++)
        delete[] checked[i];
    delete[] checked;

    // 选最大的4个点
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (houghpoints[i].v < houghpoints[j].v) {
                HoughPointInfo temp = houghpoints[i];
                houghpoints[i] = houghpoints[j];
                houghpoints[j] = temp;
            }
        }
    }
    count = 4;

    houghFPoints.clear();
    for (int i = 0; i < count; i++) {
        fPoint point;
        point.x = houghpoints[i].x + 0.5;
        point.y = houghpoints[i].y + 0.5;
        houghFPoints.push_back(point);
    }
    delete[] houghpoints;

    equtions.clear();
    for (fPoints::const_iterator it = houghFPoints.begin();
            it != houghFPoints.end(); it++) {
        // p = xcosθ + ysinθ
        Param eqution;
        eqution.p = it->y * y2p;
        const double theta = it->x * x2theta;
        eqution.sin = sin(theta);
        eqution.cos = cos(theta);
        equtions.push_back(eqution);
    }

    return equtions;
}

void A4::printLinesEquations() {
    int i = 1;
    if (equtions.size())
        for (Params::iterator it = equtions.begin(); it != equtions.end();
                it++, i++)
            cout << i << ".\t" << it->p << "\t= x * " << it->cos << "\t+ y * "
                 << it->sin << endl;
    else
        cout << "no line" << endl;
}

void A4::displayLocalMax(double radiusScale) {
    Hough tem(hough);
    const int radius =
            tem.height() > tem.width() ?
                    tem.height() * radiusScale : tem.width() * radiusScale;
    for (unsigned int i = 0; i < houghFPoints.size(); i++) {
        hough_t color[] = { tem(houghFPoints[i].x, houghFPoints[i].y) };
        tem.draw_circle(houghFPoints[i].x, houghFPoints[i].y, radius, color);
    }
    tem.display("local max", false);
}

A4::Points A4::calcPoints() {
    intersections.clear();
    for (Params::const_iterator it = equtions.begin(); it != equtions.end() - 1;
            it++) {
        for (Params::const_iterator it2 = it + 1; it2 != equtions.end();
                it2++) {
            const double d = it->cos * it2->sin - it2->cos * it->sin;
            if (d == 0)
                continue;
            const double doublex = (it->p * it2->sin - it2->p * it->sin) / d;
            const double doubley =
                    it->sin != 0 ?
                            (it->p - doublex * it->cos) / it->sin :
                            (it2->p - doublex * it2->cos) / it2->sin;
            const int x = doublex + 0.5, y = doubley + 0.5;
            if (x >= 0 && x < srcWidth && y >= 0 && y < srcHeight) {
                Point point;
                point.x = x;
                point.y = y;
                intersections.push_back(point);
            }
        }
    }
    return intersections;
}

A4::Img A4::drawLinesAndPoints() {
    return drawLinesAndPoints(Img(srcWidth, srcHeight));
}

A4::Img A4::drawLinesAndPoints(Img img, const unsigned char *inputColor) {
    static const unsigned char defaultColor[] = { 255, 255, 255 };
    static const double radiusScale = 0.005;
    const int radius =
            img.height() > img.width() ?
                    img.height() * radiusScale : img.width() * radiusScale;  // 交点圆的半径
    const unsigned char *color = inputColor == NULL ? defaultColor : inputColor;
    // y = kx + b
    // c = ax + by
    // 画线
    for (Params::const_iterator it = equtions.begin(); it != equtions.end();
            it++) {
        if (it->sin != 0) {
            const double k = -it->cos / it->sin;
            const double b = it->p / it->sin;
            // 求出直线与图像左右边界直线的交点
            const int x1 = 0;
            const int y1 = k * (x1 + 0.5) + b + 0.5;
            const int x2 = img.width() - 1;
            const int y2 = k * (x2 + 0.5) + b + 0.5;
            img.draw_line(x1, y1, x2, y2, color);
        } else {
            const int x = it->p / it->cos + 0.5;
            img.draw_line(x, 0, x, img.height() - 1, color);
        }
    }
    // 画点
    for (Points::const_iterator it = intersections.begin();
            it != intersections.end(); it++)
        img.draw_circle(it->x, it->y, radius, color);
    return img;
}

A4::Hough A4::houghSpace(const Img &edge, const int width, const int height,
                         bool unused) {
    srcWidth = edge.width();
    srcHeight = edge.height();
    hough = Hough(width, height);
    hough.fill(0);
    cimg_forXY(edge, x, y)
    {
        hough_t weight = edge(x, y);
        if (weight == 0)
            continue;
        cimg_forX(hough, xx)
        {
            double theta = (xx + 0.5) * x2theta;
            double p = y * sin(theta) + x * cos(theta);
            int yy = p / y2p + 0.5;
            if (yy >= 0)
                hough(xx, yy) += weight;
        }
    }
    return hough;
}

A4::HoughPointInfo A4::getLocalMax(const Hough &hough, const int width,
                                   const int height, const int x, const int y,
                                   const hough_t threshold,
                                   bool ** const checked) {
    static const int direction = 8;
    static const int tx[direction] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    static const int ty[direction] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    HoughPointInfo max;
    max.x = x;
    max.y = y;
    max.v = hough(x, y);
    checked[x][y] = true;
    for (int i = 0; i < direction; i++) {
        const int nx = x + tx[i];
        const int ny = y + ty[i];
        if (nx < 0 || nx >= width || ny < 0 || ny >= height
                || hough(nx, ny) < threshold || checked[nx][ny])
            continue;
        HoughPointInfo p = getLocalMax(hough, width, height, nx, ny, threshold,
                                       checked);
        if (max.v < p.v)
            max = p;
    }
    return max;
}
