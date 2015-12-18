/*
 * SineDebug.h
 *
 *  Created on: 2015Äê12ÔÂ17ÈÕ
 *      Author: Sine
 */

#include <iostream>
#include <cstdio>

#ifndef SINEDEBUG_H_
#define SINEDEBUG_H_

//#define NDEBUG
#define NSHOWFILE

#ifndef NDEBUG
static int debug_step = 0;
// for not warning me about unused varible debug_step when not using ds
void i_am_stupid(int very_much = debug_step);
#endif

// the 'd' series prints the line number and message or value or counting number
// print value, print, steps, new line, check
#ifndef NDEBUG
#define dpv(s) {lf_;std::cerr << #s":" << (s) << ' ';}
#define dp(s) {lf_; std::cerr << (s) << ' ';}
#define ds {lf_; std::cerr << (debug_step++);}
#define dn std::cerr << std::endl
#define dc(expression, s) {if(!(expression)) {dp(#expression);dp("fail");dp(s);}}
#else
#define dpv(not_use)
#define dp(not_use)
#define ds
#define dn
#define dc(not_use, not_use1)
#endif

#ifndef NSHOWFILE
#define lf_ std::cerr << "at " << __LINE__ << " " << __FUNCTION__ << ": "
#else
#define lf_
#endif

#endif /* SINEDEBUG_H_ */
