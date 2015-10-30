#include <stdio.h>
int main() {
    char input[100000] = { 0 };
    gets(input);
    long int a[10000] = { 1, 1 };
    while (input[0] != '0') {
        int i = 1;
        while (input[i]) {
            if (input[i - 1] > '2' || input[i - 1] == '0' || (input[i] > '6' && input[i - 1] == '2'))
                a[i + 1] = a[i];
            else
                a[i + 1] = a[i] + a[i - 1];

//            if ((input[i - 1] == '2' && input[i] <= '6') || input[i - 1] == '1')
//                a[i + 1] = a[i] + a[i - 1];
//            else
//                a[i + 1] = a[i];
            i++;
        }
        printf("%ld\n", a[i]);
        gets(input);
    }
    return 0;
}
