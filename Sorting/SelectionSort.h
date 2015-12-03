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
    std::size_t len = end - start;
    for (std::size_t i = 0; i < len - 1; i++) {
        T *min = start + i;
        for (std::size_t j = i + 1; j < len; j++) {
            if ((*func)(start[j], *min))
                min = start + j;
        }
        T tem = start[i];
        start[i] = *min;
        *min = tem;
    }
    return start;
}

}

#endif /* SELECTIONSORT_H_ */
