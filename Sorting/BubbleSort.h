/*
 * BubbleSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#ifndef BUBBLESORT_H_
#define BUBBLESORT_H_

namespace Sine {

template<class T>
T *BubbleSort(T *s, std::size_t len, bool (*func)(const T &, const T &)) {
    for (std::size_t i = len; i > 0; i--) {
        for (std::size_t j = 1; j < i; j++) {
            if ((*func)(s[j], s[j - 1])) {
                T tem = s[j];
                s[j] = s[j - 1];
                s[j - 1] = tem;
            }
        }
    }
    return s;
}

}

#endif /* BUBBLESORT_H_ */
