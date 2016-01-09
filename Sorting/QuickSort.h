/*
 * QuickSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>
#include <cassert>
#include <cmath>
#include <cstdlib>

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
    if (start + 1 < front)
        QuickSort(start, front, func);
    if (front + 1 < end)
        QuickSort(front, end, func);
    return start;
}

template<class T, typename Comp>
T *QuickSort1(T *start, T *end, Comp func) {
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
    if (front == end - 1 && (*func)(*front, *start)) {
        T tem = *front;
        *front = *start;
        *start = tem;
    }
    if (start + 1 < front)
        QuickSort1(start, front, func);
    if (front + 1 < end)
        QuickSort1(front, end, func);
    return start;
}

/* random access form of QuickSort1 */
template<class T, typename Comp>
T *QuickSort1_1(T *start, T *end, Comp func) {
    int n = end - start, i = 1, j = n - 1;
    while (1) {
        while (i < j && (*func)(start[i], start[0]))
            i++;
        while (i < j && !(*func)(start[j], start[0]))
            j--;
        if (i >= j)
            break;
        T tem = start[i];
        start[i] = start[j];
        start[j] = tem;
        i++;
    }
    if (i == n - 1 && (*func)(start[i], start[0])) {
        T tem = start[i];
        start[i] = start[0];
        start[0] = tem;
    }
    if (1 < i)
        QuickSort1_1(start, start + i, func);
    if (i + 1 < n)
        QuickSort1_1(start + i, end, func);
    return start;
}

template<class T, typename Comp>
void QuickSort2(T *start, T *end, Comp func) {
    T *front = start + 1, *back = end - 1, *mark = start;
    while (front < back) {
        for (; front < back; back--) {
            if ((*func)(*back, *mark)) {
                T tmp = *back;
                *back = *mark;
                *mark = tmp;
                mark = back;
                break;
            }
        }
        for (; front < back; front++) {
            if (!(*func)(*front, *mark)) {
                T tmp = *front;
                *front = *mark;
                *mark = tmp;
                mark = front;
                front++;
                break;
            }
        }
    }
    if (front > mark && (*func)(*front, *mark)) {
        T tem = *front;
        *front = *mark;
        *mark = tem;
    }
    if (start + 1 < front)
        QuickSort2(start, front, func);
    if (front + 1 < end)
        QuickSort2(front, end, func);
}

template<class T, typename Comp>
void QuickSortStandard(T *start, T *end, Comp func) {
    if (start + 1 >= end)
        return;
    T *x = end - 1, *i = start;
    for (T *j = start; j < end - 1; j++)
        if ((*func)(*j, *x)) {
            T tem = *j;
            *j = *i;
            *i++ = tem;
        }
    T tem = *x;
    *x = *i;
    *i = tem;
    QuickSortStandard(start, i, func);
    QuickSortStandard(i + 1, end, func);
}

/* try something random, based on standard*/
template<class T, typename Comp>
void QuickSort3(T *start, T *end, Comp func) {
    if (start + 1 >= end)
        return;
    T *x = end - 1, *i = start;
    if (end - start > 10) {
        T **hehe = new T*[3];
        for (int i = 0; i < 3; i++)
            hehe[i] = start + (end - start) / 5 * (i + 1);
        for (int i = 0; i < 2; i++)
            for (int j = i + 1; j < 3; j++)
                if ((*func)(*hehe[i], *hehe[j])) {
                    T *tem = hehe[i];
                    hehe[i] = hehe[j];
                    hehe[j] = tem;
                }
        T t = *hehe[1];
        *hehe[1] = *x;
        *x = t;
        delete[] hehe;
    }
    for (T *j = start; j < end - 1; j++)
        if ((*func)(*j, *x)) {
            T tem = *j;
            *j = *i;
            *i++ = tem;
        }
    T tem = *x;
    *x = *i;
    *i = tem;
    QuickSortStandard1(start, i, func);
    QuickSortStandard1(i + 1, end, func);
}

}

#endif /* QUICKSORT_H_ */
