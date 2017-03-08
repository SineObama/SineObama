// Test_Vector.cpp : Defines the entry point for the console application.
//



#include <vector>
#include <algorithm>

#include <iostream>
#include "CImg.h"
#include "MyCanny.h"

using namespace cimg_library;
using namespace std;

void Ex2()
{
    CImg<unsigned char> SrcImg;
    for (char i = '2'; i <='5';i++) {
        char s[100]={"1.bmp"};
        s[0] = i;
        SrcImg.load_bmp(s);
        SrcImg.display();
        MyCanny::myCanny(SrcImg).display();
    }
}

int main(int argc, _TCHAR* argv[])
{
	Ex2();

	return 0;
}

