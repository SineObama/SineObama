//#include <stdio.h>
//int main() {
//    char input[100000] = { 0 };
//    gets(input);
//    long long a[10000] = { 1, 1, 2, 3, 5 }, max = 4, ways;
//    while (input[0] != '0') {
//        int cut = 0, i = 0;
//        ways = 1;
//        while (input[i]) {
//            if (input[i] > '2' || input[i] == '0' || input[i + 1] == 0) {
//                if (i > 0 && input[i - 1] == '2' && input[i] > '6') {//fixme
//                } else {
//                    cut++;
//                }
//                while (cut > max) {
//                    max++;
//                    a[max] = a[max - 1] + a[max - 2];
//                }
//                ways *= a[cut];
//                cut = 0;
//            } else {
//                cut++;
//            }
//            i++;
//        }
//        printf("%ld\n", ways);
//        gets(input);
//    }
//    return 0;
//}
