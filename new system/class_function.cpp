/*
 * class function.cpp
 *
 *  Created on: 2015��3��5��
 *      Author: Sine
 */

#include "include_and_class.h"

Date::Date(int new_year = 2000, int new_month = 1, int new_day = 1) {
    year = new_year;
    _month = new_month;
    _day = new_day;
    return;
}

void Date::setyear(int new_year) {
    if (new_year != _UNCHANGE) {
        return;
    } else if (new_year < 0 || new_year > 9999) {
        PRINT_EXCEED("���");
    } else {
        year = new_year;
    }
    return;
}

void Date::setmonth(int new_month) {
    if (new_month != _UNCHANGE) {
        return;
    } else if (new_month < 1 || new_month > 12) {
        PRINT_EXCEED("�·�");
    }
    return;
}

void Date::setday(int new_day) {
    if (new_day != _UNCHANGE) {
        return;
    } else {
        int max_day = count_maxday();
        if (new_day < 1 || new_day > max_day) {
            PRINT_EXCEED("��");
        } else {
            _day = new_day;
        }
    }
    return;
}

int Date::count_maxday() const {
    switch (_month) {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            return 31;
        case 4:
        case 6:
        case 9:
        case 11:
            return 30;
        case 2:
            if (leap_year()) {
                return 29;
            } else {
                return 28;
            }
        default:
            assert(_month >= 1 && _month <= 12);
            return 0;
    }
}

bool Date::leap_year() const {
    if (year % 400 == 0) {
        return true;
    } else if (year % 100 == 0) {
        return false;
    } else if (year % 4 == 0) {
        return true;
    } else {
        return false;
    }
}

void Date::setdate(int new_year, int new_month, int new_day) {
    setyear(new_year);
    setmonth(new_month);
    setday(new_day);
    return;
}

void Date::print() const {
    printf("%4d.%2d.%2d", year, _month, _day);
    return;
}

message::message(int new_year = 2000, int new_month = 1, int new_day = 1,
                 unsigned int new_expense = 0, char *new_remark = NULL)
        : message_date(new_year, new_month, new_day) {
    // TODO
    expense = new_expense;
    // ����Ĭ��ֵ������
    return;
}

message::~message() {
    // TODO
    return;
}

void message::print_message_date() const {
    message_date.print();
    return;
}

void message::print_message_remark() const {
    printf("%d\n", remark);
    return;
}

void message::print_message() const {
    print_message_date();
    print_message_remark();
    putchar('\n');
    return;
}

dormitory::dormitory(int num)
        : dorm_num(num) {
    open_file_for_update();
    read_password_from_file(password);
    dorm_message = NULL;
    password_len = 0;
    max_message = 0;
    return;
}

dormitory::~dormitory() {
    close_file();
    // TODO
    return;
}

bool dormitory::have_right_to_enter() {
    if (password_exist()) {
        printf(">>> �������� %d ����...\n", dorm_num);
        return check_password();
    } else {
        printf(">>> ���� %d ��δ�������롣������������...\n", dorm_num);
        return set_password();
    }
}

void dormitory::open_file_for_update() {
    char file_full_path[100] = { 0 }, file_path[100] = { 0 };
    sprintf(file_path, "%s\\dorm_%d", DATA_FILE_PATH, dorm_num);
    check_and_creat_folder(file_path);
    sprintf(file_full_path, "%s\\dorm_%d\\dormitory_%d.dat", DATA_FILE_PATH,
            dorm_num, dorm_num);
    check_and_creat_file(file_full_path);
    cfPtr = fopen(file_full_path, "r+");
    return;
}

void dormitory::load_data() {
    // TODO
    return;
}

void dormitory::close_file() const {
    fclose(cfPtr);
    return;
}

bool dormitory::password_exist() {
    fseek(cfPtr, 0, SEEK_SET);
    char s[30] = { 0 };
    fgets(s, 29, cfPtr);
    if (s[1] != 0) {
        return true;
    } else {
        return false;
    }
}

bool dormitory::check_password() {
    putchar('\n');
    printf(">> ����������(ֱ�ӻس����Է���)��\n");
    fseek(cfPtr, 0, SEEK_SET);
    char input[PASSWORD_MAXLEN] = { 0 };
    get_line(input, sizeof(input));
    if (input[0] == 0) {
        // ֱ�ӻس�����
        RETURN_FALSE_WITH_MESSAGE
    }
    while (strcmp(input, password) != 0) {
        printf(">> ���벻��ȷ������������(ֱ�ӻس����Է���)��\n");
        get_line(input, sizeof(input));
        if (input[0] == 0) {
            RETURN_FALSE_WITH_MESSAGE
        }
    }
    if (strcmp(input, password) != 0) {
        return false;
    } else {
        printf(">> ������ȷ...\n");
        return true;
    }
}

bool dormitory::set_password() {
    if (password_exist() == true) {
        if (check_password() == false) {
            return false;
        }
    }
    return input_first_password();
}

bool dormitory::input_first_password() {
    bool restart = true;
    char input[PASSWORD_MAXLEN] = { 0 };
    while (restart) {
        putchar('\n');
        printf(">> �����������룬�����ǰ�ǵ�������ĸ���ţ����������ģ��20λ�����4λ(ֱ�ӻس����Է���)��\n");
        get_line(input, PASSWORD_MAXLEN);
        if (input[0] == 0) {
            return false;
        }
        restart = false;
        if (strlen(input) > 20 || strlen(input) < 4) {
            printf(">> ���볤�Ȳ�ǡ����\n");
            restart = true;
        }
        int i = 0;
        for (i = 0; i < 20 && input[i]; i++) {
            if (input[i] < 0 || input[i] > 127) {
                printf(">> ���뺬��ǡ���ַ���\n");
                restart = true;
                break;
            }
        }
    }
    printf(">> ��������ɹ���\n");
    return input_again_password(input);
}

bool dormitory::input_again_password(const char *new_password) {
    putchar('\n');
    printf(">> ���ٴ������������ȷ��(ֱ�ӻس����Է���)��\n");
    char again_input[PASSWORD_MAXLEN] = { 0 };
    get_line(again_input, PASSWORD_MAXLEN);
    // �س�����
    if (again_input[0] == 0) {
        return false;
    }
    while (strcmp(new_password, again_input) != 0) {
        putchar('\n');
        printf(">> ������ղ�����Ĳ����������������(ֱ�ӻس����Է���)��\n");
        get_line(again_input, PASSWORD_MAXLEN);
        // �س�����
        if (again_input[0] == 0) {
            return false;
        }
    }
    password_len = strlen(new_password);
    strcpy(password, new_password);
    write_password_to_file(new_password);
    printf(">> �����޸ĳɹ���\n");
    return true;
}

void dormitory::read_password_from_file(char *s) {
    fscanf(cfPtr, "%d", password_len);
    clear_current_line();
    get_line(s, password_len);
    if (s[strlen(s) - 1] == '\n') {
        s[strlen(s) - 1] = 0;
    }
    return;
}

void dormitory::write_password_to_file(const char *s) {
    fseek(cfPtr, 0, SEEK_SET);
    fprintf(cfPtr, "%d\n", password_len);
    fprintf(cfPtr, "%s\n", s);
    return;
}

void dormitory::add_payment() {
    // TODO
    return;
}

void dormitory::add_expense() {
    // TODO
    return;
}

void dormitory::check_payment() {
    // TODO
    return;
}

void dormitory::check_expense() {
    // TODO
    return;
}

void dormitory::enter_settings() {
    // TODO
    return;
}
