// Test_Vector.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <algorithm>

#include <iostream>
#include <sstream>
#include <string>
#include <exception>
#include "MyCanny.h"

using namespace cimg_library;
using namespace std;

int main() {
    bool src, grey, contrastnormalised;
    cout << "enter 3 boolean value for whether show the source picture, the greyscale and the contrastnormalised:\n";
    cin >> src >> grey >> contrastnormalised;
    MyCanny myCanny(src, grey, contrastnormalised);
    cin.sync();
    cout << "enter the filename, lowthreshold, highthreshold, gaussiankernelradius, gaussiankernelwidth(integer), contrastnormalised(boolean) each time in one line.\n";
    while (true) {
        char in[1000] = {0};
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
            myCanny(in, lowthreshold, highthreshold, gaussiankernelradius,
                    gaussiankernelwidth, contrastnormalised);
        } catch (exception e) {
            cout << "Exception: " << e.what() << endl;
        }
    }
    return 0;
}

