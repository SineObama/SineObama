/*
 * SimpleTiming.h
 *
 *  Created on: 2015��12��13��
 *      Author: Sine
 */

#ifndef SIMPLETIMING_H_
#define SIMPLETIMING_H_

namespace Sine {

class SimpleTiming {
 public:
    SimpleTiming();
    void operator()();
    int get();
 private:
    double oldTime;
    double newTime;
};

}

#endif /* SIMPLETIMING_H_ */
