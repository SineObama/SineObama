/*
 * main.cpp
 *
 *  Created on: 2017年4月20日
 *      Author: Sine
 */

#include "ImageMorphing.h"
#include <cmath>
#include <iostream>
#include <sstream>

using namespace ImageMorphing;

#define BUFFER_SIZE 1000

/*
 * 第一行输入原图和目标图文件名，以及中间帧数，用空格分离
 * 第二行输入n表示特征点的数量
 * 下面n行每行是原图的特征点坐标x和y，用空格分离
 * 后面n行是对应目标图的坐标
 *
 * 目标图将拉伸至原图大小，再进行Image Morphing
 * 中间结果：三角剖分保存在triangulaion.bmp中
 *
 * 输出图像为中间帧（不包含原图和目标图），如11帧则保存到frame1.bmp~frame11.bmp。
 * 拉伸后的目标图保存到frame999.bmp中
 *
 */
int main() {

    char srcName[BUFFER_SIZE], dstName[BUFFER_SIZE];
    int frames = 11;

    std::cin >> srcName >> dstName >> frames;
    Img src(srcName), _dst(dstName);
    const int width = src.width(), height = src.height();
    const double scalex = (double) _dst.width() / width;
    const double scaley = (double) _dst.height() / height;
    Points s, d;
    int x, y;
    int n;

    // 读取特征数据
    std::cin >> n;
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;
        if (!std::cin.good()) {  // 忽略读不出坐标的一行，视为注释
            std::cin.clear();
            char temp[BUFFER_SIZE];
            std::cin.getline(temp, BUFFER_SIZE);
            i--;
            continue;
        }
        s.push_back(Point(x, y));
    }
    for (int i = 0; i < n; i++) {
        std::cin >> x >> y;
        if (!std::cin.good()) {
            std::cin.clear();
            char temp[BUFFER_SIZE];
            std::cin.getline(temp, BUFFER_SIZE);
            i--;
            continue;
        }
        d.push_back(Point(x / scalex, y / scaley));
    }

    // 拉伸目标图，相应修改特征点坐标
    Img dst(width, height, 1, _dst.spectrum());
    cimg_library::CImg<float> scale(width, height, 1, 2);
    cimg_forXY(_dst, x, y)
        cimg_forC(_dst, c)
            dst(x, y, 0, c) = _dst(x, y, 0, c);
    cimg_forXY(scale, x, y)
    {
        scale(x, y, 0, 0) = x * scalex;
        scale(x, y, 0, 1) = y * scaley;
    }
    dst.warp(scale).display("目标图拉伸后");

    // 制作和保存
    Imgs imgs = deal(src, s, dst, d, frames);
    Imgs::iterator it = imgs.begin();
    for (int i = 0; it != imgs.end(); it++, i++) {
        std::stringstream ss;
        ss << "frame" << (i + 1) << ".bmp";
        it->save(ss.str().c_str());
    }
    dst.save("frame999.bmp");
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
