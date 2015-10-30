/*
 * hugenum.cpp
 *
 *  Created on: 2015年3月24日
 *      Author: Sine
 */

#include "hugenum.h"
#include "D:\\others\\workspace\\sine_debug.h"
#include <stdexcept>
#include <vector>

// this may cause sth.?
using std::ios;
using std::vector;
using std::size_t;
using std::cout;
using std::istream;
using std::ostream;
using std::stringstream;
using std::streambuf;
using std::string;
using std::invalid_argument;

#define hn hugenum
#define DEFAULT_PRECISION 6

static const char *hn::my_PI =
        "3.141592653589793238462643383279502884197169399375105820974944592";
static const char *hn::my_E =
        "2.718281828459045235360287471352662497757247093699959574966967627";

size_t hn::comp_precision = 16;
size_t hn::calc_precision = 128;
size_t hn::radical_time = 50;
bool hn::_fix = true;

// 2015年9月18日11:31:50checked
hn operator+(const hn & a) {
    return a;
}

// 2015年9月18日11:32:11checked
// only change the sign, omit 0 with sign = zero
hn operator-(const hn & a) {
    hn result = a;
    result.sign *= -1;
    return result;
}  // end function operator-

// 2015年9月18日11:32:31checked
// data after comp_precision will be omitted using rounding (四舍五入)
// todo what did I write above???
hn abs(const hn & a) {
    hn result = a;
    result.sign *= result.sign;
    return result;
}  // end function abs

// 2015年9月18日11:35:04checked
hn operator+(const hn & a, const hn & b) throw (invalid_argument) {
    return hn::plus(a, 1, b, 1);
}  // end function operator+

// 2015年9月18日11:35:11checked
hn operator-(const hn & a, const hn & b) throw (invalid_argument) {
    return hn::plus(a, 1, b, -1);
}  // end function operator-

// 2015年9月18日22:16:05checked
hn operator*(const hn & a, const hn & b) throw (invalid_argument) {
    hn product(0);
    product.sign = a.sign * b.sign;
    if (product.sign == 0)
        return product;

    product.power = a.power + b.power;
    product.num_len = a.num_len + b.num_len - 1;
    product.reserve(a.num_len + b.num_len);
    product.clear();

    for (size_t i = 0; i < a.num_len; i++)
        for (size_t j = 0; j < b.num_len; j++)
            product.num[i + j] += a.num[i] * b.num[j];

    product.fix();
    return product;
}  // end function operator*

// fixme be careful about comparsion between hugenum
hn operator/(hn minuend, hn subtrahend) throw (invalid_argument) {
    if (subtrahend.sign == 0)
        throw invalid_argument("The divider is zero.");
    if (minuend.sign == 0)
        return minuend;

//    hn minuend(a);  // 被除数，也是用“减法”法剩余的数
    minuend.sign = 1;
    hn quotient(0);  // 商
    quotient.sign = minuend.sign * subtrahend.sign;
    quotient.power = minuend.power - subtrahend.power;

    subtrahend.sign = 1;
    subtrahend.power = minuend.power;
    if (hn::basic_compare(minuend, subtrahend, hn::calc_precision) < 0) {
        quotient.power--;
        subtrahend.power--;
    }
    for (size_t i = 0; i < hn::calc_precision && minuend.sign >= 0;
            i++, subtrahend.power--) {
        if (quotient.capacity <= i)
            quotient.reserve(i * 2);
        for (size_t j = 9; j >= 1; j--)  // todo improve algorithm
            if (hn::basic_compare(minuend, subtrahend * hn(j),
            hn::calc_precision) >= 0) {
                minuend -= subtrahend * hn(j);
                quotient.num[i] = j;
                break;
            }
    }

    quotient.fix();  // fixme need ?
    return quotient;
}  // end function operator/

// fixme negative number ?
hn radical(hn sause, size_t root, size_t time) throw (invalid_argument) {
    if (sause.sign < 0 && root % 2 == 0)
        throw invalid_argument("radical: 负数被开偶数次方");
    if (root == 1)
        return sause;

    if (sause.sign == 0)
        if (root == 0)
            throw invalid_argument("radical: 对0开0次根号");
        else
            return sause;
    else if (root == 0)
        return hn(1);

    int resultsign = sause.sign;
    sause.sign = 1;
    if (time == DEFAULT)
        time = hn::radical_time;
    // sause > result
    hn result(1);
    while (time--) {
        hn tem = (sause - result) / root + result, product = tem;
        for (size_t i = 1; i <= root - 1; i++)
            product *= tem;
        if (compare(product, sause, hn::calc_precision) > 0)
            sause = tem;
        else
            result = tem;
    }
    result.sign = resultsign;
    return result;
}  // end function radical

