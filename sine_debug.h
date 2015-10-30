/*
 * sine_debug.h
 *
 *  Created on: 2015��4��11��
 *      Author: Sine
 */

//#define NDEBUG

#ifndef SINE_DEBUG_H_
#define SINE_DEBUG_H_

#ifndef NDEBUG
static int debug_step = 0;
// for not warning me about unused varible debug_step when not using ds
void i_am_stupid(int very_much = debug_step);
#endif

// the 'd' series prints the line number and message or value or counting number
// print value, print, steps, check, line function
#ifndef NDEBUG
#include <iostream>
#include <cstdio>
#define DO if (1)  // for indentation(����)
#define dpv(s) lf_;std::cerr << #s" : " << (s) << std::endl
#define dp(s) lf_; std::cerr << (s)
#define ds lf_; std::cerr << (debug_step++) << std::endl;
#define dc(expression, s) if(expression) {dp(s); }
#define lf_ std::cerr << "at " << __LINE__ << " " << __FUNCTION__ << ": "
#else
#define DO
#define dpv(not_use)
#define dp(not_use)
#define ds
#define dc(not_use, not_use1)
#define lf_
#endif

#endif
