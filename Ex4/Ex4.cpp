// Test_Vector.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <exception>
#include <cmath>
#include "CImg.h"
#include "MyCanny.h"

#define PI 3.141592653

using namespace cimg_library;
using namespace std;

typedef CImg<unsigned char> Img;
typedef double hough_t;
typedef CImg<hough_t> Hough;

Img toGreyScale(const Img &img) {
    Img grey(img.width(), img.height());
    cimg_forXY(grey, x, y)
    {
        int r = img(x, y, 0);
        int g = img(x, y, 1);
        int b = img(x, y, 2);
        grey(x, y) = (r * 0.2126 + g * 0.7152 + b * 0.0722);
    }
    return grey;
}

Hough houghTransform(Img edge, const int precisiontheta, const int precisionp) {
    Hough hough(precisiontheta, precisionp);
    int width = edge.width();
    int height = edge.height();
    const int maxp = sqrt(width * width + height * height) + 0.5;
    double scaley = (double) precisionp / maxp;
    double scalex = 2 * PI / precisiontheta;
    cimg_forXY(edge, x, y)
    {
        hough_t weight = edge(x, y);
        if (weight == 0)
            continue;
        cimg_forX(hough, xx)
        {
            double theta = (xx + 0.5) * scalex;
            double p = y * sin(theta) + x * cos(theta);
            int yy = p * scaley - 0.5;
            if (yy >= 0)
                hough(xx, yy) += weight;
        }
    }
    return hough;
}

void C(Img edge, Hough hough, const int num) {
    struct Point {
        hough_t weight;
        int theta, p;
    };

    // 选候选节点
    hough_t sum = 0;
    cimg_forXY(hough, x, y)
    {
        sum += hough(x, y);
    }
    hough_t value = sum / num / 2;  // 候选节点的阈值
    Point **points = new Point*[hough.width() * hough.height()];
    int count = 0;
    cimg_forXY(hough, x, y)
    {
        if (hough(x, y) >= value) {
            points[count] = new Point();
            points[count]->weight = hough(x, y);
            points[count]->theta = x;
            points[count++]->p = y;
        }
    }

    // 排序
    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (points[i]->weight < points[j]->weight) {
                Point *temp = points[i];
                points[i] = points[j];
                points[j] = temp;
            }
        }
    }

    // 输出函数
    for (int i = 0; i < num; i++) {
        cout << i << ". " << points[i]->p << " = -x * " << cos(points[i]->theta)
             << " + y * " << sin(points[i]->theta) << endl;
    }

    for (int i = 0; i < count; i++)
        delete points[i];
    delete[] points;
}

int main() {
    while (true) {
        char in[1000] = { 0 };
        cin.getline(in, 1000);
        if (in[0] == '\0')
            continue;
//        stringstream ss;
//        ss << in;
//        ss >> in;
        try {
            Img edge(in);
            edge.display("edge");
            Hough hough = houghTransform(edge, 500, 500);
            hough.display("hough");
            hough.save("hough.bmp");
            C(edge, hough, 4);
        } catch (exception &e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
    return 0;
}
