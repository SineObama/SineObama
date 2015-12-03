/*
 * HeapSort.h
 *
 *  Created on: 2015Äê11ÔÂ28ÈÕ
 *      Author: Sine
 */

#ifndef HEAPSORT_H_
#define HEAPSORT_H_

#include <cstdlib>
#include <iostream>

namespace Sine {

#define son1(i) (i * 2 + 1)
#define son2(i) (i * 2 + 2)

template<class T, typename Comp>
T *HeapSort(T *start, T *end, Comp func) {
    std::size_t len = end - start;
    for (std::size_t i = len; i > 1; i--) {
        for (std::size_t j = i / 2; ; j--) {
            T *p = NULL;
            if (son1(j) < i && (*func)(start[j], start[son1(j)]))
                p = start + son1(j);
            if (son2(j) < i) {
                if (p) {
                    if ((*func)(*p, start[son2(j)]))
                        p = start + son2(j);
                } else {
                    if ((*func)(start[j], start[son2(j)]))
                        p = start + son2(j);
                }
            }
            if (p) {
                T t(*p);
                *p = start[j];
                start[j] = t;
            }
            if (j == 0)
                break;
        }
        T tem(start[0]);
        start[0] = start[i - 1];
        start[i - 1] = tem;
    }
    return start;
}

#undef son1
#undef son2

}

#endif /* HEAPSORT_H_ */
