/*
 * main.cpp
 *
 *  Created on: 2017Äê4ÔÂ23ÈÕ
 *      Author: Sine
 */

#include "PID.h"

class hehe {

 public:

    typedef double data_t;

    hehe(data_t kp, data_t ki, data_t kd, data_t ts)
            : kp(kp),
              ki(ki),
              kd(kd),
              ts(ts) {

    }

    data_t PIDControl(data_t err) {
        static data_t I = 0;
        static data_t err0 = 0;
        data_t u = kd * (err - err0) + ki * I + kp * err;
        I += err * ts;
        err0 = err;
        return u;
    }

 private:

    const data_t kp;
    const data_t ki;
    const data_t kd;
    const data_t ts;

};

int main() {
    hehe(1, 1, 1, 1);
}
