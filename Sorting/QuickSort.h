/*
 * QuickSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>
#include <cassert>

#ifndef QUICKSORT_H_
#define QUICKSORT_H_

namespace Sine {

template<class T, typename Comp>
T *QuickSort(T *start, T *end, Comp func) {
    T *mark = start, *front = start, *back = end - 1;
    bool count0 = false, count1 = false;
    for (T *it = start; it < end; it++) {
        if ((*func)(*it, *mark))
            count1 = true;
        else
            count0 = true;
        if (count0 && count1)
            break;
    }
    if (!count0 || !count1) {
        T *src = mark;
        while (++mark < end)
            if ((*func)(*mark, *src) ^ (*func)(*src, *mark))
                break;
        if (mark >= end)
            return front;
        T tem = *mark;
        *mark = *start;
        *start = tem;
        mark = start;
    }
    while (1) {
        while (front < back && (*func)(*front, *mark))
            front++;
        while (front < back && !(*func)(*back, *mark))
            back--;
        if (front >= back)
            break;
        T tem = *front;
        *front = *back;
        *back = tem;
        front++;
    }
    QuickSort(start, front, func);
    QuickSort(back, end, func);
    return front;
}

template<class T, typename Comp>
T *UnstableQuickSort(T *start, T *end, Comp func) {
    T *mark = start, *front = start, *back = end - 1;
    while (1) {
        while (front < back && (*func)(*front, *mark))
            front++;
        while (front < back && !(*func)(*back, *mark))
            back--;
        if (front >= back)
            break;
        T tem = *front;
        *front = *back;
        *back = tem;
        front++;
    }
    QuickSort(start, front, func);
    QuickSort(back, end, func);
    return front;
}

}

#endif /* QUICKSORT_H_ */