//int compare(const hn & a, const hn & b, int comp_precision)
//                    throw (invalid_argument) {
//    if (a._system != b._system)
//        throw invalid_argument("compare: system is different.");
//    // 2015-4-7 21:43:11 fixed: comp_precision may bigger than num_len
//    // fixme this implement not efficient?
//
//    // first ensure a, b are not negetive
//    if (a.sign < 0 && b.sign < 0)
//        return compare(-b, -a);
//    else if (a.sign < 0 && b.sign >= 0)
//        return -1;
//    else if (a.sign >= 0 && b.sign < 0)
//        return 1;
//
//    /* check 0 because with power = 0
//     * it will go wrong when being compare with
//     * e.g. 0.01 whose power is negative */
//    if (a.sign == 0)
//        if (b.sign == 0)
//            return 0;  // both 0
//        else
//            return -1;  // b positive
//    else if (b.sign == 0)
//        return 1;  // a positive
//
//    /* here a, b are positive, never 0 any more
//     * compare the power directly */
//    if (a.power < b.power)
//        return -1;
//    else if (a.power > b.power)
//        return 1;
//
//    // compare number with the same power begin
//    int max_len = comp_precision + 1;
//    // check if max_len is bigger than num_len, the "alloced len"
//    if (max_len >= a.num_len)
//        max_len = a.num_len;
//    if (max_len >= b.num_len)
//        max_len = b.num_len;
//
//    for (int i = 1; i < max_len; i++)
//        if (a.num[i] > b.num[i])
//            return 1;
//        else if (a.num[i] < b.num[i])
//            return -1;
//    // e.g. when comp_precision >= 3, compare between 123.4 and 123 stop at '3'.
//
//    // here's no result. the next position need to be checked
//    int n[2] = { 0 }, next = max_len + 1;
//    if (next < a.num_len)
//        n[0] = a.num[next];
//    if (next < b.num_len)
//        n[1] = b.num[next];
//    if (max_len == comp_precision + 1) {
//        // the precision check finish, the rounding implement is needed
//        if (n[0] < 5 && n[1] >= 5)
//            return -1;
//        else if (n[0] >= 5 && n[1] < 5)
//            return 1;
//    } else {
//        if (n[0] < n[1])
//            return -1;
//        else if (n[0] > n[1])
//            return 1;
//    }
//    return 0;
//}  // end function compare

// here's some stupid definition of functions
#define operator_comparsion(t) bool operator t\
(const hugenum & a, const hugenum & b) {return hn::basic_compare(a, b) t 0; }
operator_comparsion(<)
operator_comparsion(>)
operator_comparsion(<=)
operator_comparsion(>=)
operator_comparsion(==)
operator_comparsion(!=)
#undef operator_comparsion

#define constructor(parameter, extra_expression) \
hn::hn parameter {initialization(); extra_expression; }
//constructor((int system),)
hn ::hn(const hn & a) {
    *this = a;
}
//hugenum::hugenum (double n, int system) {initialization(system); set(n); }
constructor((double n), set(n));
//hugenum::hugenum (const std::string &n, int system) {initialization(system); set(n); }
constructor((const std::string &n), set(n));
//hugenum::hugenum (char n, int system) {initialization(system); set(n); }
constructor((char n), set(n));
constructor((short n), set(n));
constructor((int n), set(n));
//hugenum::hugenum (long long n, int system) {initialization(system); set(n); }
constructor((long long n, size_t system), set(n));
//hugenum::hugenum (const int n[], int len, int system) {initialization(system); set(n, len); }
constructor((const int n[], int len, size_t system), set(n, len))
//hugenum::hugenum (const vector<int> & n, int system) {initialization(system); set(n); }
constructor((const vector<int> & n, size_t system), set(n));
#undef constructor

// use double to set. extra fix for continual 0 or 9. I think this is not necessary
void hn::set(double n) {

    const int precision = 20, string_len = precision + 20;  // power too large?
    char s_double[string_len] = { };
    sprintf(s_double, "%.*le", precision - 1, n);
    dc(s_double[precision + 1] == 'e', "double number's string illegal\n");

    if (_fix == true) {  // special fix
        const size_t max_continual_to_fix = comp_precision;
        int count[2] = { 0 };
        // checking start after point
        for (int i = 2; i <= precision; i++) {
            if (s_double[i] == '0') {
                count[0]++;
                count[1] = 0;
                if (count[0] < max_continual_to_fix)
                    continue;
            } else if (s_double[i] == '9') {
                count[0] = 0;
                count[1]++;
                if (count[1] < max_continual_to_fix)
                    continue;
                else
                    s_double[i - count[1]]++;
            } else {
                count[0] = 0;
                count[1] = 0;
                continue;
            }
            // only suitable cases reach here

            // copy e.g. "e+308" to the end location at rounding
            // e.g. 1.000199999987e+308\0 and changed to 1.0002e+308\0
            for (int j = i - max_continual_to_fix + 1, k = precision + 1;
                    s_double[k - 1] && k < string_len; j++, k++)
                s_double[j] = s_double[k];
            break;
        }
    }
    set(s_double);  // pay attention: here _system will be changed
}  // end function set(double)

