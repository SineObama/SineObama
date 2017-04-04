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
#include <vector>
#include "A4.h"
#include "MyCanny.h"
#include "A4Warpping.h"

#define PREFIX "cached_"
#define POSTFIX ".bmp"
#define DEFAULT_SYMBOL "."

using namespace std;

typedef cimg_library::CImg<unsigned char> Img;

void testWarpping(A4Warpping &a4Warpping, int *x, int *y, int *seq, int index) {
    if (index == 4) {
        int xx[4], yy[4];
        for (int i = 0; i < 4; i++) {
            xx[i] = x[seq[i]];
            yy[i] = y[seq[i]];
        }
        for (int i = 0; i < 4; i++)
            cout << xx[i] << " ";
        for (int i = 0; i < 4; i++)
            cout << yy[i] << " ";
        cout << "   ";
        a4Warpping(Img(), xx, yy, 0, 0);
        for (int i = 0; i < 4; i++)
            cout << xx[i] << " ";
        for (int i = 0; i < 4; i++)
            cout << yy[i] << " ";
        cout << endl;
        return;
    }
    testWarpping(a4Warpping, x, y, seq, index + 1);
    for (int i = 1; i < 4 - index; i++) {
        int temp = seq[index];
        seq[index] = seq[index + i];
        seq[index + i] = temp;
        testWarpping(a4Warpping, x, y, seq, index + 1);
        temp = seq[index];
        seq[index] = seq[index + i];
        seq[index + i] = temp;
    }
}

int main() {
    setbuf(stdout,NULL);

    bool showEdge, showHough, showLocalMax, showEquation;
    cout << "enter 4 boolean value for whether display the edge, the hough space, local max and show the linear equations:\n-->";
    cin >> showEdge >> showHough >> showLocalMax >> showEquation;
    cout << "enter the filename, precision and scale each time in one line.\n";
    A4Warpping a4Warpping;
    A4 a4(showHough, showLocalMax, showEquation);
    MyCanny canny(0, 0, 0);
    string s, srcName;
    Img cached_edge;
    A4::Hough cached_hough;
    Img cached_result;
    while (true) {
        cout << "->";
        cin.sync();
        cin.clear();
        char in[1000] = { 0 };
        double threshold = 0.5, precision = 0.2;
        string filename;
        cin.getline(in, 1000);
        if (in[0] == '\0')
            continue;
        stringstream ss;
        ss << in;
        ss >> s;
        try {
            /**
             * 1  边缘检测
             * 1  filename
             * 1  filename lowthreshold highthreshold gaussiankernelradius gaussiankernelwidth contrastnormalised
             *
             * 2  Hough
             * 2
             * 2  filename
             * 2  filename precision
             * 2  filename width height
             *
             * 3  选择直线
             * 3
             * 3  scale
             *
             * 4  绘制直线
             * 4
             * 4  filename
             * 4  filename red green blue
             *
             * 11,22,44分别保存最近的一次结果
             */
            if (s == "1") {  // 边缘检测
                ss >> srcName;
                if (!ss.good()) {
                    cached_edge = canny((srcName + POSTFIX).c_str(), 2.5f, 7.5f,
                                        7.0f, 16, 0);
                } else {
                    float lowthreshold = 2.5, highthreshold = 7.5,
                            gaussiankernelradius = 7;
                    int gaussiankernelwidth = 16, contrastnormalised = 0;
                    ss >> lowthreshold >> highthreshold >> gaussiankernelradius
                            >> gaussiankernelwidth >> contrastnormalised;
                    cached_edge = canny((srcName + POSTFIX).c_str(),
                                        lowthreshold, highthreshold,
                                        gaussiankernelradius,
                                        gaussiankernelwidth,
                                        contrastnormalised);
                }
                cimg_forXY(cached_edge, x, y)
                {
                    if (cached_edge(x, y))
                        cached_edge(x, y) = 255;
                }
                cached_edge.display("edge", false);
            } else if (s == "2") {  // 求Hough Space
                ss >> filename;
                Img edge;
                if (!ss.good()) {
                    edge = cached_edge;
                    cached_hough = a4.houghSpace(edge);
                } else {
                    edge.load((filename + POSTFIX).c_str());
                    double precision = 0.2;
                    ss >> precision;
                    if (!ss.good()) {
                        cached_hough = a4.houghSpace(edge, precision);
                    } else {
                        int width = precision, height = 500;
                        ss >> height;
                        cached_hough = a4.houghSpace(edge, width, height);
                    }
                }
                a4.displayHough();
            } else if (s == "3") {  // 选出直线方程
                if (!ss.good()) {
                    a4.find4Lines();
                } else {
                    double scale;
                    ss >> scale;
                    a4.find4Lines(scale);
                }
                a4.calcPoints();
                a4.printEquations();
                a4.displayLocalMax();
            } else if (s == "4") {  // 在指定图上绘制直线
                if (!ss.good()) {
                    cached_result = a4.drawLinesAndPoints();
                } else {
                    ss >> filename;
                    if (filename == DEFAULT_SYMBOL)
                        filename = srcName;
                    cached_result.load((filename + POSTFIX).c_str());
                    if (!ss.good()) {
                        cached_result = a4.drawLinesAndPoints(cached_result);
                    } else {
                        unsigned char color[3] = { 0 };
                        ss >> color[0];
                        ss >> color[1];
                        ss >> color[2];
                        cached_result = a4.drawLinesAndPoints(cached_result,
                                                              color);
                    }
                }
                cached_result.display("result", false);
            } else if (s == "11") {
                cached_edge.save(PREFIX"edge"POSTFIX);
            } else if (s == "22") {
                cached_hough.get_normalize(0, 255).save(PREFIX"hough"POSTFIX);
            } else if (s == "44") {
                cached_result.save(PREFIX"result"POSTFIX);
            } else if (s == "555") {  // 测试调整顺序功能
                int x[] = { 0, 2, 2, 0 };
                int y[] = { 3, 3, 0, 0 };
                int seq[] = { 0, 1, 2, 3 };
                testWarpping(a4Warpping, x, y, seq, 0);
            } else {
                ss >> precision >> threshold;
                cached_edge = canny(s.c_str(), 2.5, 7.5, 4, 16, 0);
                if (showEdge)
                    cached_edge.display("edge", false);
                cout << "edge detect complete...\n";
                Img result = a4(cached_edge, precision, threshold,
                                Img(s.c_str()));
                cout << "algorithm complete...\n";
                result.save("result"POSTFIX);
                cout << "result saved.\n";
            }
        } catch (exception &e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
    return 0;
}
