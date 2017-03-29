// Test_Vector.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include "MyCanny.h"
#include "CImg.h"
#include "canny.h"

using namespace cimg_library;
using namespace std;

int main() {
    CImg<unsigned char> img(200, 100);
    img.fill(0);
    const unsigned char color[] = { 255 };
    img.draw_arrow(-1000, 20, 10000, 40, color);
    img.display();
    img.save("img2.bmp");

    bool src, grey, contrastnormalised;
    cout << "enter 3 boolean value for whether show the source picture, the greyscale and the contrastnormalised:\n";
    cin >> src >> grey >> contrastnormalised;
    MyCanny myCanny(src, grey, contrastnormalised);
    cin.sync();
    cout << "enter the filename, lowthreshold, highthreshold, gaussiankernelradius, gaussiankernelwidth(integer), contrastnormalised(boolean) each time in one line.\n";
    while (true) {
        char in[1000] = { 0 };
        stringstream ss;
        float lowthreshold, highthreshold, gaussiankernelradius;
        int gaussiankernelwidth, contrastnormalised;
        cin.getline(in, 1000);
        if (in[0] == '\0')
            continue;
        ss << in;
        ss >> in >> lowthreshold >> highthreshold >> gaussiankernelradius
                >> gaussiankernelwidth >> contrastnormalised;
        try {
#ifdef use_canny
            CImg<unsigned char> img(in);
            cannyparam(img, img.width(), img.height(), lowthreshold,
                    highthreshold, gaussiankernelradius, gaussiankernelwidth,
                    contrastnormalised);
            img.display();
#else
            CImg<unsigned char> img = myCanny(in, lowthreshold, highthreshold,
                                              gaussiankernelradius,
                                              gaussiankernelwidth,
                                              contrastnormalised);
#endif
            cimg_forXY(img, x, y)
                if (img(x, y))
                    img(x, y) = 255;
            img.save("edge.bmp");
        } catch (exception &e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
    return 0;
}