// use c_string to set.
// this is basic. almost every set uses this
// 2015-4-11 00:42:48 fixed: length checking after reading in "while"
void hn::set(const string & s) {
    const int len = s.size();  // may this improve performance?

    // here check the length. sign and point and e+308 are all counted...
//    dc(len >= calc_precision, "string is too long for calculation precision\n");
// todo here length problem should not throw ?
    // check legality
    if (!string_legal(s)) {  // of course this problem can be left to users
        dp("string illegal (at set)\n");
        // set unsuccessfully
        return;
    }

    int start_location = 0;
    DO {  // check sign
        if (s[0] == '-') {
            sign = -1;
            start_location++;
        } else if (s[0] == '+') {  // todo think about 0
            sign = 1;
            start_location++;
        } else
            sign = 1;
    }
    DO {  // check power
        int power_1 = 0;  // this for the point in number
        int power_2 = 0;  // this for e.g. e+308 in floating point
        int i = start_location;
        while (i < len && s[i] != 'e' && s[i] != 'E' && s[i] != '.')
            i++;
        power_1 = i - start_location - 1;

        // then get power_2
        while (i < len && s[i] != 'e' && s[i] != 'E')
            i++;
        if (i < len && (s[i] == 'e' || s[i] == 'E')) {
            i++;
            if (i >= len)
                return;
            int power_sign = 0;
            if (s[i] == '-') {
                power_sign = -1;
                i++;
            } else if (s[i] == '+') {
                power_sign = 1;
                i++;
            }
            if (i >= len)
                return;
            while (i < len && s[i]) {
                power_2 *= 10;
                power_2 += s[i] - '0';
                i++;
            }
            power_2 *= power_sign;
        }
        power = power_1 + power_2;
    }

    // copy data from char* to int []
    int count = 1, tem_len = len + 1;
    int *tem = new int[tem_len];
    for (int i = start_location; i < len; i++)
        if (s[i] == '.') {  // here allow points
            continue;
        } else if (s[i] >= '0' && s[i] <= '9') {
            tem[count++] = s[i] - '0';
        } else
            // other charactor reaches e.g. 'e'
            break;

    fix_with(tem, tem_len);
}  // end function set(const char *)

// stupidly accept '0' to '9' for fun
void hn::set(char n) {
    if (n >= '0' && n <= '9') {
        num[1] = n - '0';
        if (num[1] != 0)
            sign = 1;
    } else {
        throw std::invalid_argument("hugenum::set(char)");
    }
}  // end function set(char)

void hn::set(short n) {
    long long t = n;
    hugenum::set(t);
}  // end function set(short)

void hn::set(int n) {
    long long t = n;
    hugenum::set(t);
}  // end function set(int)

void hn::set(long long n) {
    char t[30] = { 0 };
    sprintf(t, "%I64d", n);
    hugenum::set(t);
}  // end function set(long long)

// default positive, may not accept all data if reaches
// 2015-4-7 13:16:26 creation finish
void hn::set(const int n[], int len) {
    vector<int> tem(n, len);
    set(tem);
}  // end function set(int[], len)

void hn::set(const vector<int> & n) {
    int end = n.size() > calc_precision ? calc_precision : n.size();
    sign = 1;
    power = n.size() - 1;
    for (int i = 0; i < end; i++)
        num[i] = n[i];
}  // end function set(vector<int>)

//#define set_precision(name, min, max) int hn::set_##name##_precision(int new_)\
//{if (new_ > max || new_ < min) return -1; else {int prev = name##_precision;\
//name##_precision = new_; return prev; }}
//set_precision(comp, 1, calc_precision)
//set_precision(calc, min_unit, max_unit)
//#undef set_precision

size_t hugenum::set_comp_precision() {
    return comp_precision;
}  // end function set_comp_precision()

size_t hugenum::set_comp_precision(size_t n) throw (invalid_argument) {
    if (n < 1)
        throw invalid_argument("comp_precision smaller than 1");
    else if (n > calc_precision)
        throw invalid_argument("comp_precision bigger than calc_precision");
    size_t prev = comp_precision;
    comp_precision = n;
    return prev;
}  // end function set_comp_precision(size_t)

