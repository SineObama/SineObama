/*
 * include and class.h
 *
 *  Created on: 2015年3月5日
 *      Author: Sine
 */

#ifndef SYSTEM_INCLUDE_AND_CLASS_H_
#define SYSTEM_INCLUDE_AND_CLASS_H_

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <iomanip>
#include <string>
#include <conio.h>
#include <io.h>
#include <direct.h>
#include <assert.h>

// orders:
#define ADMIN_MODE 101
#define EXIT 102
#define LOGIN_DORM 103
#define WRONG_ORDER "W"

#define _UNCHANGE -1

#define DATA_FILE_PATH ".\\data"
#define RETURN_FALSE_WITH_MESSAGE printf("返回...\n"); return false;

#define ADMIN_PASSWORD "管理员密码"

#define PRINT_EXCEED(string) printf("输入的 "#string" 超出允许的范围。\n")

#define PASSWORD_MAXLEN 30
#define ORDER_MAXLEN 30

#ifdef NDEBUG
template<typename T>
inline void pr(T x) {return;}
#else
#define pr(x) std::cout << #x" : " << x << std::endl
#endif

void preparation();
void check_and_creat_folder(const char *);
void check_and_creat_file(const char *);
int analyse(char *order, unsigned int maxlen);
int is_number(const char*, unsigned int maxlen);
void enter_administrator_mode();
unsigned int convert_char_to_int(const char*, unsigned int maxlen);
void enter_dorm(int);
void exit_message();
void get_line(char *, unsigned int maxlen);
void clear_current_line();
//int get_input(char *, unsigned int);
//void stop_input_till_enter();
int get_order_num_from_kb(unsigned int min, unsigned int max);

class Date {
 public:
    Date(int year, int month, int day);
    void setyear(int);
    void setmonth(int);
    void setday(int);
    void setdate(int year, int month, int day);
    void print() const;
 private:
    bool leap_year() const;
    int count_maxday() const;
    int year, _month, _day;
};

class message {
 public:
    message(int, int, int, unsigned int, char *);
    ~message();

    void print_message_date() const;
    void print_message_remark() const;
    void print_message() const;
 private:
    Date message_date;
    int expense;
    char remark[255];
};

class dormitory {
 public:
    dormitory(int num);
    ~dormitory();
    bool have_right_to_enter();

    void open_file_for_update();
    void load_data();
    void close_file() const;

    void add_payment();
    void add_expense();
    void check_payment();
    void check_expense();
    void enter_settings();
    const int dorm_num;
 private:
    bool password_exist();
    bool check_password();
    bool set_password();
    bool input_first_password();
    bool input_again_password(const char *);
    void read_password_from_file(char *);
    void write_password_to_file(const char *);

    char password[PASSWORD_MAXLEN];
    message *dorm_message;
    int max_message;
    int password_len;
    FILE *cfPtr;
};

#endif /* SYSTEM_INCLUDE_AND_CLASS_H_ */
