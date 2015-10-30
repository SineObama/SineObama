/*
 * main.cpp
 *
 *  Created on: 2015Äê3ÔÂ11ÈÕ
 *      Author: Sine
 */

#include "CONS_ClASS.h"

int main() {
    cons a;
    while (1) {
        char s[10000] = { 0 }, in_s[1000] = { 0 };
        gets(s);
        if (s[0] == EOF) {
            break;
        }
        int in = 0, out = 0, count = 0;
        while (s[count] != ' ' and s[count] != 0) {
            in_s[count] = s[count];
            count++;
        }
        while (s[count] == ' ' and s[count] != 0) {
            count++;
        }
        while (s[count] != ' ' and s[count] != 0) {
            in *= 10;
            in += s[count] - '0';
            count++;
        }
        while (s[count] == ' ' and s[count] != 0) {
            count++;
        }
        while (s[count] != ' ' and s[count] != 0) {
            out *= 10;
            out += s[count] - '0';
            count++;
        }
        char out_s[1000] = { 0 };
        a.calculate(in_s, out_s, in, out);
        cout << in_s << " in " << a.get_input_system() << " system has beed changed to " << a.get_output_system()
             << " system:" << endl;
        printf("%s\n", out_s);
        fflush(stdout);
    }
    return 0;
}