size_t hugenum::set_calc_precision() {
    return calc_precision;
}  // end function set_calc_precision()

size_t hugenum::set_calc_precision(size_t n) throw (invalid_argument) {
    if (n < 1)
        throw invalid_argument("calc_precision smaller than the min");
    else if (n > calc_precision)
        throw invalid_argument("calc_precision bigger than the max");
    if (n > max_unit || n < min_unit)
        return 0;
    size_t prev = calc_precision;
    calc_precision = n;
    return prev;
}  // end function set_calc_precision(size_t)

bool hn::fix_double() {
    return _fix;
}  // end function fix_double()

bool hn::fix_double(bool n) {
    bool prev = _fix;
    _fix = n;
    return prev;
}  // end function fix_double(bool)

// todo ensure \0 the end
// try to implement just like how printf() does
int snprint(char *s, int max, const char *order, const hn & a) {
    enum {
        left_ = 1 << 0,
        showpos_ = 1 << 1,
        add_space = 1 << 2,
        showbase_ = 1 << 3,
        add_zero = 1 << 4
    };
    int flag = 0;  // to save the setting above

    DO {  // %[flags][width][.perc][F|N|h|l]type
        /*
         *  example:    "%-6.7lf"
         *  "  %   -       6           .7       lf  "
         *         ↑       ↑           ↑        ↑
         *     start  width_start  prec_start  end todo type omitted ?
         */
        int start = 0, end = 0;  // of setting before English charactor
        if (order[0] == '%')  // todo the % can be omitted ?
            start++;
        for (int i = start; i < strlen(order); i++)
            if ((order[i] >= 'a' && order[i] <= 'z')
                    || (order[i] >= 'A' && order[i] <= 'Z')) {
                end = i;
                break;
            }
        // todo if no English charactor ? no type, anything to do ?
        // todo hex oct cannot have sign space prec

        int prec_start = 0;
        DO {  // get the start loction of precision
            int i = start - 1;
            while (++i < end && order[i] != '.') {
            }
            prec_start = i;
        }

        int width_start = 0;
        DO {  // get the start loction of width
            int i = start - 1;
            while (++i < prec_start && (order[i] <= '0' || order[i] > '9')) {
            }
            width_start = i;
        }

        // set flag
        for (int i = start; i < width_start; i++)
            switch (order[i]) {  // ommit repeated flag
                case '-':
                    flag |= left_;
                    break;
                case '+':
                    flag |= showpos_;
                    break;
                case ' ':
                    flag |= add_space;
                    break;
                case '#':
                    flag |= showbase_;
                    break;
                case '0':
                    flag |= add_zero;
                    break;
                default:
                    ;  // todo invalid charactor
            }
        if ((flag & add_space) && (flag & showpos_))  // omit add_space
            flag &= ~add_space;
        if ((flag & add_zero) && (flag & left_))  // omit add_zero
            flag &= ~add_zero;

        // set width
        int width = 0;
        for (int i = width_start; i < prec_start; i++) {
            width *= 10;
            width += order[i] - '0';  // todo other invalid charactors
        }
        hn::width = width;
        if (width == 0)  // todo is this ok? omit left_hand?
            flag &= ~left_;

        // set precision
        int precision = 0;  // means number of numbers after the point
        for (int i = prec_start + 1; i < end; i++) {  // the first char is '.'
            precision *= 10;
            precision += order[i] - '0';  // todo other invalid charactors
        }
        // if no number after '.', the default is 0, in function printf()
        hn::precision = precision;
    }

    hn::flags = 0;
    if (flag & left_)
        hn::flags |= ios::left;
    if (flag & showpos_)
        hn::flags |= ios::showpos;
    if (flag & add_space)
        hn::flags |= hn::ios_add_space;
    if (flag & showbase_)
        hn::flags |= ios::showbase;
    if (flag & add_zero) {
        hn::flags |= ios::internal;
        hn::fill = '0';
    }
//    hn::width = width;
//    hn::precision = precision;
    // type is ommitted

    // use overloaded << to print in string through stringstream
    stringstream tem;
    streambuf *backup = cout.rdbuf(tem.rdbuf());
    hn::resetting = false;
    cout << a;
    hn::resetting = true;
    tem.width(max);  // todo no problem?
    tem >> s;  // todo auto add \0 ?
    cout.rdbuf(backup);
    return strlen(s);
}  // end function snprint

int print(const char *order, const hugenum & a) {
    char s[10000] = { };  // todo enough?
    snprint(s, 10000, order, a);
    printf("%s\n", s);
    return strlen(s);
}  // end function print

