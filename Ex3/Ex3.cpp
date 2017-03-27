// Test_Vector.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <exception>
#include "CImg.h"

using namespace cimg_library;
using namespace std;

typedef CImg<unsigned char> Img;

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

template<class T>
void histogramEqualization(T *p, const int len, const int level) {
    int *histogram = new int[level];
    int *remap = new int[level];
    memset(histogram, 0, sizeof(int) * level);
    for (int i = 0; i < len; i++)
        histogram[p[i]]++;
    int sum = 0;
    for (int i = 0; i < level; i++) {
        sum += histogram[i];
        remap[i] = (sum * (level - 1)) / len;
        cout << remap[i] << endl;
    }
    for (int i = 0; i < len; i++)
        p[i] = remap[p[i]];
    delete[] remap;
    delete[] histogram;
}

Img imgHistogramEqualization(Img src) {
    int len = src.width() * src.height();
    if (src.spectrum() == 1) {  // grey
        histogramEqualization<unsigned char>(src, len, 256);
    } else {
        unsigned char *p = src;
        histogramEqualization<unsigned char>(p, len, 256);
        histogramEqualization<unsigned char>(p + len, len, 256);
        histogramEqualization<unsigned char>(p + len * 2, len, 256);
    }
    return src;
}

int main() {
    cout << "enter the filename and a boolean whether to use greyscale each time:"
         << endl;
    while (true) {
        char in[1000] = { 0 };
        bool grey = true;
        stringstream ss;
        cin.getline(in, 1000);
        if (in[0] == '\0')
            continue;
        ss << in;
        ss >> in >> grey;
        try {
            Img img(in);
            if (!grey) {
                imgHistogramEqualization(img).display();
            } else {
                Img grey = toGreyScale(img);
                grey.display();
                imgHistogramEqualization(grey).display();
            }
        } catch (exception &e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
    return 0;
}
