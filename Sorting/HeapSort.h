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

template<class T>
T *HeapSort(T *s, std::size_t len, bool (*func)(const T &, const T &)) {
    for (std::size_t i = len; i > 1; i--) {
        for (std::size_t j = i / 2; ; j--) {
            T *p = NULL;
            if (son1(j) < i && (*func)(s[j], s[son1(j)]))
                p = s + son1(j);
            if (son2(j) < i) {
                if (p) {
                    if ((*func)(*p, s[son2(j)]))
                        p = s + son2(j);
                } else {
                    if ((*func)(s[j], s[son2(j)]))
                        p = s + son2(j);
                }
            }
            if (p) {
                T t(*p);
                *p = s[j];
                s[j] = t;
            }
            if (j == 0)
                break;
        }
        T tem(s[0]);
        s[0] = s[i - 1];
        s[i - 1] = tem;
    }
    return s;
}

#undef son1
#undef son2

}

#endif /* HEAPSORT_H_ */