istream &operator>>(istream & in, hugenum & a) {
    char s[10000] = { };  // todo enough?
    in.width(10000);
    in >> s;
    a = s;
    return in;
}  // end function operator>>

// todo think about print 2.e+308  .2 ?
ostream &operator<<(ostream & out, const hugenum & a) {
    if (a.resetting) {
        hn::flags = out.flags();
        hn::width = out.width();
        hn::precision = out.precision();
    }
    char *body = nullptr;
    int body_len = 0;
    DO {  // produce the main body of output, no sign, but e+308
        if (a.num_len <= a.power + 2) {  // serve as a integer
        // todo change base
            body_len = a.power + 2;
            new_memset(body, body_len);
            for (int i = 1; i < a.num_len; i++)
                body[i - 1] = a.num[i] + '0';
            for (int i = a.num_len; i < a.power + 2; i++)
                body[i - 1] = '0';
        } else {  // serve as a floating point
            enum {
                auto_,
                fixed_,
                scientific_
            } floatfield_ = auto_;
            /* In ostream, fixed and scientific do showpoint,
             * but won't change the showpoint option in flag.
             * Here the control is passed to showpoint_,
             * and they just mean to use scientific or not. */
            bool showpoint_ = true;  // default true for fixed and scientific

            if ((hn::flags & ios::fixed) && !(hn::flags & ios::scientific))
                floatfield_ = fixed_;
            if (!(hn::flags & ios::fixed) && (hn::flags & ios::scientific))
                floatfield_ = scientific_;
            // todo need to change the method?

            int power_len = 0;  // for scientific, and potential scientific
            DO {  // set power len, the len of number. e+308 -> len = 3
                int tem = a.power;
                do
                    power_len++;
                while (tem /= 10);
                if (power_len < DEFAULT_POWER_LEN)
                    power_len = DEFAULT_POWER_LEN;
            }  // end set power len

            if (floatfield_ == auto_) {  // the shorter one will be printed
                /* here precision stand for significance digit (有效数字) */
                showpoint_ = hn::flags & ios::showpoint;
                if (a.num_len - 1 > a.precision) {
                    body_len = a.precision;
                    floatfield_ = scientific_;
                } else {
                    body_len = a.num_len - 1;
                    floatfield_ = fixed_;
                }
            }  // end auto_ case

            // start floating point cases
            int fixed_len = 0, scientific_len = 0, end_of_number = 0;
            switch (floatfield_) {
                /* showpoint_ need not be set here for fixed_ and scientific_
                 * just stand for using scientific or not. */
                case fixed_:
                    // e.g. 12.345678: a.power + 1 for "12", 1 for ".", precision for...
                    fixed_len = a.power + 1 + 1 + a.precision;
                    body_len = fixed_len + 1;
                    new_memset(body, body_len);
                    // todo
                    break;
                case scientific_:
                    // e.g. 1.234567e+308, 1 for "1", 1 for ".", 2 for "e+", ...
                    scientific_len = 1 + 1 + a.precision + 2 + power_len;
                    body_len = scientific_len + 1;
                    new_memset(body, body_len);
                    /* needs 1 + a.precision numbers for precision,
                     * one more for point
                     * char*: 1.234567 with precision = 6
                     * num[]: 01234567 with the same position*/
                    end_of_number = 1 + a.precision;
                    DO {  // creat a temporary for rounding:
                        int tem_len = end_of_number + 1;  // one more for rounding
                        int *tem = nullptr;
                        new_memset(tem, tem_len);
                        int limit_len =
                                tem_len < a.num_len ? tem_len : a.num_len;
                        COPY_INT(tem, a.num, limit_len);
                        if (tem[2 + a.precision] >= 5)
                            tem[1 + a.precision]++;
                        for (int i = 1 + a.precision; i >= 2; i--) {
                            if (tem[i] > 9)
                                tem[i - 1]++;
                            else
                                break;
                        }
                        body[0] = tem[1] + '0';
                        body[1] = '.';
                        for (int i = 2; i < limit_len; i++)
                            body[i] = tem[i] + '0';
                        for (int i = limit_len; i < 2 + a.precision; i++)
                            body[i] = '0';
                        delete[] tem;
                    }  // end rounding and copy number

                    body[2 + a.precision] =
                    hn::flags & ios::uppercase ? 'E' : 'e';
                    body[3 + a.precision] = a.power >= 0 ? '+' : '-';
                    break;
                default:
                    break;
            }  // end switch floating point cases
        }  // end floating point
    }  // end produce the main body of output

    if ((hn::flags & ios::left ? 1 : 0) + (hn::flags & ios::right ? 1 : 0)
            + (hn::flags & ios::internal ? 1 : 0) > 1)
        hn::flags &= ~ios::adjustfield;
    delete[] body;
    return out;
}  // end function operator<<

