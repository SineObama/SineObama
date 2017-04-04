/*
 * A4Warpping.cpp
 *
 *  Created on: 2017年4月4日
 *      Author: Sine
 */

#include <cmath>
#include "A4Warpping.h"

A4Warpping::A4Warpping() {
}

A4Warpping::Img A4Warpping::operator()(const Img &src, int *x, int *y) {

}

A4Warpping::Img A4Warpping::operator()(const Img &src, int *x, int *y,
                                       int width, int height) {
    // 调整4个点的顺序，从左上角开始，顺时针，对应下图。。。
    /**
     * y
     * |0     1
     * |
     * |        } height
     * |
     * |3     2
     * 0------------x
     *   width
     */
#define dis(a, b) sqrt((x[a] - x[b])*(x[a] - x[b]) + (y[a] - y[b])*(y[a] - y[b]))
#define swap(a, b) { temp = x[a]; x[a] = x[b]; x[b] = temp;\
                     temp = y[a]; y[a] = y[b]; y[b] = temp; }
    // 取前面3个点根据距离排为0,1,2，但有可能存在上下左右的翻转
    int temp;
    double d01 = dis(0, 1);
    double d12 = dis(1, 2);
    double d02 = dis(0, 2);

    if (d01 > d02) {
        swap(1, 2)
        if (d02 > d12)
            swap(0, 2)
        else if (d12 > d01)
            swap(0, 1);
    } else {
        if (d12 > d02)
            swap(0, 1)
        else if (d01 > d12)
            swap(0, 2)
    }

//    if (d01 > d12 && d12 > d02) {
//        swap(1, 2);
//    }
//    if (d01 > d02 && d02 > d12) {
//        swap(1, 2);
//        swap(0, 2);
//    }
//    if (d12 > d01 && d01 > d02) {
//        swap(1, 2);
//        swap(0, 1);
//    }
//    if (d12 > d02 && d02 > d01) {
//        swap(0, 1);
//    }
//    if (d02 > d01 && d01 > d12) {
//        swap(0, 2);
//    }
//    if (d02 > d12 && d12 > d01) {
//    }

// 以x/y坐标判断左右/上下方向，进行翻转
    if (x[0] > x[1]) {
        swap(0, 1);
        swap(2, 3);
    }
    if (y[0] < y[3]) {
        swap(0, 3);
        swap(1, 2);
    }
#undef swap
#undef dis
}
