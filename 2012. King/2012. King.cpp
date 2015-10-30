#include <stdio.h>

int main() {
    int n, i, j;
    scanf("%d", &n);
    char tem;
    int child[1001] = { 0 };
    for (i = 1; i <= n; i++) {
        for (j = 1; j <= n; j++) {
            scanf(" %c", &tem);
            if (tem == '1' && child[j] == 0) {
                child[j]--;
            }
        }
    }
    for (i = 1; i <= n; i++) {
        if (child[i] == 0) {
            printf("%d\n", i);
            break;
        }
        if (i == n) {
            printf("-1\n");
        }
    }
    return 0;
}
