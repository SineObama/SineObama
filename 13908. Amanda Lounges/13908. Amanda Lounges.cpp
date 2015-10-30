#include <cstdio>
int n, m, i, j, s[200200] = { 0 }, e[200200] = { 0 }, r[200200] = { 0 },
        u[200200] = { 0 };

int suan(int i) {
    int j = i - 1;
    for (; j >= 0; j--) {
        if (u[s[j]] + u[e[j]] - r[j] > 0) {
            return n + 1;
        }
    }
    if (i >= m) {
        return 0;
    }
    if (r[i] == 2) {
        return suan(i + 1);
    }
    int check = u[s[i]] + u[e[i]] - r[i];
    if (check == 0) {
        return suan(i + 1);
    } else if (check > 0) {
        return n + 1;
    } else {
        int case1, case2;
        u[s[i]] = 1;
        case1 = suan(i + 1);
        u[s[i]] = 0;
        u[e[i]] = 1;
        case2 = suan(i + 1);
        if (case1 > case2) {
            return 1 + case2;
        } else {
            u[s[i]] = 1;
            u[e[i]] = 0;
            return 1 + case1;
        }
    }
}

int main() {
    scanf("%d %d", &n, &m);
    for (i = 0; i < m; i++) {
        scanf("%d %d %d", &s[i], &e[i], &r[i]);
    }
    int count = 0;
    for (i = 0; i < m; i++) {
        if (r[i] == 2) {
            if (u[s[i]] == 0) {
                count++;
                u[s[i]] = 1;
            }
            if (u[e[i]] == 0) {
                count++;
                u[e[i]] = 1;
            }
        }
    }
    int tem;
    if ((tem = suan(0)) <= n) {
        printf("%d\n", count + tem);
    } else {
        printf("impossible\n");
    }
    return 0;
}
