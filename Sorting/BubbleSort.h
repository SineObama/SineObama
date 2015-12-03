/*
 * BubbleSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#ifndef BUBBLESORT_H_
#define BUBBLESORT_H_

namespace Sine {

template<class T, typename Comp>
T *BubbleSort(T *start, T *end, Comp func) {
    for (T *it = end; it > start; it--)
        for (T *it2 = start + 1; it2 < it; it2++)
            if ((*func)(*it2, *it2)) {
                T tem = *it2;
                *it2 = *(it2 - 1);
                *(it2 - 1) = tem;
            }
    return start;
}

}

#endif /* BUBBLESORT_H_ */
