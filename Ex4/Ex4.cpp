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
//#include "CImg.h"
#include "A4.h"
#include "MyCanny.h"

//using namespace cimg_library;
using namespace std;

//typedef CImg<unsigned char> Img;

//Img toGreyScale(const Img &img) {
//    Img grey(img.width(), img.height());
//    cimg_forXY(grey, x, y)
//    {
//        int r = img(x, y, 0);
//        int g = img(x, y, 1);
//        int b = img(x, y, 2);
//        grey(x, y) = (r * 0.2126 + g * 0.7152 + b * 0.0722);
//    }
//    return grey;
//}

int main() {
    vector<int> v(1);
    v[0] = 0;
    bool showHough, showLocalMax;
    cout << "enter 2 boolean value for whether show the hough space and local max:\n";
    cin >> showHough >> showLocalMax;
    A4 a4(showHough, showLocalMax);
    cin.sync();
    cout << "enter the filename, threshold scale(0~1) and the precision(0~1) each time in one line.\n";
    while (true) {
        char in[1000] = { 0 };
        double threshold = 0.5, precision = 0.2;
        cin.getline(in, 1000);
        if (in[0] == '\0')
            continue;
        stringstream ss;
        ss << in;
        ss >> in >> threshold >> precision;
        try {
            MyCanny canny(0, 0, 0);
            canny(in).save("temp.bmp");
            a4("temp.bmp", precision, threshold);
        } catch (exception &e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
    return 0;
}
