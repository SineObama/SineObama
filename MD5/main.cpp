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

void test(const char *str) {
    MD5 a;
    a.encrypt((MD5::data_t *)str, strlen(str) * 8);
    std::cout << str << "->" << a.get() << std::endl;
    return;
}

int main() {
    test("");
    test("1234567890123456789012345678901212345678901234567890123");
    test("12345678901234567890123456789012123456789012345678901234");
    std::string s("1");
    s += '\004';
    test(s.c_str());

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
