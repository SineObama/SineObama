/*
 * func.h
 *
 *  Created on: 2015Äê6ÔÂ1ÈÕ
 *      Author: Sine
 */

#ifndef FUNC_H_
#define FUNC_H_

#include <string>
#include "PA.h"
#include "Affair.h"

// used in "while" structure to make an convenient way...
#define DO_UNTIL_BREAK 1

#define WARNING_CHARACTOR '\020'

void print_order();

enum order {
    _add,
    _list,
    _sort,
    _mark,
    _mark_back,
    _delete,
    _delete_done,
    _delete_all,
    _wrong_order,
    _exit_
};

order get_order();

Affair NewAffair();

std::string getline_string();

void warning_message(const std::string & m);

void my_pause();

void message(const std::string & s);

// and also do something with the function pointer
void get_number_and_(PA & pa, void (*p)(PA &, int));

void do_mark(PA & pa, int num);

void do_mark_back(PA & pa, int num);

void do_delete(PA & pa, int num);

#endif /* FUNC_H_ */
