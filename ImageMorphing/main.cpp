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

void test_deal() {
    int n;
    std::cin >> n;
    Points s, d;
    const int width = 490, height = 700;
    const double scalex = (double) 323 / width, scaley = (double) 399 / height;
    s.push_back(Point(0, 0));
    s.push_back(Point(width - 1, 0));
    s.push_back(Point(width - 1, height - 1));
    s.push_back(Point(0, height - 1));
    d.push_back(Point(0, 0));
    d.push_back(Point(width - 1, 0));
    d.push_back(Point(width - 1, height - 1));
    d.push_back(Point(0, height - 1));
    int x, y;
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;
        if (!std::cin.good()) {
            std::cin.clear();
            char temp[1000];
            std::cin.getline(temp, 1000);
            i--;
            continue;
        }
        s.push_back(Point(x, y));
    }
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;
        if (!std::cin.good()) {
            std::cin.clear();
            char temp[1000];
            std::cin.getline(temp, 1000);
            i--;
            continue;
        }
        d.push_back(Point(x / scalex, y / scaley));
    }
//    Img src(width, height), dst(width, height);
//    cimg_forXY(src, x, y)
//    {
//        src(x, y) = (float) (x + y) / (width + height) * 255;
//        dst(x, y) = (float) (x + y) / (width + height) * 255;
//    }
//    deal(src, s, dst, d, 11);
    Img img2(width, height, 1, 3), img("2.bmp");
    cimg_library::CImg<float> scale(width, height, 1, 2);
    cimg_forXY(img, x, y)
    {
        img2(x, y, 0, 0) = img(x, y, 0, 0);
        img2(x, y, 0, 1) = img(x, y, 0, 1);
        img2(x, y, 0, 2) = img(x, y, 0, 2);
    }
    cimg_forXY(scale, x, y)
    {
        scale(x, y, 0, 0) = x * scalex;
        scale(x, y, 0, 1) = y * scaley;
    }
    img2.warp(scale).display("scale");
    deal(Img("1.bmp"), s ,img2, d, 11);
}

int main() {
    test_deal();
//    Points s, d;
//    deal(Img("1.bmp"), s, Img("2.bmp"), d, 11);
}

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
