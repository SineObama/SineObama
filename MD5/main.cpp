/*
 * md5.cpp
 *
 *  Created on: 2016年10月4日
 *      Author: Sine
 */

#include <iostream>
#include <string>
#include <cstring>
#include <cstdio>
#include "MD5.h"

#define MAXLEN 1000

void test(const char *str) {
    MD5 a;
    a.encrypt((MD5::data_t *) str, strlen(str) * 8);
    std::cout << str << "->" << a.get() << std::endl;
    return;
}

int main() {

    std::cout << "输入要加密为MD5的字符串（少于" << MAXLEN << "个字符），回车结束：\n";
    while (true) {
        std::string in;
        char s[MAXLEN] = {};
        std::cin.getline(s, MAXLEN);
        test(s);
    }

    // 已解决：链接改成静态，加参数-static
    // 奇怪的问题：就算不执行如下某一行代码，只要编译了，就会导致用eclipse运行时程序以非0退出，并看不到标准输出。
    // 无问题：
//    std::string s;
    // 有问题：
//    std::string("d4");
//    std::string s("d4");
//    s = std::string("d4");
//    s += "d4";
//    *p = "d4";
//    std::string *p = new std::string();
//    p = new std::string("d4");

    return 0;
}
