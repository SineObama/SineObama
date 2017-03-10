// Test_Vector.cpp : Defines the entry point for the console application.
//



#include <vector>
#include <algorithm>

#include <iostream>
#include "MyCanny.h"

using namespace cimg_library;
using namespace std;

void Ex2()
{
    for (char i = '2'; i <='5';i++) {
        char s[100]={"1.bmp"};
        s[0] = i;
        MyCanny::myCanny(s, 1, 3, 2.0f, 16, 0);
    }
}

int main(int argc, _TCHAR* argv[])
{
	Ex2();

	return 0;
}

