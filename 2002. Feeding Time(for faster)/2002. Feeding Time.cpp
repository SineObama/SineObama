#include <stdio.h>
#define grass '.'
#define rock '*'

struct {
    char w;
    int find;
} farm[752][752];
int sum[20000] = { 0 }, max_index = 0, row, column, x[500000], y[500000],
        map = 0;

void check(int, int);
void check_around(int, int);

int main() {
    int y2, x2, i;
    scanf("%d %d", &column, &row);
    for (y2 = 1; y2 <= row; y2++) {
        for (x2 = 1; x2 <= column; x2++) {
            scanf(" %c", &farm[y2][x2].w);
        }
    }
    for (y2 = 1; y2 <= row; y2++) {
        for (x2 = 1; x2 <= column; x2++) {
            if (farm[y2][x2].w == grass && farm[y2][x2].find == 0) {
                farm[y2][x2].find = 1;
                sum[++max_index]++;
                x[0] = x2, y[0] = y2, map = 1, i = 0;
                while (i < map) {
                    check_around(y[i], x[i]);
                    i++;
                }
            }
        }
    }
    int max = 0;
    for (i = 1; i <= max_index; i++) {
        if (sum[i] > max) {
            max = sum[i];
        }
    }
    printf("%d\n", max);
    return 0;
}

void check(int m, int n) {
    if (farm[m][n].w == rock || farm[m][n].find != 0 || 0 >= m || 0 >= n
            || m > row || n > column) {
    } else {
        farm[m][n].find = 1;
        sum[max_index]++;
        x[map] = n, y[map] = m;
        map++;
    }
    return;
}

void check_around(int y, int x) {
    check(y - 1, x - 1);
    check(y - 1, x);
    check(y - 1, x + 1);
    check(y, x - 1);
    check(y, x + 1);
    check(y + 1, x - 1);
    check(y + 1, x);
    check(y + 1, x + 1);
    return;
}
