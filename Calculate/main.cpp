/*
 * main.cpp
 *
 *  Created on: 2015年3月24日
 *      Author: Sine
 */

#include "Header.h"
#include "hugenum.h"
#define MAXLEN 100000
#define str(x) #x

int main() {
//    char s[MAXLEN], result[MAXLEN];
//    while (1) {
//        memset(s, 0, MAXLEN);
//        print_messsage();
//        get_input(s, MAXLEN);
//        Calculate(s, result);
//        output(result);
//    }

//    hugenum a(str(0.0000000221));
//    hugenum b(str(0.0003));
//    a /= b;
//    a /= b;
//    a.print();
    hugenum b = radical(hugenum(200), 2);
//    (b).print();

//    hugenum c(str(2222222322222222222222222222222222222222222222222222222222));
//    hugenum d(str(-1.000000000000000000000000000000000000000000000000000000012));
//    c += d;
//    c.print();

    fflush(stdout);
    return 0;
}

/*
 * 准备更新底层代码：（逗比）
 * 用于比较大小的精度
 * 用于输出的可选精度
 * 用于计算的精度
 * 保留的数据长度
 */
