/*
 * main.cpp
 *
 *  Created on: 2017Äê4ÔÂ20ÈÕ
 *      Author: Sine
 */

#include "ImageMorphing.h"
#include <cmath>
#include <iostream>

using namespace ImageMorphing;

void test_inTriangle() {
    cimg_library::CImg<unsigned char> img(400, 400);
    int x, y;
    std::cin >> x >> y;
    Point p1(x, y);
    std::cin >> x >> y;
    Point p2(x, y);
    std::cin >> x >> y;
    Point p3(x, y);
    cimg_forXY(img, x, y)
    {
        if (inTriangle(Point(x, y), p1, p2, p3)) {
            img(x, y) = 255;
        }
    }
    img.display();
}

int main() {
    test_inTriangle();
//    Points s, d;
//    deal(Img("1.bmp"), s, Img("2.bmp"), d, 11);
}

void test_divide() {
    int n;
    std::cin >> n;
    Points points;
    int width = 0, height = 0;
    std::cin >> width >> height;
    points.push_back(Point(0, 0));
    points.push_back(Point(width - 1, 0));
    points.push_back(Point(width - 1, height - 1));
    points.push_back(Point(0, height - 1));
    for (int i = 0; i < n; i++) {
        int x, y;
        std::cin >> x >> y;
        points.push_back(Point(x, y));
    }
    Triangles triangles = divide(points);
    cimg_library::CImg<unsigned char> img(width, height);
    img.fill(0);
    drawTriangle(drawPoint(img, points, 4), points, triangles).display();
}

void test_inCircle() {
    cimg_library::CImg<unsigned char> img(400, 400);
    int x, y;
    std::cin >> x >> y;
    Point p1(x, y);
    std::cin >> x >> y;
    Point p2(x, y);
    std::cin >> x >> y;
    Point p3(x, y);
    cimg_forXY(img, x, y)
    {
        if (inCircle(Point(x, y), p1, p2, p3)) {
            img(x, y) = 255;
        }
    }
    img.display();
}
