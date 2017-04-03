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
    return drawLine(src);
}

A4::Params A4::findLines(double scale) {

    hough_t maxVote = -1;
    cimg_forXY(hough, x, y)
        if (maxVote < hough(x, y))
            maxVote = hough(x, y);
    hough_t threshold = maxVote * scale;  // 开始搜索的的阈值
    const int width = hough.width();
    const int height = hough.height();
    // 对横纵坐标都在固定范围内的点视为一条直线
    const int xgap = width * 0.05;
    const int ygap = height * 0.05;
    IntPoint *intpoints = new IntPoint[width * height / 4];
    bool ** const checked = new bool*[width];  // 标记检测过的点
    for (int i = 0; i < width; i++)
        checked[i] = new bool[height];
    int count = 0;
    cimg_forXY(hough, x, y)
        if (hough(x, y) >= threshold && !checked[x][y]) {
            intpoints[count] = getLocalMax(hough, width, height, x, y,
                                           threshold, checked);
            bool newpoint = true;
            for (int i = 0; i < count; i++) {
                if ((abs(intpoints[i].x - intpoints[count].x) < xgap
                        || abs(abs(intpoints[i].x - intpoints[count].x) - width)
                                < xgap)
                        && abs(intpoints[i].y - intpoints[count].y) < ygap) {
                    newpoint = false;
                    if (hough(intpoints[i].x, intpoints[i].y)
                            < hough(intpoints[count].x, intpoints[count].y))
                        intpoints[i] = intpoints[count];
                    break;
                }
            }
            if (newpoint)
                count++;
        }
    for (int i = 0; i < width; i++)
        delete[] checked[i];
    delete[] checked;

    if (count == 0) {
        cout << "points no found" << endl;
        return Params();
    }

    points.clear();
    for (int i = 0; i < count; i++) {
        Point point;
        point.x = intpoints[i].x + 0.5;
        point.y = intpoints[i].y + 0.5;
        points.push_back(point);
    }
    delete[] intpoints;

    params.clear();
    for (Points::const_iterator it = points.begin(); it != points.end(); it++) {
        // p = xcosθ + ysinθ
        Param param;
        param.p = it->y * y2p;
        const double theta = it->x * x2theta;
        param.sin = sin(theta);
        param.cos = cos(theta);
        params.push_back(param);
    }

    return params;
}

void A4::printLinesEquations() {
    int i = 1;
    for (Params::iterator it = params.begin(); it != params.end(); it++, i++)
        cout << i << ".\t" << it->p << "\t= x * " << it->cos << "\t+ y * "
             << it->sin << endl;
}

void A4::displayLocalMax(double radiusScale) {
    Hough tem(hough);
    const int radius =
            tem.height() > tem.width() ?
                    tem.height() * radiusScale : tem.width() * radiusScale;
    for (unsigned int i = 0; i < points.size(); i++) {
        hough_t color[] = { tem(points[i].x, points[i].y) };
        tem.draw_circle(points[i].x, points[i].y, radius, color);
        tem(points[i].x, points[i].y) *= 2;
    }
    tem.display("local max", false);
}

A4::Img A4::drawLine() {
    return drawLine(Img(srcWidth, srcHeight));
}

A4::Img A4::drawLine(Img img, const unsigned char *color) {
    static const unsigned char defaultColor[] = { 255, 255, 255 };
    const unsigned char *usedColor = color == NULL ? defaultColor : color;
    // y = kx + b
    // c = ax + by
    for (Params::const_iterator it = params.begin(); it != params.end(); it++) {
        if (it->sin != 0) {
            const double k = -it->cos / it->sin;
            const double b = it->p / it->sin;
            int x1 = 0;
            int y1 = k * (x1 + 0.5) + b + 0.5;
            int x2 = img.width() - 1;
            int y2 = k * (x2 + 0.5) + b + 0.5;
            img.draw_line(x1, y1, x2, y2, usedColor);
        } else {
            int x = it->p / it->cos + 0.5;
            img.draw_line(x, 0, x, img.height(), usedColor);
        }
    }
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

A4::IntPoint A4::getLocalMax(const Hough &hough, const int width,
                             const int height, const int x, const int y,
                             const hough_t threshold, bool ** const checked) {
    static const int direction = 8;
    static const int tx[direction] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    static const int ty[direction] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    IntPoint max;
    max.x = x;
    max.y = y;
    checked[x][y] = true;
    for (int i = 0; i < direction; i++) {
        const int nx = x + tx[i];
        const int ny = y + ty[i];
        if (nx < 0 || nx >= width || ny < 0 || ny >= height
                || hough(nx, ny) < threshold || checked[nx][ny])
            continue;
        IntPoint p = getLocalMax(hough, width, height, nx, ny, threshold,
                                 checked);
        if (hough(max.x, max.y) < hough(p.x, p.y))
            max = p;
    }
    return max;
}
