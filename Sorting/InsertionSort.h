/*
 * InsertionSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#ifndef INSERTIONSORT_H_
#define INSERTIONSORT_H_


namespace Sine {

template<class T>
T *InsertionSort(T *s, std::size_t len, bool (*func)(const T &, const T &)) {
    for (std::size_t i = 1; i < len; i++) {
        for (std::size_t j = i; j > 0; j--) {
            if ((*func)(s[j], s[j - 1])) {
                T tem = s[j];
                s[j] = s[j - 1];
                s[j - 1] = tem;
            } else {
                break;
            }
        }
    }
    return s;
}

}

#endif /* INSERTIONSORT_H_ */
