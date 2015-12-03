/*
 * InsertionSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_


namespace Sine {

template<class T, typename Comp>
T *InsertionSort(T *start, T *end, Comp func) {
    for (T *it = start + 1; it < end; it++) {
        for (T *it2 = it; it2 > start; it2--) {
            if ((*func)(*it2, *(it2 - 1))) {
                T tem = *it2;
                *it2 = *(it2 - 1);
                *(it2 - 1) = tem;
            } else {
                break;
            }
        }
    }
    return start;
}

}

#endif /* INSERTIONSORT_H_ */
