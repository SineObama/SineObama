/*
 * md5.cpp
 *
 *  Created on: 2016��10��4��
 *      Author: Sine
 */

#include <iostream>
#include <string>
#include "MD5.h"

void test(const char *str) {
    MD5 a;
    a.encrypt((unsigned char *)str, 0);
    std::cout << str << "->" << a.get() << std::endl;
    return;
}

int main() {
    test("");

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
