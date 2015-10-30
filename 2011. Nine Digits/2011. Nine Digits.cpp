#include <stdio.h>

int check(void);

int a[3][3];

int main() {
    while (~scanf("%d%d%d%d%d%d%d%d%d", &a[0][0], &a[0][1], &a[0][2], &a[1][0],
                 &a[1][1], &a[1][2], &a[2][0], &a[2][1], &a[2][2])) {
        int step = 0;
        while (check() == 0) {

        }
        printf("%d\n", step);
    }
    return 0;
}

int check(void) {
    int i, j, num = 1;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            if (a[i][j] == num) {
                num++;
            } else {
                return 0;
            }
        }
    }
    return 1;
}
