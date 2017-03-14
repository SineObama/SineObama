/*
 * Machine.h
 *
 *  Created on: 2017Äê3ÔÂ14ÈÕ
 *      Author: Sine
 */

#ifndef MACHINE_H_
#define MACHINE_H_

class Machine {

 public:

    typedef double data_t;

    Machine(size_t n);
    ~Machine();

    data_t learn(data_t ** const data, const size_t m, data_t alpha = 1,
                 const size_t times = 100, data_t scale = 1.1);
    data_t guess(data_t *value);

 private:

    typedef unsigned int size_t;
    typedef double comp_t;

    size_t n;
    data_t *theta;

};

#endif /* MACHINE_H_ */