// too complex, check needed at runtime
bool hn::string_legal(const std::string & s) throw (invalid_argument) {

    const int len = s.size();
    int location = 0, count_point = 0;
    if (location < len)
        if (s[location] == '-' || s[location] == '+')
            location++;

    // once I cancel below, now I don't know why
    if (location < len)
        if (s[location] == '.') {  // .1 = 0.1
            location++;
            count_point++;
        }
//    if (len <= location)  // no other number e.g. +.
//        return false;
    // todo "." stand for 0 ?

    // this check is a little complex...
    for (int i = location; i < len; i++) {
        if (s[i] >= '0' && s[i] <= '9')
            continue;
        // not number
        if (s[i] == '.') {
            count_point++;
            if (count_point > 1)  // more than ont point is no allowed
                return false;
            else
                continue;
        }
        // illegal charactor, except for e.g. e+308
        if (s[i] == 'e' || s[i] == 'E')
            i++;
        else
            return false;
        if (i < len && (s[i] == '-' || s[i] == '+'))
            i++;
        if (i >= len)
            return false;
        for (; i < len; i++)
            if (s[i] < '0' || s[i] > '9')
                return false;
        return true;
    }  // end check

    return true;
}  // end function string_legal

bool hn::charactor_legal(char c) {
    if (c >= '0' && c <= '9')
        return true;
    else
        return false;
}  // end function charactor_legal

hn &hn::operator=(const hn & a) {
    // 2015-4-11 21:25:48 checked
    if (this == &a)  // check self assignment. useful?
        return *this;
    sign = a.sign;
    power = a.power;
    // todo others
    return *this;
}  // end function operator=

void hn::initialization() {
    sign = 0;  // also mean the number is 0
    power = 0;
    num = NULL;
    // todo allocate ?
}  // end function initialization

// 2015年9月18日11:42:00checked
void hn::reserve(size_t n) {
    if (n <= capacity)
        return;
    int *tem = new int[n];
    memcpy(tem, num, num_len * sizeof(int));  // fixme or capacity ?
    delete[] num;
    num = tem;
    capacity = n;
}  // end function reserve

// 2015年9月18日11:39:49checked
// set all to 0
void hn::clear() {
    memset(num, 0, capacity * 4);
}  // end function clear

void hn::allocate(size_t n) {
    delete[] num;
    num = new int[n];
    num_len = 0;
    capacity = n;
}  // end function allocate

