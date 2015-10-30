/*
 * DynamicArray.h
 *
 *  Created on: 2015Äê5ÔÂ30ÈÕ
 *      Author: Sine
 */

// this class is not used in PA, temporaryily

// an array that can automatically grow
// the stored type must have default constructor
// or it's an internal type, no initialization is provided
// though somehow strange

#ifndef DYNAMICARRAY_H_
#define DYNAMICARRAY_H_

#include <vector>
#include <ostream>

#define nullptr NULL

template<class type>
class DynamicArray {

 public:

    DynamicArray();
    DynamicArray(const DynamicArray & other);
    ~DynamicArray();

    int getSize() const;

    type & operator[](int);
    type operator[](int) const;

 protected:

    void cpy(type *, const type *, int size);
    //  double the size if the space is not enough
    void grow();

 private:

    type * _data;
    int _size;
    static const int _DEFAULT_SIZE = 16;

};

#endif /* DYNAMICARRAY_H_ */
