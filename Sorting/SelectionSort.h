/*
 * SelectionSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#ifndef SELECTIONSORT_H_
#define SELECTIONSORT_H_

#include <cstdlib>

namespace Sine {

template<class T, typename Comp>
T *SelectionSort(T *start, T *end, Comp func) {
    for (T *it = start; it < end - 1; it++) {
        T *fore = it;
        for (T *it2 = it + 1; it2 < end; it2++)
            if ((*func)(*it2, *fore))
                fore = it2;
        T tem = *it;
        *it = *fore;
        *fore = tem;
    }
    return start;
}

}

#endif /* SELECTIONSORT_H_ */
