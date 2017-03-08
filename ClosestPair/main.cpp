/*
 * main.cpp
 *
 *  Created on: 2016Äê11ÔÂ30ÈÕ
 *      Author: Sine
 */

#include <fstream>
#include <iostream>

int main() {
    std::ifstream ifs("1.txt");
    std::cout << ifs.is_open();
    return 0;
}
