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

template<class T>
T *SelectionSort(T *s, std::size_t len, bool (*func)(const T &, const T &)) {
    for (std::size_t i = 0; i < len - 1; i++) {
        T *min = s + i;
        for (std::size_t j = i + 1; j < len; j++) {
            if ((*func)(s[j], *min))
                min = s + j;
        }
        T tem = s[i];
        s[i] = *min;
        *min = tem;
    }
    return s;
}

}

#endif /* SELECTIONSORT_H_ */
