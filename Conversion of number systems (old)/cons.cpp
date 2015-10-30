/*
 * cons.cpp
 *
 *  Created on: 2015年3月11日
 *      Author: Sine
 */

#include "CONS_ClASS.h"

cons::cons() {
    sign = '+';
    input_system = DEFAULT_SYSTEM1;
    output_system = DEFAULT_SYSTEM1;
    return;
}

char *cons::calculate(int x, char *out, int in_system, int out_system) {
    long long tem_lld = x;
    return calculate(tem_lld, out, in_system, out_system);
}

char *cons::calculate(register long long x, char *out, int in_system,
                      int out_system) {
    int num = 0, i = 0;
    long long tem_lld = x;
    while (tem_lld) {
        tem_lld /= 10;
        num++;
    }
    char tem_s[20] = { 0 };
    for (i = num - 1; i >= 0; i--) {
        tem_s[i] = '0' + x % 10;
        x /= 10;
    }
    return calculate(tem_s, out, in_system, out_system);
}

char *cons::calculate(float x, char *out, int in_system, int out_system) {
    double tem_lf = x;
    return calculate(tem_lf, out, in_system, out_system);
}

char *cons::calculate(double x, char *out, int in_system, int out_system) {
    // todo
    // double精度好像有16位小数，整数2……
    return calculate("0", out, in_system, out_system);
}

char *cons::calculate(const string &x, char *out, int in_system,
                      int out_system) {
    return calculate(x.c_str(), out, in_system, out_system);
}

char *cons::calculate(const char *in, char *out, int in_system,
                      int out_system) {
    set_input_system(in_system);
    fix_input_system(in);
    set_output_system(out_system);
    if (check_legality_with_system(in) == false) {  // shows illegal
        cout << "Here is (some of) the input string:" << endl;
        register int i = 0;
        for (i = 0; i < (1 << 7) and in[i]; i++) {
            cout << in[i];
        }
        cout << endl;
        program_exit(1);
    }
    // predict maxlen and malloc
    int input_len = strlen(in), c_len = 1, tem = output_system;
    for (c_len = 1; c_len < 6; c_len++) {
        if (tem < input_system) {
            tem *= output_system;
        } else {
            break;
        }
    }
    int maxlen = c_len * input_len + 1, *s = new int[maxlen];
    memset(s, 0, maxlen * sizeof(int));
    int cur = 0;
    if (in[cur] == '-' || in[cur] == '+') {
        cur++;
    }
    // calculation begins
    while (in[cur]) {
        int add = change_char_to_int(in[cur]), count = 0;
        for (count = maxlen - 1; count >= 0; count--) {
            s[count] *= input_system;
        }
        s[0] += add;
        for (count = 0; count < maxlen; count++) {
            if (s[count] >= output_system) {
                s[count + 1] += s[count] / output_system;
                s[count] %= output_system;
            }
        }
        cur++;
    }
    // calculation ends
    cur = maxlen - 1;
    while (s[cur] == 0 and cur > 0) {
        cur--;
    }
    int output_len = cur + 1;
    for (cur = 0; cur < output_len; cur++) {
        out[cur] = change_int_to_char(s[output_len - cur - 1]);
    }
    out[cur] = 0;
    return out;
}

void cons::set_input_system(int in_system) {
    if (in_system == DEFAULT_SYSTEM1) {
        // do nothing
    } else if (in_system > 36) {
        cout << "The input system is too large." << endl;
        program_exit(1);
    } else if (in_system < 2) {
        cout << "The input system is too small." << endl;
        program_exit(1);
    } else {
        printf("%d\n", input_system);
        input_system = in_system;
    }
    return;
}

void cons::set_output_system(int out_system) {
    if (out_system == DEFAULT_SYSTEM1) {
        output_system = DEFAULT_SYSTEM2;
    } else if (out_system > 36) {
        cout << "The output system is too large." << endl;
        program_exit(1);
    } else if (out_system < 2) {
        cout << "The output system is too small." << endl;
        program_exit(1);
    } else {
        output_system = out_system;
    }
    return;
}

// with extra order "unsigned" and "signed". warn when order is invalid
//void cons::print(int order_system, const char *order) {
//    if (strcmp(order, "default") == 0) {
//        if (sign == '-') {
//            putchar(sign);
//        }
//    } else if (strcmp(order, "unsigned") == 0) {
//        // do nothing
//    } else if (strcmp(order, "signed") == 0) {
//        putchar(sign);
//    } else {
//        cout << "Print order is invalid." << endl;
//        cout << "Programme stop." << endl;
//        exit(1);
//    }
//    printf("%s\n", calculate());
//    return;
//}

