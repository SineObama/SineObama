/*
 * class.cpp
 *
 *  Created on: 2015��3��15��
 *      Author: Sine
 */

#include "include.h"

class numCal {
 public:
    numCal(int[], int);  // ���캯�����βηֱ����������顢���鳤��
    numCal(const numCal&);  // �������캯��
    ~numCal();  // ����������Ҫ����ͷ��ڴ�Ĳ���
    int sum();  // ��̬����arr��size��Ԫ��ֵ��
    numCal& operator=(const numCal&);  // =���������
    numCal& operator++();  // ǰ��++��������أ�ʹ��++A��A.arr��ÿ��Ԫ��ֵ��1(AΪnumCal���͵Ķ�����ͬ)��

    numCal operator++(int);  // ����++��������أ�ʹ��A++��A.arr��ÿ��Ԫ��ֵ��1��
 private:
    int size;  // ����arrԪ�ظ���
    int *arr;  // ��̬����
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
