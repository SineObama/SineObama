/*
 * DynamicArray.cpp
 *
 *  Created on: 2015Äê5ÔÂ30ÈÕ
 *      Author: Sine
 */

#include "DynamicArray.h"
#include <cstdio>

template<class type>
DynamicArray<type>::DynamicArray() {
    _size = _DEFAULT_SIZE;
    _data = new type[_size];
}

template<class type>
DynamicArray<type>::DynamicArray(const DynamicArray & other) {
    _size = other._size;
    _data = new type[_size];
    cpy(_data, other._data, _size);
}

template<class type>
DynamicArray<type>::~DynamicArray() {
    delete[] _data;
}

template<class type>
int DynamicArray<type>::getSize() const {
    return _size;
}

template<class type>
type & DynamicArray<type>::operator[](int i) {
    while (_size <= i) {
        grow();
    }
    return _data[i];
}

template<class type>
type DynamicArray<type>::operator[](int i) const {
    return _data[i];
}

template<class type>
void DynamicArray<type>::cpy(type * t, const type * f, int size) {
    int i;
    for (i = 0; i < size; i++) {
        t[i] = f[i];
    }
}

template<class type>
void DynamicArray<type>::grow() {
    DynamicArray tem = (*this);
    delete[] _data;
    _data = new type[tem._size * 2];
    cpy(_data, tem._data, tem._size);
    _size *= 2;
}