// though this is very long, that's just try to reduce run time
int hn::basic_compare(const hugenum & a, const hugenum & b, int precision = -1,
                      bool ignoresign = false) throw (invalid_argument) {
    // 2015-4-7 21:43:11 fixed: comp_precision may bigger than num_len
    // may this implement not efficient?

    int asign = a.sign, resultchange = 1; /* because we will treat them as positive later
     in the case of they are all negative,
     the result is the negation of the result of comparing |a| and |b|,
     then we will multiply -1 to the result */

    // check cases with negative num
    if (ignoresign == false) {
        if (a.sign < 0 && b.sign < 0)
            resultchange = -1;
        else if (a.sign < 0 && b.sign >= 0)
            return -1;
        else if (a.sign >= 0 && b.sign < 0)
            return 1;
    }

    // we ignore the sign and treat two num as non-negative below

    /* check 0 first because with power = 0
     * it will go wrong when comparing 0 with
     * e.g. 0.01 whose power is negative */
    if (a.sign == 0)
        if (b.sign == 0)
            return 0;
        else
            return -1 * resultchange;  // b positive (maybe just treat it as)
    else if (b.sign == 0)
        return 1 * resultchange;  // a positive (maybe just treat it as)

    // compare the power directly
    if (a.power < b.power)
        return -1 * resultchange;
    else if (a.power > b.power)
        return 1 * resultchange;

    // compare number with the same power begin
    if (precision == DEFAULT)  // the default
        precision = comp_precision;
    int max_len = precision;
    // limit max_len by num_len, the "significance digit" : 有效数字
    if (max_len > a.num_len)
        max_len = a.num_len;
    if (max_len > b.num_len)
        max_len = b.num_len;

    /* first check number before max_len:
     * if we get precision = 6 and two num: 124.0... and 122.9...
     * we get first "different number": "4" and "2"
     * 124 must be bigger because using carry, 122.9 grow to 123 at most
     * this is shown by first "different number": 4 - 2 = 2 > 1
     * while it's like 123.0... and 122.9... further check is needed
     * also, when no "different number" before max_len,
     * another further check is needed */
    bool carrycheck = false;  // whether it's necessary for first further check
    int checkstart = 0, result = 0;  // used for
    const hn *big, *small;
    for (int i = 0; i < max_len; i++) {
        if (a.num[i] > b.num[i]) {
            if (a.num[i] - b.num[i] > 1 || a.num_len <= precision
                    || b.num_len <= precision) {
                return 1 * resultchange;
            } else {
                carrycheck = true;
                checkstart = i;
                big = &a, small = &b;
                result = 1;
                break;
            }
        } else if (b.num[i] < a.num[i]) {
            if (b.num[i] - a.num[i] > 1 || a.num_len <= precision
                    || b.num_len <= precision) {
                return -1 * resultchange;
            } else {
                carrycheck = true;
                checkstart = i;
                big = &b, small = &a;
                result = -1;
                break;
            }
        }
    }
    /* while it's like 123.0... and 122.9... further check explanation:
     * we need to judge whether the numbers before precision of 122.9 are all 9
     *   as well as the number on the precision bigger than 4
     * that is such as 122.9995 or 122.9998, which can carry to 123.000
     *   but not 122.9994 or 122.9895, which can never carry to 123.000
     *
     * for the latter case, the compare finished: 123.0... is bigger
     * for the former case, which shows 122.9 = 123.000,
     * we should next judge whether the numbers before 123.0 are all 0
     *   as well as the number on the precision smaller than 5
     * that is such as 123.0004 or 123.0001, which can carry to 123.000
     *   but not 123.0005 or 123.0104, which can never carry to 123.000
     *
     * for the former case, they are equals
     * for the latter case, 123.0... is bigger */
    if (carrycheck) {
        for (int i = checkstart + 1; i < precision; i++)
            if (small->num[i] != 9)  // can't keep to carry
                return result * resultchange;
        if (small->num[precision] < 5)  // can't carry
            return result * resultchange;

        int max = big->num_len < precision ? big->num_len : precision;
        for (int i = checkstart + 1; i < max; i++)
            if (big->num[i] != 0)  // can carry
                return result * resultchange;
        if (big->num_len >= precision && big->num[precision] > 4)  // can carry
            return result * resultchange;

        return 0;
    }

    // second further check. for no "different number":
    // two cases result in comparsion no result: precision or num_len
    const hn *longer, *shorter;
    if (a.num_len < b.num_len) {
        longer = &b, shorter = &a;
        result = -1;
    } else {
        longer = &a, *shorter = &b;
        result = 1;
    }

    // case 1: comparsion stop above because of precision
    if (shorter->num_len > precision) {
        // compare the number at the precision using carry
        int big = 0, small = 0;
        if (longer->num[precision] > 4)
            big = 1;
        if (shorter->num[precision] > 4)
            small = 1;
        if (big == small)
            return 0;
        else if (big > small)
            return result * resultchange;
        else
            // (big < small)
            return -1 * result * resultchange;
    }

    // case 2: comparsion stop because shorter->num_len, two cases:
    if (longer->num_len <= precision) {
        // longer is bigger because within num_len numbers end with nonzero
        return result * resultchange;
    } else {
        for (int i = max_len; i < precision; i++)
            if (longer->num[i] != 0)
                return result * resultchange;
        if (longer->num[precision] < 5)  // can't carry
            return result * resultchange;
    }
}

