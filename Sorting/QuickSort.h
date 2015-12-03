/*
 * QuickSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

namespace Sine {

template<class T>
T *QuickSort(T *s, std::size_t len, bool (*func)(const T &, const T &)) {
    T *mark = s, *start = s + 1, *end = s + len - 1;
//    for (int i = 0; i < len; i++)
//        printf("%d ", s[i]);
//    printf("in\n");
    std::size_t count0 = 0, count1 = 0;
    for (std::size_t i = 0; i < len; i++) {
        if ((*func)(s[i], *mark))
            count1++;
        else
            count0++;
        if (count0 && count1)
            break;
    }
    if (!count0 || !count1) {
        T *src = mark;
        while (++mark < s + len)
            if ((*func)(*mark, *src) ^ (*func)(*src, *mark))
                break;
        if (mark >= s + len)
            return s;
    }
    T tem = *mark;
    *mark = *s;
    *s = tem;
    mark = s;
    while (1) {
        while (start < end && (*func)(*start, *mark))
            start++;
        while (start < end && !(*func)(*end, *mark))
            end--;
        if (start >= end)
            break;
        T tem = *start;
        *start = *end;
        *end = tem;
        start++;
    }
    std::size_t len1 = start - s;
    std::size_t len2 = len - len1;
    if (len1 > 1 && len1 != len)
        QuickSort(s, len1, func);
    if (len2 > 1 && len2 != len)
        QuickSort(s + len1, len2, func);
//    for (int i = 0; i < len; i++)
//        printf("%d ", s[i]);
//    printf("out\n");
    return s;
}

}

#endif /* QUICKSORT_H_ */
