#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <set>

#define MAX 30
//#define GUESS 300

struct statee {
    short s[8];
    char ss[MAX];
    short len;
    statee() {
        memset(ss, 0, MAX);
        len = 0;
    }
};

bool operator==(const statee &a, const statee &b) {
    for (int i = 0; i < 8; i++)
        if (a.s[i] != b.s[i])
            return false;
    return true;
}

bool operator<(const statee &a, const statee &b) {
    for (int i = 0; i < 8; i++)
        if (a.s[i] < b.s[i])
            return true;
        else if (a.s[i] > b.s[i])
            return false;
    return false;
}

statee A(const statee &a) {
    statee re;
    re.s[0] = a.s[4];
    re.s[1] = a.s[5];
    re.s[2] = a.s[6];
    re.s[3] = a.s[7];
    re.s[4] = a.s[0];
    re.s[5] = a.s[1];
    re.s[6] = a.s[2];
    re.s[7] = a.s[3];
    re.len = a.len + 1;
    memcpy(re.ss, a.ss, a.len);
    re.ss[a.len] = 'A';
    return re;
}

statee B(const statee &a) {
    statee re;
    re.s[0] = a.s[3];
    re.s[1] = a.s[0];
    re.s[2] = a.s[1];
    re.s[3] = a.s[2];
    re.s[4] = a.s[7];
    re.s[5] = a.s[4];
    re.s[6] = a.s[5];
    re.s[7] = a.s[6];
    re.len = a.len + 1;
    memcpy(re.ss, a.ss, a.len);
    re.ss[a.len] = 'B';
    return re;
}

statee C(const statee &a) {
    statee re;
    re.s[0] = a.s[0];
    re.s[1] = a.s[5];
    re.s[2] = a.s[1];
    re.s[3] = a.s[3];
    re.s[4] = a.s[4];
    re.s[5] = a.s[6];
    re.s[6] = a.s[2];
    re.s[7] = a.s[7];
    re.len = a.len + 1;
    memcpy(re.ss, a.ss, a.len);
    re.ss[a.len] = 'C';
    return re;
}

int main() {
    int n;
    statee begin;
    begin.s[0] = 1;
    begin.s[1] = 2;
    begin.s[2] = 3;
    begin.s[3] = 4;
    begin.s[4] = 8;
    begin.s[5] = 7;
    begin.s[6] = 6;
    begin.s[7] = 5;
    while (scanf("%d", &n) && n != -1) {
//        if (n > GUESS)
//            break;
        statee target;
        for (int i = 0; i < 8; i++)
            scanf("%hd", &target.s[i]);
        std::queue<statee> st;
        std::set<statee> se;
        st.push(begin);
        se.insert(begin);
        int M = -1;
        char ss[MAX] = { };
        while (!st.empty()) {
            statee cur = st.front(), tem;
            st.pop();
            if (cur.len >= n)
                break;

            tem = C(cur);
            if (se.insert(tem).second) {
                if (tem == target) {
                    M = tem.len;
                    memcpy(ss, tem.ss, M);
                    break;
                } else {
                    st.push(tem);
                }
            }
            tem = B(cur);
            if (se.insert(tem).second) {
                if (tem == target) {
                    M = tem.len;
                    memcpy(ss, tem.ss, M);
                    break;
                } else {
                    st.push(tem);
                }
            }
            tem = A(cur);
            if (se.insert(tem).second) {
                if (tem == target) {
                    M = tem.len;
                    memcpy(ss, tem.ss, M);
                    break;
                } else {
                    st.push(tem);
                }
            }
        }
        if (M == -1)
            printf("-1\n");
        else if (M == 0)
            printf("0\n");
        else
            printf("%d  %s\n", M, ss);
    }
}
