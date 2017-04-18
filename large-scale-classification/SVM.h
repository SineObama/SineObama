/*
 * SVM.h
 *
 *  Created on: 2017Äê4ÔÂ18ÈÕ
 *      Author: Sine
 */

#ifndef SVM_H_
#define SVM_H_

#include <cstdlib>

class SVM {

 public:

    typedef float data_t;

    static void train(const data_t * const *yx, const int l, const int n, data_t *bw);
    static void predict(data_t **yx, const int l, const int n, data_t *bw);

 private:

    SVM();

};

#endif /* SVM_H_ */
