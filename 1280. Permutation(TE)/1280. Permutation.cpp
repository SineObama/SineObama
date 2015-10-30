#include <cstdio>
#include <cstring>

int s[110][110] = { }, n;
bool r[110] = { };

int get(int sit, int num) {
    if (s[sit][num])
        return s[sit][num];
    int sum = 0;
    if (r[sit]) {
        for (int i = num; i <= n - sit; i++) {
            sum += get(sit + 1, i);
        }
    } else {
        for (int i = 1; i < num; i++) {
            sum += get(sit + 1, i);
        }
    }
    return s[sit][num] = sum;
}

int main() {
    while (scanf("%d", &n) && n != 0) {
        memset(s, 0, 110 * 110 * 4);
        memset(r, 0, 110);
        s[n][1] = 1;
        int t1, t2;
        scanf("%d", &t1);
        for (int i = 1; i < n; i++) {
            scanf("%d", &t2);
            if (t1 < t2)
                r[i] = true;
            t1 = t2;
        }
        int sum = 0;
        for (int i = 1; i <= n; i++)
            sum += get(1, i);
        printf("%d\n", sum);
        for (int i = 1; i <= n; i++) {
            for (int j = 1; j <= n - i + 1; j++) {
                printf("%d ", s[j][i]);
            }
            printf("\n");
        }
    }
}
