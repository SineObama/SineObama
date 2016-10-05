/*
 * md5.cpp
 *
 *  Created on: 2016��10��4��
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

    // �ѽ�������Ӹĳɾ�̬���Ӳ���-static
    // ��ֵ����⣺���㲻ִ������ĳһ�д��룬ֻҪ�����ˣ��ͻᵼ����eclipse����ʱ�����Է�0�˳�������������׼�����
    // �����⣺
//    std::string s;
    // �����⣺
//    std::string("d4");
//    std::string s("d4");
//    s = std::string("d4");
//    s += "d4";
//    *p = "d4";
//    std::string *p = new std::string();
//    p = new std::string("d4");

    return 0;
}
