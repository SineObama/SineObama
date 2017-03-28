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

A4::A4(bool showHough, bool showLocalMax)
        : showHough(showHough),
          showLocalMax(showLocalMax) {
}

void A4::operator()(const char *edgeName, const double precision, const double scale) {
    Img edge(edgeName);

    const int width = edge.width();
    const int height = edge.height();
    const int maxp = sqrt(width * width + height * height) + 1;
    Hough hough(maxp * precision, maxp * precision);
    const double yToP = (double) 1 / precision;
    const double xToTheta = 2 * PI / precision / maxp;
    cimg_forXY(edge, x, y)
    {
        hough_t weight = edge(x, y);
        if (weight == 0)
            continue;
        cimg_forX(hough, xx)
        {
            double theta = (xx + 0.5) * xToTheta;
            double p = y * sin(theta) + x * cos(theta);
            int yy = p / yToP + 0.5;
            if (yy >= 0)
                hough(xx, yy) += weight;
        }
    }
    if (showHough)
        hough.display("hough");
    hough.save("hough.bmp");

    hough_t maxVote = -1;
    cimg_forXY(hough, x, y)
        if (maxVote < hough(x, y))
            maxVote = hough(x, y);

    // 选节点
    hough_t threshold = maxVote * scale;  // 开始搜索的的阈值
    Point *points = new Point[hough.width() * hough.height()];
    int count = findAllLocalMax(hough, threshold, points);

    if (count == 0) {
        cout << "points no found" << endl;
    } else {
        for (int i = 0; i < count; i++)
            hough(points[i].x, points[i].y) *= 3;
        if (showLocalMax)
            hough.display("hough - local max");

        // 输出函数，绘制直线
        Img line(width, height);
        for (int i = 0; i < count; i++) {
            // p = xcosθ + ysinθ
            // p - xcosθ = ysinθ
            const double p = (points[i].y + 0.5) * yToP;
            const double theta = (points[i].x + 0.5) * xToTheta;
            const double sine = sin(theta);
            const double cosine = cos(theta);
            cout << (i + 1) << ".\t" << p << "\t= x * " << cosine << "\t+ y * "
                 << sine << endl;
            if (sine != 0) {
                // y = kx + b
                const double k = -cosine / sine;
                const double b = p / sine;
                cimg_forX(line, x)
                {
                    int y = k * (x + 0.5) + b + 0.5;
                    if (y >= 0 && y < height)
                        line(x, y) = 255;
                }
            } else {
                const int x = p / cosine + 0.5;
                cimg_forY(line, y)
                    line(x, y) = 255;
            }
        }
        line.display("line");
        line.save("line.bmp");
    }

    delete[] points;
}

int A4::findAllLocalMax(const Hough &hough, const hough_t threshold,
                        Point * const points) {
    bool ** const checked = new bool*[hough.width()];
    for (int i = 0; i < hough.width(); i++)
        checked[i] = new bool[hough.height()];
    const int width = hough.width();
    const int height = hough.height();
    const int xgap = width * 0.05;
    const int ygap = height * 0.05;
    int count = 0;
    cimg_forXY(hough, x, y)
        if (hough(x, y) >= threshold && !checked[x][y]) {
            points[count] = getLocalMax(hough, width, height, x, y, threshold,
                                        checked);
            // 对横纵坐标都在固定范围内的点视为一条直线
            bool newpoint = true;
            for (int i = 0; i < count; i++) {
                if ((abs(points[i].x - points[count].x) < xgap
                        || abs(abs(points[i].x - points[count].x) - width)
                                < xgap)
                        && abs(points[i].y - points[count].y) < ygap) {
                    newpoint = false;
                    if (points[i].weight < points[count].weight) {
                        points[i] = points[count];
                    }
                    break;
                }
            }
            if (newpoint)
                count++;
        }
    for (int i = 0; i < hough.width(); i++)
        delete[] checked[i];
    delete[] checked;
    return count;
}

A4::Point A4::getLocalMax(const Hough &hough, const int width, const int height,
                          const int x, const int y, const hough_t threshold,
                          bool ** const checked) {
    static const int direction = 8;
    static const int tx[direction] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    static const int ty[direction] = { -1, -1, 0, 1, 1, 1, 0, -1 };
    Point max;
    max.weight = hough(x, y);
    max.x = x;
    max.y = y;
    checked[x][y] = true;
    for (int i = 0; i < direction; i++) {
        const int nx = x + tx[i];
        const int ny = y + ty[i];
        if (nx < 0 || nx >= width || ny < 0 || ny >= height
                || hough(nx, ny) < threshold || checked[nx][ny])
            continue;
        Point p = getLocalMax(hough, width, height, nx, ny, threshold, checked);
        if (max.weight < p.weight)
            max = p;
    }
    return max;
}
