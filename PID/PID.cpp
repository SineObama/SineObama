/*
 * PID.cpp
 *
 *  Created on: 2017年4月23日
 *      Author: Sine
 */

#include "PID.h"

data_t PIDControl(data_t err) {
    static const data_t ts = (data_t) 1 / 40;  //采样时间
    static const data_t kd = 0.9;
    static const data_t ki = 1e-4;
    static const data_t kp = 0.8;
    static data_t I = 0;
    static data_t err0 = 0;
    data_t u = kd * (err - err0) + ki * I + kp * err;
    I += err * ts;
    err0 = err;
    return u;
}