int cons::get_input_system() {
    return input_system;
}

int cons::get_output_system() {
    return output_system;
}

// correct the input system. stop and warn when it's illegal
void cons::fix_input_system(const char *s) {
    int start_num = get_start_num(s);
    if (input_system == DEFAULT_SYSTEM1) {  // system need to be recognize
        if (s[start_num] == '0') {
            if (s[start_num + 1] == 'x' or s[start_num + 1] == 'X') {
                input_system = 16;  // 0x or 0X shows it's 16 system
            } else {
                input_system = 8;  // start with 0 shows it's 8 system
            }
        } else {
            cout << "Can't recognize string's system, without a valid parameter."
                 << endl;
            program_exit(1);
        }
    }
    return;
}

// check whether the string contains illegal charactors. warn when it does
bool cons::check_legality_with_system(const char *s) const {
    if (strlen(s) > 1 << 20) {
        cout << "The input string is somehow too long." << endl;
        return false;
    }
    int cur = 0, count_point = 0;
    bool result = true;
    while (s[cur]) {
        if (act_as_number(s[cur])) {
            // do nothing
        } else if (cur == 0 and (s[cur] == '-' or s[cur] == '+')) {
            // the first charactor is a sign. do nothing
        } else if (s[cur] == '.') {
            if (count_point == 1) {
                cout << "The input string has more than one point. " << endl;
                result = false;
            }
            count_point++;
        } else {
            cout << "The input string has illegal charactor '" << s[cur]
                 << "' , the " << cur << "th charactor of it"
                 << " (ASCII number " << static_cast<int>(s[cur]) << "). "
                 << endl;
            result = false;
        }
        cur++;
    }
    return check_with_system(s) and result;
}

// next, check whether there is a charactor that out of the range of system
bool cons::check_with_system(const char *s) const {
    bool go_on = true;
    int cur = 0;
    if (input_system <= 10) {
        char max_int = input_system + '0' - 1;
        while (s[cur]) {
            if (s[cur] > max_int) {
                cout << "The charactor '" << s[cur] << "' at the " << cur + 1
                     << "th charactor of the string, is out of the range of "
                     << input_system << " system." << endl;
                go_on = false;
            }
            cur++;
        }
    } else {
        char max_up_char = input_system + 'A' - 10 - 1, max_low_char =
                max_up_char - 'A' + 'a';
        while (s[cur]) {
            if ((s[cur] <= 'Z' and s[cur] > max_up_char)
                    or (s[cur] >= 'a' and s[cur] > max_low_char)) {
                cout << "The charactor '" << s[cur] << "' at the " << cur + 1
                     << "th charactor of the string, is out of the range of "
                     << input_system << " system." << endl;
                go_on = false;
            }
            cur++;
        }
    }
    return go_on;
}

// judge whether the first charactor is a sign that should be skipped
int cons::get_start_num(const char *s) const {
    if (s[0] == '-') {
        return 1;
    } else if (s[0] == '+') {
        return 1;
    } else {
        return 0;
    }
}

// judge whether charactor is number or upper or lower charactor
bool cons::act_as_number(const char &ch) const {
    if ('0' <= ch and ch <= '9') {
        return true;
    } else if ('A' <= ch and ch <= 'Z') {
        return true;
    } else if ('a' <= ch and ch <= 'z') {
        return true;
    }
    return false;
}

int cons::change_char_to_int(char ch) const {
    if (ch <= '9') {
        return ch - '0';
    } else if (ch >= 'A' and ch <= 'Z') {
        return ch - 'A' + 10;
    } else if (ch >= 'a' and ch <= 'z') {
        return ch - 'a' + 10;
    } else {
        cout << "A error occured at function change_char_to_int." << endl;
        program_exit(1);
        return 0;
    }
}

char cons::change_int_to_char(int i) const {
    if (i < 10 and i >= 0) {
        return i + '0';
    } else if (i >= 10 and i <= 35) {
        return i + 'A' - 10;
    } else {
        cout << "A error occured at function change_int_to_char." << endl;
        program_exit(1);
        return 0;
    }
}

void cons::program_exit(int x) const {
    cout << "Program stop." << endl;
    exit(x);
}
