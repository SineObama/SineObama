/*
 * SVM.h
 * @version 0.2
 *  Created on: 2017年4月18日
 *      Author: Sine
 */

#ifndef SVM_H_
#define SVM_H_

#include <cstdlib>
#include <float.h>

#define VERSION "0.2.0"

#define FORMAT "f"
#define DATA_MAX FLT_MAX

class SVM {

 public:

    typedef unsigned int size_t;
    typedef float data_t;

    // 初始化一个模型。输入特征数
    SVM(size_t n);
    // 读取一个模型。
    SVM(const char *filename);

    ~SVM();

    void train(const data_t * const *yx, const size_t l, const data_t C,
               const size_t iterations, const char *modelFile);
    void predict(const data_t * const *yx, const size_t l,
                 const char *outputFile);

 private:

    size_t n;
    data_t *bw;  // b,w

    // scaling
    data_t *min;
    data_t *width;

};

#endif /* SVM_H_ */
