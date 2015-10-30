/*
 * function.cpp
 *
 *  Created on: 2015年3月5日
 *      Author: Sine
 */

#include "include_and_class.h"

// 检测数据文件夹是否存在并创建……
void preparation() {
    check_and_creat_folder(DATA_FILE_PATH);
    check_and_creat_folder(DATA_FILE_PATH"\\manager");
    return;
}

void check_and_creat_folder(const char *path) {
    if (access(path, 0) != 0) {
        mkdir(path);
    }
    return;
}

void check_and_creat_file(const char *path) {
    if (access(path, 0) != 0) {
        if (access(path, 0) != 0) {
            FILE *tem = fopen(path, "w");
            fclose(tem);
        }
    }
    return;
}

int analyse(char *s, unsigned int maxlen) {
    if (strcmp(s, ADMIN_PASSWORD) == 0) {
        return ADMIN_MODE;
    } else if (s[0] == 0) {
        return EXIT;
    } else if (is_number(s, maxlen) == 3) {
        return LOGIN_DORM;
    } else {
        return WRONG_ORDER;
    }
}

// 字符串由数字组成则返回数字个数(不允许负号)，否则返回0。注意特殊情况：如果最大长度内无空字符则返回-1
int is_number(const char *s, unsigned int maxlen) {
    register unsigned int count;
    for (count = 0; count < maxlen; count++) {
        if (s[count] > '9' || s[count] < '0') {
            if (s[count] == 0) {
                return count;
            }
            return false;
        }
    }
    return -1;
}

void enter_administrator_mode() {
    printf(">> 已进入管理员模式。\n");
    printf(">> 目前管理员模式还未建设。退出...\n");
    // TODO
    return;
}

// 虽然是unsigned但是功能上支持字符串首位是符号的
unsigned int convert_char_to_int(const char *s, unsigned int maxlen) {
    if (s[0] == '-' && is_number(&s[1], 1)) {
        return -convert_char_to_int(&s[1], maxlen - 1);
    } else if (s[0] == '+' && is_number(&s[1], 1)) {
        return +convert_char_to_int(&s[1], maxlen - 1);
    }
    register unsigned int num = 0, count = 0;
    while (count < maxlen) {
        assert(is_number(&s[count], 1));
        num *= 10;
        num += s[count] - '0';
        count++;
    }
    return num;
}

void enter_dorm(int num) {
    dormitory dorm(num);
    if (dorm.have_right_to_enter() == false) {
        return;
    }
    putchar('\n');
    printf(">> 进入 %d 宿舍成功，正在读取数据...\n", num);
    dorm.load_data();
    printf("读取完毕。\n");
    char order = -1;
    while (order != 0) {
        putchar('\n');
        printf(">> 请输入命令代号：\n");
        printf("   1.添加舍费上交记录\n");
        printf("   2.添加舍费支出记录\n");
        printf("   3.查询舍费上交记录\n");
        printf("   4.查询舍费支出记录\n");
        printf("   5.设置\n");
        printf("   0.退出登录\n");
        putchar('\n');

        order = get_order_num_from_kb(0, 5);
        switch (order) {
            // TODO
            case "1":
                printf("--> 1.添加舍费上交记录\n");
                dorm.add_payment();
                break;
            case "2":
                printf("--> 2.添加舍费支出记录\n");
                dorm.add_expense();
                break;
            case "3":
                printf("--> 3.查询舍费上交记录\n");
                dorm.check_payment();
                break;
            case "4":
                printf("--> 4.查询舍费支出记录\n");
                dorm.check_expense();
                break;
            case "5":
                printf("--> 5.设置\n");
                dorm.enter_settings();
                break;
            case "0":
                printf("--> 0.退出登录\n");
                printf("   正在退出...\n");
                break;
            case WRONG_ORDER:
                printf(">> 请输入正确的命令代号。\n");
                break;
            default:
                assert(0);
                exit(0);
        }
    }
    return;
}

void exit_message() {
    putchar('\n');
    printf(">>> 系统成功退出。\n");
    printf("————————————————————————\n");
    printf("请按任意键继续...\n");
    getch();
    return;
}

// 与gets()类似，增加一个最大读入长度的参数，防止数组下标超出范围，并确保最后一位空
// 注意：与gets一样，遇到换行符直接返回，stdin流中可能第一个就是换行符
void get_a_line(char *input, unsigned int maxlen) {
    register unsigned int count = 0;
    for (count = 0; count < maxlen; count++) {
        scanf("%c", &input[count]);
        if (input[count] == '\n') {
            // 往后一直补0
            for (; count < maxlen; count++) {
                input[count] = 0;
            }
            return;
        }
    }

    // 运行到此处说明读入了maxlen个字符，却还没有换行符
    if (maxlen - 1 >= 0) {
        input[maxlen - 1] = 0;
    }
    clear_current_line();
    return;
}

// 清楚stdin流直到遇到换行符，当然要保证有换行符
void clear_current_line() {
    while (getchar() != '\n') {
    }
    return;
}

// 这里用了奇葩方式限制输入的字符数，看起来bug很多
// 此函数不支持中文输入，奇怪
// 还没调试好，发现可能输入一些奇葩的键，Esc什么的，一般不该输入的，就弃用了吧
//int get_input(char *input, unsigned int maxlen) {
//    register unsigned int count = 0;
//    while (1) {
//        input[count] = getch();
//        if (input[count] == 13) {
//            putchar('\n');
//            input[count] = 0;
//            return count;
//        }
//        if (count < maxlen - 1 and input[count] not_eq 8) {
//            putchar(input[count]);
//        }
//        if (input[count] == 8) {
//            printf("\b \b");
//            count -= 2;
//            if (count < -1) {
//                count = -1;
//            }
//        }
//        if (count < maxlen - 1) {
//            count++;
//        }
//    }
//    // 到此处说明没有读入了maxlen个字符，还没有换行符
//    input[maxlen - 1] = 0;
//    stop_input_till_enter();
//    return maxlen - 1;
//}

//void stop_input_till_enter() {
//    while (getch() != 13) {
//    }
//    printf("\n");
//    return;
//}

// 支持输入一位数字作为命令
char get_order_num_from_kb(unsigned int min = 0, unsigned int max = 9) {
    assert(max < 10);
    unsigned char order = getch();
    if (order < min + '0' || order > max + '0') {
        return WRONG_ORDER;
    } else {
        return order;
    }
}
