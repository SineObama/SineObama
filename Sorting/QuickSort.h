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
    T *mark = start, *s = start, *e = end - 1;
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
            return s;
        T tem = *mark;
        *mark = *start;
        *start = tem;
        mark = start;
    }
    while (1) {
        while (s < e && (*func)(*s, *mark))
            s++;
        while (s < e && !(*func)(*e, *mark))
            e--;
        if (s >= e)
            break;
        T tem = *s;
        *s = *e;
        *e = tem;
        s++;
    }
    assert(s == e);
    QuickSort(start, s, func);
    QuickSort(e, end, func);
    return s;
}

}

#endif /* QUICKSORT_H_ */
