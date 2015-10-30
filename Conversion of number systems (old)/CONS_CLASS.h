/*
 * CONS_CLASS.h
 *
 *  Created on: 2015Äê3ÔÂ11ÈÕ
 *      Author: Sine
 */

#ifndef CONVERSION_OF_NUMBER_SYSTEMS__OLD__CONS_CLASS_H_
#define CONVERSION_OF_NUMBER_SYSTEMS__OLD__CONS_CLASS_H_

#include <cstdio>
#include <iostream>
#include <cstring>
#include <cstdlib>
#define DEFAULT_SYSTEM1 0
#define DEFAULT_SYSTEM2 10
using namespace std;

class cons {
 public:
    cons();
    char *calculate(int, char *, int = DEFAULT_SYSTEM1, int = DEFAULT_SYSTEM2);
    char *calculate(long long, char *, int = DEFAULT_SYSTEM1, int =
            DEFAULT_SYSTEM2);
    char *calculate(float, char *, int = DEFAULT_SYSTEM1,
                    int = DEFAULT_SYSTEM2);
    char *calculate(double, char *, int = DEFAULT_SYSTEM1,
                    int = DEFAULT_SYSTEM2);
    char *calculate(const string &, char *, int = DEFAULT_SYSTEM1, int =
            DEFAULT_SYSTEM2);
    char *calculate(const char *, char *, int = DEFAULT_SYSTEM1, int =
            DEFAULT_SYSTEM2);
    void set_input_system(int = DEFAULT_SYSTEM1);
    void set_output_system(int = DEFAULT_SYSTEM1);
//    void print(int = 10, const char * = "default");
    int get_input_system();
    int get_output_system();
 private:
    void fix_input_system(const char *);
    bool check_legality_with_system(const char *) const;
    bool check_with_system(const char *) const;
    int get_start_num(const char *) const;
    bool act_as_number(const char &) const;
    int change_char_to_int(char) const;
    char change_int_to_char(int) const;
    void program_exit(int) const;
    char sign;
    int input_system, output_system;
};

#endif /* CONVERSION_OF_NUMBER_SYSTEMS__OLD__CONS_CLASS_H_ */
