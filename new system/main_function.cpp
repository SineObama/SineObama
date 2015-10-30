/*
 * function.cpp
 *
 *  Created on: 2015��3��5��
 *      Author: Sine
 */

#include "include_and_class.h"

// ��������ļ����Ƿ���ڲ���������
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

// �ַ�������������򷵻����ָ���(��������)�����򷵻�0��ע����������������󳤶����޿��ַ��򷵻�-1
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
    printf(">> �ѽ������Աģʽ��\n");
    printf(">> Ŀǰ����Աģʽ��δ���衣�˳�...\n");
    // TODO
    return;
}

// ��Ȼ��unsigned���ǹ�����֧���ַ�����λ�Ƿ��ŵ�
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
    printf(">> ���� %d ����ɹ������ڶ�ȡ����...\n", num);
    dorm.load_data();
    printf("��ȡ��ϡ�\n");
    char order = -1;
    while (order != 0) {
        putchar('\n');
        printf(">> ������������ţ�\n");
        printf("   1.�������Ͻ���¼\n");
        printf("   2.������֧����¼\n");
        printf("   3.��ѯ����Ͻ���¼\n");
        printf("   4.��ѯ���֧����¼\n");
        printf("   5.����\n");
        printf("   0.�˳���¼\n");
        putchar('\n');

        order = get_order_num_from_kb(0, 5);
        switch (order) {
            // TODO
            case "1":
                printf("--> 1.�������Ͻ���¼\n");
                dorm.add_payment();
                break;
            case "2":
                printf("--> 2.������֧����¼\n");
                dorm.add_expense();
                break;
            case "3":
                printf("--> 3.��ѯ����Ͻ���¼\n");
                dorm.check_payment();
                break;
            case "4":
                printf("--> 4.��ѯ���֧����¼\n");
                dorm.check_expense();
                break;
            case "5":
                printf("--> 5.����\n");
                dorm.enter_settings();
                break;
            case "0":
                printf("--> 0.�˳���¼\n");
                printf("   �����˳�...\n");
                break;
            case WRONG_ORDER:
                printf(">> ��������ȷ��������š�\n");
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
    printf(">>> ϵͳ�ɹ��˳���\n");
    printf("������������������������������������������������\n");
    printf("�밴���������...\n");
    getch();
    return;
}

// ��gets()���ƣ�����һ�������볤�ȵĲ�������ֹ�����±곬����Χ����ȷ�����һλ��
// ע�⣺��getsһ�����������з�ֱ�ӷ��أ�stdin���п��ܵ�һ�����ǻ��з�
void get_a_line(char *input, unsigned int maxlen) {
    register unsigned int count = 0;
    for (count = 0; count < maxlen; count++) {
        scanf("%c", &input[count]);
        if (input[count] == '\n') {
            // ����һֱ��0
            for (; count < maxlen; count++) {
                input[count] = 0;
            }
            return;
        }
    }

    // ���е��˴�˵��������maxlen���ַ���ȴ��û�л��з�
    if (maxlen - 1 >= 0) {
        input[maxlen - 1] = 0;
    }
    clear_current_line();
    return;
}

// ���stdin��ֱ���������з�����ȻҪ��֤�л��з�
void clear_current_line() {
    while (getchar() != '\n') {
    }
    return;
}

// �����������ⷽʽ����������ַ�����������bug�ܶ�
// �˺�����֧���������룬���
// ��û���Ժã����ֿ�������һЩ����ļ���Escʲô�ģ�һ�㲻������ģ��������˰�
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
//    // ���˴�˵��û�ж�����maxlen���ַ�����û�л��з�
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

// ֧������һλ������Ϊ����
char get_order_num_from_kb(unsigned int min = 0, unsigned int max = 9) {
    assert(max < 10);
    unsigned char order = getch();
    if (order < min + '0' || order > max + '0') {
        return WRONG_ORDER;
    } else {
        return order;
    }
}
