/*
 * 1035. DNA matching.cpp
 *
 *  Created on: 2015Äê10ÔÂ13ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <cstring>

int main() {
    int test;
    scanf("%d", &test);
    while (test--) {
        char s[110][110] = { };
        bool used[110] = { };
        int n, count = 0;
        scanf("%d", &n);
        for (int i = 0; i < n; i++)
            scanf("%s", s[i]);
        for (int i = 0; i < n - 1; i++) {
            if (used[i])
                continue;
            for (int j = i + 1; j < n; j++) {
                if (used[j])
                    continue;
                bool ok = true;
                for (int k = 0; k < 110 && s[i][k]; k++) {
                    bool out = false;
                    switch (s[i][k]) {
                        case 'A':
                            if (s[j][k] != 'T')
                                out = true;
                            break;
                        case 'T':
                            if (s[j][k] != 'A')
                                out = true;
                            break;
                        case 'C':
                            if (s[j][k] != 'G')
                                out = true;
                            break;
                        case 'G':
                            if (s[j][k] != 'C')
                                out = true;
                            break;
                    }
                    if (out) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    used[i] = true;
                    used[j] = true;
                    count++;
                    break;
                }
                ok = true;
                for (int k = 0, l = strlen(s[i]) - 1; k < 110 && s[i][k];
                        k++, j--) {
                    bool out = false;
                    switch (s[i][k]) {
                        case 'A':
                            if (s[j][l] != 'T')
                                out = true;
                            break;
                        case 'T':
                            if (s[j][l] != 'A')
                                out = true;
                            break;
                        case 'C':
                            if (s[j][l] != 'G')
                                out = true;
                            break;
                        case 'G':
                            if (s[j][l] != 'C')
                                out = true;
                            break;
                    }
                    if (out) {
                        ok = false;
                        break;
                    }
                }
                if (ok) {
                    used[i] = true;
                    used[j] = true;
                    count++;
                    break;
                }
            }
        }
        printf("%d\n", count);
    }
}
