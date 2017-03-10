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

/*
 C version. Based on Java code by
 * Tom Gibara
 */

#define ffabs(x) ( (x) >= 0 ? (x) : -(x) )
#define GAUSSIAN_CUT_OFF 0.005f
#define MAGNITUDE_SCALE 100.0f
#define MAGNITUDE_LIMIT 1000.0f
#define MAGNITUDE_MAX ((int) (MAGNITUDE_SCALE * MAGNITUDE_LIMIT))

class MyCanny {

 public:

    static void myCanny(const char *filename);
    static void myCanny(const char *filename, float lowthreshold,
                        float highthreshold, float gaussiankernelradius,
                        int gaussiankernelwidth, int contrastnormalised);

 private:

    MyCanny();
    virtual ~MyCanny();

    typedef cimg_library::CImg<unsigned char> Img;
    typedef cimg_library::CImg<int> IntMat;
    typedef struct {
        unsigned char *data; /* input image */
        float *xConv; /* temporary for convolution in x direction */
        float *yConv; /* temporary for convolution in y direction */
        float *xGradient; /* gradients in x direction, as detected by Gaussians */
        float *yGradient; /* gradients in x direction,a s detected by Gaussians */
    } CANNY;

    static CANNY *allocatebuffers(unsigned char *grey, int width, int height);
    static void killbuffers(CANNY *can);
    static IntMat computeGradients(Img, float kernelRadius, int kernelWidth);
    static IntMat performHysteresis(const IntMat &mat, int low, int high);
    static void follow(IntMat &idata, const IntMat &mat, int x1, int y1, int i1,
                       int threshold);

    static Img toGreyScale(Img);
    static void normalizeContrast(Img data, int width, int height);
    static float hypotenuse(float x, float y);
    static float gaussian(float x, float sigma);

};

void MyCanny::myCanny(const char *filename) {
    myCanny(filename, 2.5f, 7.5f, 2.0f, 16, 0);
}

void MyCanny::myCanny(const char *filename, float lowthreshold,
                      float highthreshold, float gaussiankernelradius,
                      int gaussiankernelwidth, int contrastnormalised) {
    Img img(filename);
    img.display("source");
    if (img.spectrum() != 1) {
        img = toGreyScale(img);
        img.display("greyscale");
    }

    int w = img.width(), h = img.height();
    if (contrastnormalised) {
        normalizeContrast(img, w, h);
        img.display("contrastnormalised");
    }

    IntMat mat = computeGradients(img, gaussiankernelradius,
                                  gaussiankernelwidth);
    IntMat idata = performHysteresis(mat,
                                     (lowthreshold * MAGNITUDE_SCALE + 0.5f),
                                     (highthreshold * MAGNITUDE_SCALE + 0.5f));

    Img answer(w, h, 1, 1);
    for (int i = 0; i < w * h; i++)
        answer[i] = idata[i] > 0 ? 1 : 0;
    answer.display("final");

    return;
}

/*
 buffer allocation
 */
MyCanny::CANNY *MyCanny::allocatebuffers(unsigned char *grey, int width,
                                         int height) {
    CANNY *answer;

    answer = (CANNY*) malloc(sizeof(CANNY));
    if (!answer)
        goto error_exit;
    answer->data = (unsigned char *) malloc(width * height);
    answer->xConv = (float *) malloc(width * height * sizeof(float));
    answer->yConv = (float *) malloc(width * height * sizeof(float));
    answer->xGradient = (float *) malloc(width * height * sizeof(float));
    answer->yGradient = (float *) malloc(width * height * sizeof(float));
    if (!answer->data || !answer->xConv || !answer->yConv || !answer->xGradient
            || !answer->yGradient)
        goto error_exit;

    memcpy(answer->data, grey, width * height);

    return answer;
    error_exit: killbuffers(answer);
    return 0;
}

/*
 buffers destructor
 */
void MyCanny::killbuffers(CANNY *can) {
    if (can) {
        free(can->data);
        free(can->xConv);
        free(can->yConv);
        free(can->xGradient);
        free(can->yGradient);
    }
}

/* NOTE: The elements of the method below (specifically the technique for
 non-maximal suppression and the technique for gradient computation)
 are derived from an implementation posted in the following forum (with the
 clear intent of others using the code):
 http://forum.java.sun.com/thread.jspa?threadID=546211&start=45&tstart=0
 My code effectively mimics the algorithm exhibited above.
 Since I don't know the providence of the code that was posted it is a
 possibility (though I think a very remote one) that this code violates
 someone's intellectual property rights. If this concerns you feel free to
 contact me for an alternative, though less efficient, implementation.
 */

