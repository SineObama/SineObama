/*
 * MyCanny.h
 *
 *  Created on: 2017Äê3ÔÂ8ÈÕ
 *      Author: Sine
 */

/*
 * homework: modify the canny "Code0" to use CImg for implementation
 * by Sine
 */

#ifndef MYCANNY_H_
#define MYCANNY_H_

#include "CImg.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ffabs(x) ( (x) >= 0 ? (x) : -(x) )
#define GAUSSIAN_CUT_OFF 0.005f
#define MAGNITUDE_SCALE 100.0f
#define MAGNITUDE_LIMIT 1000.0f
#define MAGNITUDE_MAX ((int) (MAGNITUDE_SCALE * MAGNITUDE_LIMIT))

class MyCanny {

 public:

    MyCanny(bool src, bool grey, bool contrastnormalised);

    void operator()(const char *filename);
    void operator()(const char *filename, float lowthreshold,
                    float highthreshold, float gaussiankernelradius,
                    int gaussiankernelwidth, int contrastnormalised);

 private:

    bool src, grey, contrastnormalised;

    typedef cimg_library::CImg<unsigned char> Img;
    typedef cimg_library::CImg<int> IntMat;
    typedef cimg_library::CImg<float> FMat;

    static IntMat computeGradients(Img &, float kernelRadius, int kernelWidth);
    static IntMat performHysteresis(const IntMat &mat, int low, int high);
    static void follow(IntMat &idata, const IntMat &mat, int x1, int y1, int i1,
                       int threshold);

    static Img toGreyScale(Img);
    static void normalizeContrast(Img data, int width, int height);
    static float hypotenuse(float x, float y);
    static float gaussian(float x, float sigma);

};

#endif /* MYCANNY_H_ */
