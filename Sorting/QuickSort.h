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
            return start;
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
    return start;
}

template<class T, typename Comp>
T *QuickSort2(T *start, T *end, Comp func) {
    T *front = start + 1, *back = end - 1;
    while (1) {
        while (front < back && (*func)(*front, *start))
            front++;
        while (front < back && !(*func)(*back, *start))
            back--;
        if (front >= back)
            break;
        T tem = *front;
        *front = *back;
        *back = tem;
        front++;
    }
    if (front == end - 1) {
        T tem = *front;
        *front = *start;
        *start = tem;
    }
    if (start + 1 < front)
        QuickSort(start, front, func);
    if (front + 1 < end)
        QuickSort(front, end, func);
    return start;
}

template<class T, typename Comp>
void QuickSort3(T *start, T *end, Comp func) {
    T *front = start, *back = end - 1, *mark = start;
    if ((*func)(*back, *start)) {
        T tem = *back;
        *back = *start;
        *start = tem;
    }
    while (1) {
        for (; back > mark; back--) {
            if ((*func)(*back, *mark)) {
                T tmp = *back;
                *back = *mark;
                *mark = tmp;
                mark = back;
                back--;
                break;
            }
        }
        if (mark >= front)
            break;
        for (; front < mark; front++) {
            if ((*func)(*front, *mark)) {
                T tmp = *front;
                *front = *mark;
                *mark = tmp;
                mark = front;
                front++;
                break;
            }
        }
        if (mark <= back)
            break;
    }
    if (front == start)
        front++;
    if (start + 1 < front)
        QuickSort3(start, front, func);
    if (front + 1 < end)
        QuickSort3(front, end, func);
}

}

#endif /* QUICKSORT_H_ */
