/*
 * main.c
 *
 *  Created on: 2015��3��5��
 *      Author: Sine
 */
// 2015��3�� ����һЩ���������⣬Ҳ������һ��C++��classʲô�ģ��������ݴ���̫�鷳���Ͳ�����
#include "include_and_class.h"

int main() {
    atexit(exit_message);
    printf(">>> ��ӭ����������ù���ϵͳ��\n");
    preparation();
    char order[ORDER_MAXLEN] = { 0 };
    while (1) {
        putchar('\n');
        printf("    ����������Ž����¼���ᣬ�������Ա����������Աģʽ��ֱ�Ӱ��س��˳���\n");
        get_line(order, ORDER_MAXLEN);
        switch (analyse(order, ORDER_MAXLEN)) {
            case ADMIN_MODE:
                enter_administrator_mode();
                break;
            case EXIT:
                printf(">>> �����˳�...\n");
                exit(0);
            case LOGIN_DORM:
                printf(">>> ����׼����������...\n");
                enter_dorm(convert_char_to_int(order, 3));
                break;
            case WRONG_ORDER:
                printf(">>> �����ȷ(���������λ����)\n");
                break;
            default:
                assert(1);
                exit(0);
        }
    }
    return 0;
}
