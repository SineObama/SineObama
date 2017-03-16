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
    typedef unsigned int size_t;

    Machine(size_t n, const char *filename = NULL);
    ~Machine();

    void learn(const char *trainFile, const char *thetaFilePrefix,
                 const size_t saveTimes = 100, const size_t times = 100,
                 data_t alpha = 0.1);
    void guess(const char *test, const char *output);

 private:

    typedef float comp_t;

    static void readcsv(const char *filename, data_t **data, int row,
                        int column);

    size_t n;
    data_t *theta;
    data_t *min;
    data_t *gap;

};

#endif /* MACHINE_H_ */
