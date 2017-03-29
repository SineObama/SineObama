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

#define POSTFIX ".bmp"
#define TEMP_EDGE "temp_edge"

using namespace std;

typedef cimg_library::CImg<unsigned char> Img;

int main() {
    bool showHough, showLocalMax;
    cout << "enter 2 boolean value for whether show the hough space and local max:\n-->";
    cin >> showHough >> showLocalMax;
    cout << "enter the filename, precision and scale each time in one line.\n";
    A4 a4(showHough, showLocalMax);
    MyCanny canny(0, 0, 0);
    string s;
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
             * 11 filename
             * 12 filename lowthreshold highthreshold gaussiankernelradius gaussiankernelwidth contrastnormalised
             *
             * 2  Hough
             * 21 filename
             * 22 filename precision
             * 23 filename width height
             *
             * 3  选择直线
             * 31
             * 32 scale
             *
             * 4  绘制直线
             * 41
             * 42 filename
             * 43 filename red green blue
             */
            if (s[0] == '1') {  // 边缘检测
                ss >> filename;
                Img result;
                if (s[1] == '1') {
                    result = canny((filename + POSTFIX).c_str());
                } else {
                    float lowthreshold = 2.5, highthreshold = 7.5,
                            gaussiankernelradius = 2;
                    int gaussiankernelwidth = 16, contrastnormalised = 0;
                    ss >> lowthreshold >> highthreshold >> gaussiankernelradius
                            >> gaussiankernelwidth >> contrastnormalised;
                    result = canny((filename + POSTFIX).c_str(), lowthreshold,
                                   highthreshold, gaussiankernelradius,
                                   gaussiankernelwidth, contrastnormalised);
                }
                cimg_forXY(result, x, y)
                {
                    if (result(x, y))
                        result(x, y) = 255;
                }
                result.save(TEMP_EDGE POSTFIX);
            } else if (s[0] == '2') {  // 求Hough Space
                ss >> filename;
                if (filename == ".")
                    filename = TEMP_EDGE;
                Img img((filename + POSTFIX).c_str());
                A4::Hough hough;
                if (s[1] == '1') {
                    hough = a4.houghSpace(img);
                } else if (s[1] == '2') {
                    double precision = 0.2;
                    ss >> precision;
                    hough = a4.houghSpace(img, precision);
                } else {
                    int width = 360, height = 500;
                    ss >> width >> height;
                    hough = a4.houghSpace(img, width, height);
                }
                hough.save("temp_hough" POSTFIX);
            } else if (s[0] == '3') {
                if (s[1] == '1') {
                    a4.findLines();
                } else {
                    double scale;
                    ss >> scale;
                    a4.findLines(scale);
                }
            } else if (s[0] == '4') {
                Img img;
                if (s[1] == '1') {
                    img = a4.drawLine();
                } else {
                    ss >> filename;
                    img.load((filename + POSTFIX).c_str());
                    if (s[1] == '2') {
                        img = a4.drawLine(img);
                    } else {
                        unsigned char color[] = { 0, 0, 0 };
                        int tem;
                        ss >> tem;
                        color[0] = tem;
                        ss >> tem;
                        color[1] = tem;
                        ss >> tem;
                        color[2] = tem;
                        img = a4.drawLine(img, color);
                    }
                }
                img.display("result");
                img.save("temp_result" POSTFIX);
            } else {
                cout << "normal:" << endl;
                a4(canny(s.c_str()), precision, threshold);
            }
        } catch (exception &e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
    return 0;
}