MyCanny::IntMat MyCanny::computeGradients(Img img, float kernelRadius,
                                          int kernelWidth) {
    int width = img.width(), height = img.height();
    CANNY *can = allocatebuffers(img, width, height);
    float *kernel;
    float *diffKernel;
    int kwidth;

    int initX;
    int maxX;
    int initY;
    int maxY;

    int x, y;
    int i;
    int flag;

    kernel = (float *) malloc(kernelWidth * sizeof(float));
    diffKernel = (float *) malloc(kernelWidth * sizeof(float));
    if (!kernel || !diffKernel)
        throw;

    /* initialise the Gaussian kernel */
    for (kwidth = 0; kwidth < kernelWidth; kwidth++) {
        float g1, g2, g3;
        g1 = gaussian((float) kwidth, kernelRadius);
        if (g1 <= GAUSSIAN_CUT_OFF && kwidth >= 2)
            break;
        g2 = gaussian(kwidth - 0.5f, kernelRadius);
        g3 = gaussian(kwidth + 0.5f, kernelRadius);
        kernel[kwidth] = (g1 + g2 + g3) / 3.0f
                / (2.0f * (float) 3.14 * kernelRadius * kernelRadius);
        diffKernel[kwidth] = g3 - g2;
    }

    initX = kwidth - 1;
    maxX = width - (kwidth - 1);
    initY = width * (kwidth - 1);
    maxY = width * (height - (kwidth - 1));

    /* perform convolution in x and y directions */
    for (x = initX; x < maxX; x++) {
        for (y = initY; y < maxY; y += width) {
            int index = x + y;
            float sumX = can->data[index] * kernel[0];
            float sumY = sumX;
            int xOffset = 1;
            int yOffset = width;
            while (xOffset < kwidth) {
                sumY += kernel[xOffset]
                        * (can->data[index - yOffset]
                                + can->data[index + yOffset]);
                sumX += kernel[xOffset]
                        * (can->data[index - xOffset]
                                + can->data[index + xOffset]);
                yOffset += width;
                xOffset++;
            }

            can->yConv[index] = sumY;
            can->xConv[index] = sumX;
        }
    }
    free(kernel);
    free(diffKernel);

    for (x = initX; x < maxX; x++) {
        for (y = initY; y < maxY; y += width) {
            float sum = 0.0f;
            int index = x + y;
            for (i = 1; i < kwidth; i++)
                sum += diffKernel[i]
                        * (can->yConv[index - i] - can->yConv[index + i]);
            can->xGradient[index] = sum;
        }

    }

    for (x = kwidth; x < width - kwidth; x++) {
        for (y = initY; y < maxY; y += width) {
            float sum = 0.0f;
            int index = x + y;
            int yOffset = width;
            for (i = 1; i < kwidth; i++) {
                sum += diffKernel[i]
                        * (can->xConv[index - yOffset]
                                - can->xConv[index + yOffset]);
                yOffset += width;
            }

            can->yGradient[index] = sum;
        }

    }

    IntMat magnitude(width, height, 1, 1);
    initX = kwidth;
    maxX = width - kwidth;
    initY = width * kwidth;
    maxY = width * (height - kwidth);
    for (x = initX; x < maxX; x++) {
        for (y = initY; y < maxY; y += width) {
            int index = x + y;
            int indexN = index - width;
            int indexS = index + width;
            int indexW = index - 1;
            int indexE = index + 1;
            int indexNW = indexN - 1;
            int indexNE = indexN + 1;
            int indexSW = indexS - 1;
            int indexSE = indexS + 1;

            float xGrad = can->xGradient[index];
            float yGrad = can->yGradient[index];
            float gradMag = hypotenuse(xGrad, yGrad);

            /* perform non-maximal supression */
            float nMag = hypotenuse(can->xGradient[indexN],
                                    can->yGradient[indexN]);
            float sMag = hypotenuse(can->xGradient[indexS],
                                    can->yGradient[indexS]);
            float wMag = hypotenuse(can->xGradient[indexW],
                                    can->yGradient[indexW]);
            float eMag = hypotenuse(can->xGradient[indexE],
                                    can->yGradient[indexE]);
            float neMag = hypotenuse(can->xGradient[indexNE],
                                     can->yGradient[indexNE]);
            float seMag = hypotenuse(can->xGradient[indexSE],
                                     can->yGradient[indexSE]);
            float swMag = hypotenuse(can->xGradient[indexSW],
                                     can->yGradient[indexSW]);
            float nwMag = hypotenuse(can->xGradient[indexNW],
                                     can->yGradient[indexNW]);
            float tmp;
            /*
             * An explanation of what's happening here, for those who want
             * to understand the source: This performs the "non-maximal
             * supression" phase of the Canny edge detection in which we
             * need to compare the gradient magnitude to that in the
             * direction of the gradient; only if the value is a local
             * maximum do we consider the point as an edge candidate.
             *
             * We need to break the comparison into a number of different
             * cases depending on the gradient direction so that the
             * appropriate values can be used. To avoid computing the
             * gradient direction, we use two simple comparisons: first we
             * check that the partial derivatives have the same sign (1)
             * and then we check which is larger (2). As a consequence, we
             * have reduced the problem to one of four identical cases that
             * each test the central gradient magnitude against the values at
             * two points with 'identical support'; what this means is that
             * the geometry required to accurately interpolate the magnitude
             * of gradient function at those points has an identical
             * geometry (upto right-angled-rotation/reflection).
             *
             * When comparing the central gradient to the two interpolated
             * values, we avoid performing any divisions by multiplying both
             * sides of each inequality by the greater of the two partial
             * derivatives. The common comparand is stored in a temporary
             * variable (3) and reused in the mirror case (4).
             *
             */
            flag = ((xGrad * yGrad <= 0.0f) /*(1)*/
            ? ffabs(xGrad) >= ffabs(yGrad) /*(2)*/
            ? (tmp = ffabs(xGrad * gradMag))
                    >= ffabs(yGrad * neMag - (xGrad + yGrad) * eMag) /*(3)*/
            && tmp > fabs(yGrad * swMag - (xGrad + yGrad) * wMag) /*(4)*/
            :
              (tmp = ffabs(yGrad * gradMag))
                      >= ffabs(xGrad * neMag - (yGrad + xGrad) * nMag) /*(3)*/
              && tmp > ffabs(xGrad * swMag - (yGrad + xGrad) * sMag) /*(4)*/
                    : ffabs(xGrad) >= ffabs(yGrad) /*(2)*/
                    ? (tmp = ffabs(xGrad * gradMag))
                            >= ffabs(yGrad * seMag + (xGrad - yGrad) * eMag) /*(3)*/
                    && tmp > ffabs(yGrad * nwMag + (xGrad - yGrad) * wMag) /*(4)*/
                    :
                      (tmp = ffabs(yGrad * gradMag))
                              >= ffabs(xGrad * seMag + (yGrad - xGrad) * sMag) /*(3)*/
                      && tmp > ffabs(xGrad * nwMag + (yGrad - xGrad) * nMag) /*(4)*/
            );
            if (flag) {
                magnitude[index] =
                        (gradMag >= MAGNITUDE_LIMIT) ?
                                MAGNITUDE_MAX :
                                (int) (MAGNITUDE_SCALE * gradMag);
                /*NOTE: The orientation of the edge is not employed by this
                 implementation. It is a simple matter to compute it at
                 this point as: Math.atan2(yGrad, xGrad); */
            } else {
                magnitude[index] = 0;
            }
        }
    }
    killbuffers(can);
    return magnitude;
}

