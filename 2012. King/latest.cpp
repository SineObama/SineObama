//#include <stdio.h>
//#include <malloc.h>
//int n;
//char a[1001][1001] = { { 0 } };
//
//void fuc(int cur, int king, int check[]) {
//    if (check[cur] == 0) {
//        check[cur] = 1;
//        int i;
//        for (i = 1; i <= n; i++) {
//            if (a[cur][i] == '1') {
//                a[king][i] = '1';
//                fuc(i, king, check);
//            }
//        }
//    }
//    return;
//}
//
//int main() {
//    int i, j, judge;
//    scanf("%d", &n);
//    for (i = 1; i <= n; i++) {
//        for (j = 1; j <= n; j++) {
//            scanf(" %c", &a[i][j]);
//        }
//    }
//    for (i = 1; i <= n; i++) {
//        int check[1001] = { 0 };
//        fuc(i, i, check);
//        judge = 1;
//        for (j = 1; j <= n; j++) {
//            if (i != j && a[i][j] == '0') {
//                judge = 0;
//                break;
//            }
//        }
//        if (judge == 0 && i != n) {
//            continue;
//        } else if (judge == 1) {
//            printf("%d\n", i);
//            break;
//        } else {
//            printf("-1\n");
//        }
//    }
//    return 0;
//}
