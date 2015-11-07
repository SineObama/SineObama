/*
 * Sqrt.h
 *
 *  Created on: 2015Äê10ÔÂ24ÈÕ
 *      Author: Sine
 */

#ifndef SQRT_H_
#define SQRT_H_

#include <iostream>

namespace Sine {

/*
 * base on basic operators on class T, also constructor with an integer.
 */

template<class T>
T Sqrt2(T _b, int _n, int t = 50) {
    T src(_b);
    while (t--) {
        T tem(_b);
        for (int j = 0; j < _n - 2; j++)
            tem = tem * _b;
        _b = _b - (tem * _b - src) / (T(_n) * tem);
    }
    return _b;
}

template<class T>
T Sqrt1(T _b, int _n, const T & _m) {
    T src(_b);
    int t = 10000;
    while (t--) {
        T tem(_b);
        for (int j = 0; j < _n - 2; j++)
            tem = tem * _b;
        _b = _b - (tem * _b - src) / (T(_n) * tem);
        if ((tem * _b - src > T(0) && tem * _b - src < _m)
                || (src - tem * _b > T(0) && src - tem * _b < _m)) {
            std::cout << t << "\n";
            return _b;
        }
    }
    throw;
    return _b;
}

}

#endif /* SQRT_H_ */