hn hn::plus(const hugenum & a, int asign, const hugenum & b, int bsign)
                    throw (invalid_argument) {
    asign *= a.sign;
    bsign *= b.sign;
    // we will use asign and bsign, ignore sign of a and b

    int totalchange = 1;
    /* these are cases whose result is negative,
     * we will ensure the sum in the () is not negative */
    if (asign >= 0 && bsign < 0 && basic_compare(a, b, -1, 1) < 0)
        totalchange = -1;
    else if (asign < 0 && bsign >= 0 && basic_compare(a, b, -1, 1) > 0)
        totalchange = -1;
    else if (asign < 0 && bsign < 0)
        totalchange = -1;

    // check if one is zero. necessary for comparsion with power below
    if (asign == 0)
        if (bsign == 0)
            return b;
        else
            return bsign > 0 ? b : -b;
    else if (b.sign == 0)
        return asign > 0 ? a : -a;

    // todo think about gap > precision
    /*
     * algorithm explanation:
     *          a[0]               a[gap]       i + gap
     *           ↓                   ↓            ↓
     *  up    :  1234567890123456789012345678901234567890...
     *  down  :  |←     gap = 20   →|12345678901234567890...
     *                               ↑            ↑
     *                              b[0]          i
     */
    const hn *up, *down;
    int upsign, downsign;
    if (a.power >= b.power)
        up = &a, upsign = asign, down = &b, downsign = bsign;
    else
        up = &b, upsign = bsign, down = &a, downsign = asign;
    int gap = up->power - down->power;

    int up_len = up->num_len;
    // begin and end is the sum's position at which we copy number from down
    int begin = gap;
    int end = gap + down->num_len;
    if (end > calc_precision)
        end = calc_precision;
    hn result(0);
    result.sign = totalchange;
    result.power = up->power;
    result.num = new int[up_len > end ? up_len : end];

    if (upsign * totalchange >= 0)
        for (int i = 0; i < up_len; i++)
            result.num[i] += up->num[i];
    else
        for (int i = 0; i < up_len; i++)
            result.num[i] -= up->num[i];
    if (downsign * totalchange >= 0)
        for (int i = begin; i < end; i++)
            result.num[i] += down->num[i - gap];
    else
        for (int i = begin; i < end; i++)
            result.num[i] -= down->num[i - gap];

    result.fix();
    return result;
}

// 2015年9月19日17:37:18looked
// fixme this is very important. it should fix all kinds of problems.
// fix the num. only fix the numbers before num_len
void hn::fix() {
    if (num_len == 0)  // for avoid (unsigned int)0 - 1 and other things
        return;

//    if (num_len > calc_precision) {
//        num_len = calc_precision;
//        if (capacity > num_len && num[num_len] >= 5)
//            num[num_len - 1]++;
//        memset(num + num_len, 0, (capacity - num_len)*sizeof(int));
//    }

    carry();

    if (num[0] < 0) {  // the num is negative. we'd better avoid this case
        sign *= -1;
        for (size_t i = num_len - 1; i > 0; i--) {
            num[i - 1]++;
            num[i] -= 10;
            num[i] = -num[i];
        }
        num[0] = -num[0];
    }

    // throw extra 0 away
    int *begin = num, *end = num + num_len;
    while (end != begin && *(end - 1) == 0)
        end--;
    while (begin != end && *begin == 0)
        begin++;
    num_len = end - begin;

    if (num_len == 0) {
        power = 0;
        sign = 0;
        return;
    }

    // fix the begin
    size_t count = 0;
    int tem[11] = { };  // store the carry from the first number
    if (*begin > 9) {
        int n = *begin / 10;
        *begin %= 10;
        while (n) {
            tem[count++] = n % 10;
            n /= 10;
        }
    }
    num_len += count;
    power += count - (begin - num);

    if (num_len > calc_precision) {
        end -= num_len - calc_precision;  // calc_precision should be big enough: longer than the max integer
        num_len = calc_precision;
        if (*end >= 5) {  // rounding, and may cause carry
            *(end - 1)++;  // check extra carry below
            for (int *ptr = end - 1; ptr > begin; ptr--) {
                if (*ptr > 9) {
                    *ptr -= 10;
                    *(ptr - 1)++;
                } else {
                    break;
                }
            }
            if (*begin > 9) {
                *begin -= 10;
                tem[0]++;
                for (int i = 0; i < 10; i++) {
                    if (tem[i] > 9) {
                        tem[i] -= 10;
                        tem[i + 1]++;
                    } else {
                        break;
                    }
                }
            }
        }  // end dealing with carry
    }  // end dealing num too long (looger than precision)

    int *tem_ptr = num;
    bool new_ = false;  // for later delete
    if (capacity < num_len) {
        capacity = num_len;
        num = new int[capacity];
        new_ = true;
    }
    if (count < begin - tem_ptr)
        for (int *i = begin, *j = num + count; i < end; i++, j++)
            *j = *i;
    else if (count > begin - tem_ptr)
        for (int *i = end - 1, *j = num + count - 1 + end - begin; i >= begin;
                i--, j--)
            *j = *i;
    if (count) {
        for (size_t i = 0, j = count - 1; i < count; i++, j--)
            num[j] = tem[i];
        num_len += count;
    }
    if (new_)
        delete[] tem_ptr;

    return;
}  // end function fix_with

// only carry the numbers before num_len
// it will make the first number negative if the num is negative
void hn::carry() {
    if (num_len <= 1)  // especially 0
        return;
    for (size_t i = num_len - 1; i > 0; i--) {
        num[i - 1] += num[i] / 10;
        num[i] %= 10;
        if (num[i] < 0) {
            num[i - 1]--;
            num[i] += 10;
        }
    }
}

#undef DEFAULT_PRECISION
#undef hn
