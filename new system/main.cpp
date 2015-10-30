/*
 * main.c
 *
 *  Created on: 2015年3月5日
 *      Author: Sine
 */
// 2015年3月 做了一些日子这玩意，也就体验一下C++，class什么的，发现数据处理太麻烦，就不做了
#include "include_and_class.h"

int main() {
    atexit(exit_message);
    printf(">>> 欢迎来到宿舍费用管理系统。\n");
    preparation();
    char order[ORDER_MAXLEN] = { 0 };
    while (1) {
        putchar('\n');
        printf("    请输入宿舍号进入登录宿舍，输入管理员密码进入管理员模式，直接按回车退出：\n");
        get_line(order, ORDER_MAXLEN);
        switch (analyse(order, ORDER_MAXLEN)) {
            case ADMIN_MODE:
                enter_administrator_mode();
                break;
            case EXIT:
                printf(">>> 正在退出...\n");
                exit(0);
            case LOGIN_DORM:
                printf(">>> 正在准备进入宿舍...\n");
                enter_dorm(convert_char_to_int(order, 3));
                break;
            case WRONG_ORDER:
                printf(">>> 命令不正确(宿舍号是三位数字)\n");
                break;
            default:
                assert(1);
                exit(0);
        }
    }
    return 0;
}
