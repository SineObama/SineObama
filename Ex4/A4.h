/*
 * A4.h
 *
 *  Created on: 2017Äê3ÔÂ27ÈÕ
 *      Author: Sine
 */

#ifndef A4_H_
#define A4_H_

#include <vector>
#include "CImg.h"

class A4 {
 public:

    typedef cimg_library::CImg<unsigned char> Img;
    typedef double hough_t;
    typedef cimg_library::CImg<hough_t> Hough;
    struct Point {
        double x, y;
    };
    typedef std::vector<Point> Points;
    struct Param {
        double cos, sin, p;
    };
    typedef std::vector<Param> Params;

    A4(bool showHough, bool showLocalMax, bool showFunction);

    Img operator()(const Img &edge, double precision, double scale);
    Img operator()(const char *edgeName, double precision, double scale);

    Hough houghSpace(const Img &edge, double precision = 0.2);
    Hough houghSpace(const Img &edge, int width, int height);
    void displayHough();
    Params findLines(double scale = 0.6);
    void printFunctions();
    void displayLocalMax(double radiusScale = 0.003);
    Img drawLine();
    Img drawLine(Img, const unsigned char *color = NULL);

 private:

    struct IntPoint {
        int x, y;
    };
    bool showHough, showLocalMax, showFunction;

    int srcWidth, srcHeight;
    double y2p, x2theta;
    Hough hough;
    Points points;
    Params params;

    Hough houghSpace(const Img &edge, const int width, const int height,
                     bool unused);
    static IntPoint getLocalMax(const Hough &hough, const int width,
                                const int height, const int x, const int y,
                                const hough_t threshold, bool ** const checked);
};

#endif /* A4_H_ */
