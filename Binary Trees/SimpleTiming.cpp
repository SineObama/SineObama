/*
 * SimpleTiming.cpp
 *
 *  Created on: 2015Äê12ÔÂ13ÈÕ
 *      Author: Sine
 */

#include "SimpleTiming.h"
#include <ctime>

namespace Sine {

SimpleTiming::SimpleTiming() {
    newTime = oldTime = 0;
}

void SimpleTiming::operator()() {
    oldTime = newTime;
    newTime = clock();
}

int SimpleTiming::get() {
    if (!oldTime)
        return 0;
    return int(newTime - oldTime);
}

}
