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
    for (char i = '2'; i <='2';i++) {
        char s[100]={"1.bmp"};
        s[0] = i;
        SrcImg.load_bmp(s);
        SrcImg.display();
        MyCanny::myCanny(SrcImg, 2.5f, 7.5f, 2.0f, 16, 1).display();
    }
}

void tem(const CImg<unsigned char> & a) {}

int main(int argc, _TCHAR* argv[])
{
	Ex2();
    CImg<unsigned char> SrcImg;
	tem(SrcImg);

	return 0;
}

