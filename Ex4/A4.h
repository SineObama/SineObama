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
    struct Param {
        double cos, sin, p;
    };
    typedef std::vector<Param> Params;
    struct Point {
        int x,y;
    };
    typedef std::vector<Point> Points;

    A4(bool showHough, bool showLocalMax, bool showEquation);

    Img operator()(const Img &edge, double precision, double scale,
                   const Img &src);
    Hough houghSpace(const Img &edge, double precision = 0.2);
    Hough houghSpace(const Img &edge, int width, int height);
    void displayHough();

    Params findLines(double scale = 0.4);
    void printLinesEquations();
    void displayLocalMax(double radiusScale = 0.005);
    Points calcPoints();

    Img drawLinesAndPoints();
    Img drawLinesAndPoints(Img, const unsigned char *inputColor = NULL);

 private:

    struct HoughPoint {
        double x, y;
    };
    typedef std::vector<HoughPoint> HoughPoints;
    struct HoughIntPoint {
        int x, y;
        hough_t v;
    };

    bool showHough, showLocalMax, showEquation;

    int srcWidth, srcHeight;
    double y2p, x2theta;
    Hough hough;

    HoughPoints houghPoints;
    Params params;
    Points points;

    Hough houghSpace(const Img &edge, const int width, const int height,
                     bool unused);
    static HoughIntPoint getLocalMax(const Hough &hough, const int width,
                                const int height, const int x, const int y,
                                const hough_t threshold, bool ** const checked);
};

#endif /* A4_H_ */
