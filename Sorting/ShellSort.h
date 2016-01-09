/*
 * ShellSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#include <iostream>

#ifndef SHELLSORT_H_
#define SHELLSORT_H_

namespace Sine {

template<class T, typename Comp>
T *ShellSort(T *start, T *end, Comp func) {
    std::size_t len = end - start;
    std::size_t number[50] = { 0, 1 }, max = 2;
    while (number[max - 1] * 4 < len) {
        number[max] = int(number[max - 1] * 3.5 - 1);
        max++;
    }
    for (std::size_t index = max - 1; index > 0; index--) {
        std::size_t far = number[index];
        for (std::size_t j = far; j < len; j++) {
            std::size_t k = j;
            while (k >= far && (*func)(start[k], start[k - far])) {
                T tem = start[k];
                start[k] = start[k - far];
                k -= far;
                start[k] = tem;
            }
        }
    }
    return start;
}

template<class T, typename Comp>
void ShellSort1_help(T *start, int len, int far, Comp func) {
    if (far * 14 < len)
        ShellSort1_help(start, len, far * 3.5 - 1, func);
    for (int j = far; j < len; j++) {
        int k = j;
        while (k >= far && (*func)(start[k], start[k - far])) {
            T tem = start[k];
            start[k] = start[k - far];
            k -= far;
            start[k] = tem;
        }
    }
}

template<class T, typename Comp>
T *ShellSort1(T *start, T *end, Comp func) {
    std::size_t len = end - start;
    ShellSort1_help(start, len, 1, func);
    return start;
}

}

#endif /* SHELLSORT_H_ */
