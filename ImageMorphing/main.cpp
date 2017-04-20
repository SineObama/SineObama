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

void test_inCircle1() {
    int x, y;
    std::cin >> x >> y;
    Point p0(x, y);
    std::cin >> x >> y;
    Point p1(x, y);
    std::cin >> x >> y;
    Point p2(x, y);
    std::cin >> x >> y;
    Point p3(x, y);
    std::cout << inCircle(p0, p1, p2, p3);
}

void test_inCircle2() {
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

int main() {
    std::cout
            << acos(-1) << std::endl;
    std::cout
            << (((-6) * 94 + (-6) * 94) / sqrt((-6) * (-6) + (-6) * (-6))
                    / sqrt(94 * 94 + 94 * 94)) << std::endl;
    test_inCircle2();
    Points s, d;
    deal(Img("1.bmp"), s, Img("2.bmp"), d, 11);
}
