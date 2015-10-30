#include <stdio.h>

int men[10010] = { 0 }, high[20010], low[20010],
        prev[10010] = { 0 }, sum = 0, n, m, poor = 0;

void checkMenHigherThantheHigher(int cur) {  // 对第cur+1个代表的建议进行判断
    if (men[high[cur]] <= men[low[cur]]) {
        men[high[cur]] = men[low[cur]] + 1;
    } else {
        return;
    }
    if (prev[high[cur]] == 1) {  // 如果在当前路径已经找到过
        poor = 1;
        return;
    }
    prev[high[cur]] = 1;
    int i;
    for (i = 0; i < m; i++) {
        if (low[i] == high[cur]) {
            checkMenHigherThantheHigher(i);
            if (poor) {
                return;
            }
        }
    }
    prev[high[cur]] = 0;
    return;
}

int main() {
    int i, j;
    scanf("%d %d", &n, &m);
    for (i = 0; i < m; i++) {
        scanf("%d %d", &high[i], &low[i]);
    }
    for (j = 0; j < m; j++) {
        checkMenHigherThantheHigher(j);
        if (poor) {
            break;
        }
    }
    if (poor) {
        printf("Poor Xed\n");
    } else {
        for (i = 1; i <= n; i++) {
            sum += men[i];
        }
        printf("%d\n", sum + n * 100);
    }
    return 0;
}
