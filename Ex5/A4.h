/*
 * A4.h
 *
 *  Created on: 2017年3月27日
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
    struct Eqution {
        double cos, sin, p;
    };
    typedef std::vector<Eqution> Equtions;
    struct Point {
        int x, y;
    };
    typedef std::vector<Point> Points;

    A4(bool showHough, bool showLocalMax, bool showEquation);

    Img operator()(const Img &src, const Img &edge, double precision = 0.2,
                   double scale = 0.4);

    // 1. 根据边缘图计算Hough space
    Hough houghSpace(const Img &edge, double precision = 0.2);
    Hough houghSpace(const Img &edge, int width, int height);
    void displayHough();

    // 2. 从Hough space中找出A4纸4条边和其交点
    Equtions find4Lines(double scale = 0.4);
    void printEquationsAndIntersections();
    void displayLocalMax(double radiusScale = 0.005);
    Points calcPoints();
    Points getPoints();

    // 3. 在图上标出线和交点
    Img drawLinesAndPoints();
    Img drawLinesAndPoints(Img, const unsigned char *inputColor = NULL);

 private:

    struct fPoint {
        double x, y;
    };
    typedef std::vector<fPoint> fPoints;
    struct HoughPointInfo {
        int x, y;
        hough_t v;
    };

    bool showHough, showLocalMax, showEquationAndIntersections;

    int srcWidth, srcHeight;
    double y2p, x2theta;
    Hough hough;

    fPoints houghFPoints;
    Equtions equtions;
    Points intersections;  // 交点

    Hough houghSpace(const Img &edge, const int width, const int height,
                     bool unused);
    static HoughPointInfo getLocalMax(const Hough &hough, const int width,
                                      const int height, const int x,
                                      const int y, const hough_t threshold,
                                      bool ** const checked);
};

#endif /* A4_H_ */
