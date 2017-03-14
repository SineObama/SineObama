/*
 * MyCanny.cpp
 *
 *  Created on: 2017年3月11日
 *      Author: Sine
 */

#include "MyCanny.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ffabs(x) ( (x) >= 0 ? (x) : -(x) )
#define GAUSSIAN_CUT_OFF 0.005f
#define MAGNITUDE_SCALE 100.0f
#define MAGNITUDE_LIMIT 1000.0f
#define MAGNITUDE_MAX ((int) (MAGNITUDE_SCALE * MAGNITUDE_LIMIT))

//设置是否显示原图，灰度图，某预处理后的图
MyCanny::MyCanny(bool src, bool grey, bool contrastnormalised)
        : src(src),
          grey(grey),
          contrastnormalised(contrastnormalised) {
}

void MyCanny::operator()(const char *filename) {
    (*this)(filename, 2.5f, 7.5f, 2.0f, 16, 0);
}

void MyCanny::operator()(const char *filename, float lowthreshold,
                         float highthreshold, float gaussiankernelradius,
                         int gaussiankernelwidth, int contrastnormalise) {
    Img img(filename);
    if (src)
        img.display("source");
    if (img.spectrum() != 1) {
        img = toGreyScale(img);
        if (grey)
            img.display("greyscale");
    }

    int w = img.width(), h = img.height();
    if (contrastnormalise) {
        normalizeContrast(img, w, h);
        if (contrastnormalised)
            img.display("contrastnormalised");
    }

    IntMat mat = computeGradients(img, gaussiankernelradius,
                                  gaussiankernelwidth);
    IntMat idata = performHysteresis(mat,
                                     (lowthreshold * MAGNITUDE_SCALE + 0.5f),
                                     (highthreshold * MAGNITUDE_SCALE + 0.5f));

    Img answer(w, h);
    for (int i = 0; i < w * h; i++)
        answer[i] = idata[i] > 0 ? 1 : 0;
    answer.display("final");

    return;
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
MyCanny::IntMat MyCanny::computeGradients(Img &data, float kernelRadius,
                                          int kernelWidth) {
    int width = data.width(), height = data.height();

    FMat xConv(width, height); /* temporary for convolution in x direction */
    FMat yConv(width, height); /* temporary for convolution in y direction */
    FMat xGradient(width, height); /* gradients in x direction, as detected by Gaussians */
    FMat yGradient(width, height); /* gradients in x direction,a s detected by Gaussians */

    int kwidth;

    int initX;
    int maxX;
    int initY;
    int maxY;

    int flag;

    FMat kernel(kernelWidth);
    FMat diffKernel(kernelWidth);

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
    for (int x = initX; x < maxX; x++) {
        for (int y = initY; y < maxY; y += width) {
            int index = x + y;
            float sumX = data[index] * kernel[0];
            float sumY = sumX;
            int xOffset = 1;
            int yOffset = width;
            while (xOffset < kwidth) {
                sumY += kernel[xOffset]
                        * (data[index - yOffset] + data[index + yOffset]);
                sumX += kernel[xOffset]
                        * (data[index - xOffset] + data[index + xOffset]);
                yOffset += width;
                xOffset++;
            }
            yConv[index] = sumY;
            xConv[index] = sumX;
        }
    }

    for (int x = initX; x < maxX; x++) {
        for (int y = initY; y < maxY; y += width) {
            float sum = 0.0f;
            int index = x + y;
            for (int i = 1; i < kwidth; i++)
                sum += diffKernel[i] * (yConv[index - i] - yConv[index + i]);
            xGradient[index] = sum;
        }
    }

    for (int x = initX; x < maxX; x++) {
        for (int y = initY; y < maxY; y += width) {
            float sum = 0.0f;
            int index = x + y;
            for (int i = 1, yOffset = width; i < kwidth; i++, yOffset += width)
                sum += diffKernel[i]
                        * (xConv[index - yOffset] - xConv[index + yOffset]);
            yGradient[index] = sum;
        }
    }

    IntMat magnitude(width, height);
    initX = kwidth;
    maxX = width - kwidth;
    initY = width * kwidth;
    maxY = width * (height - kwidth);
    for (int x = initX; x < maxX; x++) {
        for (int y = initY; y < maxY; y += width) {
            int index = x + y;
            int indexN = index - width;
            int indexS = index + width;
            int indexW = index - 1;
            int indexE = index + 1;
            int indexNW = indexN - 1;
            int indexNE = indexN + 1;
            int indexSW = indexS - 1;
            int indexSE = indexS + 1;

            float xGrad = xGradient[index];
            float yGrad = yGradient[index];
            float gradMag = hypotenuse(xGrad, yGrad);

            /* perform non-maximal supression */
            float nMag = hypotenuse(xGradient[indexN], yGradient[indexN]);
            float sMag = hypotenuse(xGradient[indexS], yGradient[indexS]);
            float wMag = hypotenuse(xGradient[indexW], yGradient[indexW]);
            float eMag = hypotenuse(xGradient[indexE], yGradient[indexE]);
            float neMag = hypotenuse(xGradient[indexNE], yGradient[indexNE]);
            float seMag = hypotenuse(xGradient[indexSE], yGradient[indexSE]);
            float swMag = hypotenuse(xGradient[indexSW], yGradient[indexSW]);
            float nwMag = hypotenuse(xGradient[indexNW], yGradient[indexNW]);
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

void MyCanny::normalizeContrast(Img &data, int width, int height) {
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
