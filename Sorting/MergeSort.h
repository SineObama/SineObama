/*
 * MergeSort.h
 *
 *  Created on: 2015Äê12ÔÂ2ÈÕ
 *      Author: Sine
 */

#ifndef MERGESORT_H_
#define MERGESORT_H_

namespace Sine {

template<class T, typename Comp>
T *MergeSort(T *start, T *end, Comp func) {
    std::size_t len = end - start;
    std::size_t len1 = len / 2, len2 = len - len1;
    T *s1 = new T[len1], *s2 = new T[len2];
    for (std::size_t i = 0; i < len1; i++)
        s1[i] = start[i];
    for (std::size_t i = 0; i < len2; i++)
        s2[i] = start[i + len1];
    if (len1 > 1)
        MergeSort(s1, s1 + len1, func);
    if (len2 > 1)
        MergeSort(s2, s2 + len2, func);
    T *p1 = s1, *p2 = s2;
    for (std::size_t i = 0; i < len; i++) {
        if (p1 - s1 < len1 && p2 - s2 < len2) {
            if ((*func)(*p1, *p2))
                start[i] = *(p1++);
            else
                start[i] = *(p2++);
        } else if (p1 - s1 < len1)
            start[i] = *(p1++);
        else
            start[i] = *(p2++);
    }
    delete[] s1;
    delete[] s2;
    return start;
}

}

#endif /* MERGESORT_H_ */