/*
 we follow edges. high gives the parameter for starting an edge,
 how the parameter for continuing it.
 */
MyCanny::IntMat MyCanny::performHysteresis(const IntMat &magnitude, int low,
                                           int high) {

    int w = magnitude.width(), h = magnitude.height();
    IntMat idata(w, h, 1, 1);

    for (int offset = 0, y = 0; y < h; y++)
        for (int x = 0; x < w; x++, offset++)
            if (idata[offset] == 0 && magnitude[offset] >= high)
                follow(idata, magnitude, x, y, offset, low);
    return idata;
}

/*
 recursive portion of edge follower
 */

void MyCanny::follow(IntMat &idata, const IntMat &mat, int x1, int y1, int i1,
                     int threshold) {
    int x, y;
    int x0 = x1 == 0 ? x1 : x1 - 1;
    int x2 = x1 == mat.width() - 1 ? x1 : x1 + 1;
    int y0 = y1 == 0 ? y1 : y1 - 1;
    int y2 = y1 == mat.height() - 1 ? y1 : y1 + 1;

    idata[i1] = mat[i1];
    for (x = x0; x <= x2; x++) {
        for (y = y0; y <= y2; y++) {
            int i2 = x + y * mat.width();
            if ((y != y1 || x != x1) && idata[i2] == 0 && mat[i2] >= threshold)
                follow(idata, mat, x, y, i2, threshold);
        }
    }
}

MyCanny::Img MyCanny::toGreyScale(Img img) {
    Img grey(img.width(), img.height(), 1, 1);
    cimg_forXY(grey, x, y)
    {
        int r = img(x, y, 0);
        int g = img(x, y, 1);
        int b = img(x, y, 2);
        grey(x, y) = (r * 0.2126 + g * 0.7152 + b * 0.0722);
    }
    return grey;
}

void MyCanny::normalizeContrast(Img data, int width, int height) {
    int histogram[256] = { 0 };
    int remap[256];
    int sum = 0;
    int j = 0;
    int k;
    int target;
    int i;

    for (i = 0; i < width * height; i++)
        histogram[data[i]]++;

    for (i = 0; i < 256; i++) {
        sum += histogram[i];
        target = (sum * 255) / (width * height);
        for (k = j + 1; k <= target; k++)
            remap[k] = i;
        j = target;
    }

    for (i = 0; i < width * height; i++)
        data[i] = remap[data[i]];
}

float MyCanny::hypotenuse(float x, float y) {
    return (float) sqrt(x * x + y * y);
}

float MyCanny::gaussian(float x, float sigma) {
    return (float) exp(-(x * x) / (2.0f * sigma * sigma));
}

#endif /* MYCANNY_H_ */
