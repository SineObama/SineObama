/*
 * A4.h
 *
 *  Created on: 2017Äê3ÔÂ27ÈÕ
 *      Author: Sine
 */

#ifndef A4_H_
#define A4_H_

#include "CImg.h"

class A4 {
    typedef cimg_library::CImg<unsigned char> Img;
    typedef double hough_t;
    typedef cimg_library::CImg<hough_t> Hough;
 public:
    A4(bool showHough, bool showLocalMax);
    void operator()(const char *edgeName, const double precision,
                    const double threshold);
 private:
    struct Point {
        hough_t weight;
        int x, y;
    };
    bool showHough, showLocalMax;
    static int findAllLocalMax(const Hough &hough, const hough_t threshold,
                               Point * const points);
    static Point getLocalMax(const Hough &hough, const int width,
                             const int height, const int x, const int y,
                             const hough_t threshold, bool ** const checked);
};

#endif /* A4_H_ */
