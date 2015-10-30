/*
 * class.cpp
 *
 *  Created on: 2015年3月15日
 *      Author: Sine
 */

#include "include.h"

class numCal {
 public:
    numCal(int[], int);  // 构造函数，形参分别是整形数组、数组长度
    numCal(const numCal&);  // 拷贝构造函数
    ~numCal();  // 析构函数，要完成释放内存的操作
    int sum();  // 求动态数组arr中size个元素值和
    numCal& operator=(const numCal&);  // =运算符重载
    numCal& operator++();  // 前序++运算符重载，使得++A后，A.arr的每个元素值加1(A为numCal类型的对象，下同)；

    numCal operator++(int);  // 后序++运算符重载，使得A++后，A.arr的每个元素值加1。
 private:
    int size;  // 数组arr元素个数
    int *arr;  // 动态数组
};

numCal::numCal(int s[], int len = 0) {
    size = len;
    int i;
    arr = new int[size];
    for (i = 0; i < size; i++) {
        arr[i] = s[i];
    }
    return;
}

numCal::numCal(const numCal &resource) {
    *this = resource;
    return;
}

numCal::~numCal() {
    delete[] arr;
}

int numCal::sum() {
    int i, sum = 0;
    for (i = 0; i < size; i++) {
        sum += arr[i];
    }
    return sum;
}

//numCal& numCal::operator=(const numCal &resource) {
//
//}
//
//numCal& numCal::operator++() {
//
//}
//
//numCal numCal::operator++(int) {
//
//}
