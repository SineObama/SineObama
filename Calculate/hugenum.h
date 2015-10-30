/*
 * hugenum.h
 *
 *  Created on: 2015年3月24日
 *      Author: Sine
 */

// hugenum saved at 2015-4-8 23:13:18. no bug found for now. next to do: formatted output
#ifndef HUGENUM_H_
#define HUGENUM_H_

// todo register?优化减内存？inline?各处处理0的符号.最大公约数.limit num_len
// todo ！！！隐式转换时候没有默认构造函数初始化指针，构造函数可能delete一个野指针
// todo try overloading e.g. char *
// 暂时约定constructor都调用初始化函数来申请内存，set函数可以看做另外调用，调用时默认已初始化指针
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <cassert>
#include <iomanip>
#include <stdexcept>

#define nullptr NULL
#define DEFAULT -1

class hugenum {

    friend hugenum operator+(const hugenum &);
    friend hugenum operator-(const hugenum &);
    friend hugenum abs(const hugenum &);
    friend hugenum operator+(const hugenum &, const hugenum &);
    friend hugenum operator-(const hugenum &, const hugenum &);
    friend hugenum operator*(const hugenum &, const hugenum &);
    friend hugenum operator/(hugenum, hugenum);  // depend on binary operator -
    friend hugenum radical(hugenum, std::size_t root = 2, std::size_t time =
    DEFAULT) throw (invalid_argument);  // -1 for default

    // todo the precision....
    // return 1 while a > b, -1 while a < b, 0 while a = b
    friend int compare(const hugenum &, const hugenum &,
                       int comp_precision = -1);  // -1 for default
    friend bool operator <(const hugenum & a, const hugenum & b);
    friend bool operator >(const hugenum & a, const hugenum & b);
    friend bool operator <=(const hugenum & a, const hugenum & b);
    friend bool operator >=(const hugenum & a, const hugenum & b);
    friend bool operator ==(const hugenum & a, const hugenum & b);
    friend bool operator !=(const hugenum & a, const hugenum & b);

 public:

    enum {
        max_unit = 4097,
        min_unit = 2,
//        unit_size = sizeof(int),
        radical_max_time = 1 << 10,  // todo set/get default radical time
        radical_min_time = 1 << 5,
//        ios_add_space = 1 << 29
    };
    static const char *my_PI;
    static const char *my_E;
//    hugenum(int system = 10);
    hugenum(const hugenum &);
    explicit hugenum(double);  // todo for constructor the third parameter for capacity ?
    explicit hugenum(const std::string &);
    explicit hugenum(char);
    explicit hugenum(short);
    explicit hugenum(int);
    explicit hugenum(long long);
    hugenum(const int n[], int len);
    explicit hugenum(const std::vector<int> &);

    void set(double);
    void set(const std::string &);
    void set(char n) throw (std::invalid_argument);  // just for fun
    void set(short);
    void set(int);
    void set(long long);
    // start at [0], as the highest number
    void set(const int n[], int len);
    void set(const std::vector<int> &);

    // if fail, return -1, else return previous precision
    static std::size_t set_comp_precision();  // todo change name: comp_precision...
    static std::size_t set_comp_precision(std::size_t) throw (invalid_argument);
    static std::size_t set_calc_precision();
    static std::size_t set_calc_precision(std::size_t) throw (invalid_argument);
    static bool fix_double();
    static bool fix_double(bool);

    friend int sprint(char *s, const char *order, const hugenum & a) {
        return snprint(s, 1 << 31, order, a);
    }
    friend int snprint(char *s, int max, const char *order, const hugenum &);
    friend int print(const char *order, const hugenum &);
    friend std::istream &operator>>(std::istream &, hugenum &);
    friend std::ostream &operator<<(std::ostream &, const hugenum &);
    static bool string_legal(const std::string &) throw (std::invalid_argument);
    static bool charactor_legal(char);

    hugenum &operator=(const hugenum &);
#define operator_assignment(t) hugenum &operator t##=\
    (const hugenum & a) {return *this = *this t a; }
    operator_assignment(+)
    operator_assignment(-)
    operator_assignment(*)
    operator_assignment(/)
#undef operator_assignment

 protected:

    // sability......
    void initialization();
    void reserve(std::size_t);
    void clear();
    void allocate(std::size_t);
    //  friend std::ostream &base_print(std::ostream & out, const hugenum & a);  // use self flags...to print. ensure valid befor calling this
    // return 1 while a > b, -1 while a < b, 0 while a = b
    static int basic_compare(const hugenum &, const hugenum &, int precision =
            -1,
                             bool ignoresign = false);
    static hugenum plus(const hugenum & a, int asign, const hugenum & b,
                        int bsign);  // the implement of +-
    void fix();
    void carry();  // fix num to be in the scope : 进位

 private:

    // todo e.g. change name
    int sign;  // -1 for negative, 0 for zero, 1 for positive
    int power;  // e.g. e+308
    int *num;
    std::size_t num_len;  // size. todo what if it's 0
    std::size_t capacity;

    static std::size_t comp_precision; /* used in comparison 即有效数字.
     never bigger than the calc_precision. omitted at radical's comparsion. */
    static std::size_t calc_precision; /* the max precision a hugenum may have.
     between the min_unit and the max_unit. */
    static std::size_t radical_time;  // calculate times at radical
    static bool _fix;

    // data used in formatted output
//    static int flags;
//    static int width;
//    static int precision;
//    static char fill;
//    static bool resetting; /* used to adopt C-style flags through calling
//     operator>>, which normally get flags from std::cout */

};

#endif /* HUGENUM_H_ */
