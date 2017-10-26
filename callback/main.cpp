#include<iostream>
#include<string>
#include<cstdio>
#include<cstring>
#include<sstream>
#include <functional>

#include "4.h"

using namespace std;

std::function<int(int)> f1, f2;

//args是一个模板参数包；rest是一个函数参数包
//args表示零个或多个模板类型参数
//rest表示零个或多个函数参数
template<typename T, typename ... args>
void foo(const T &t, const T &t2, const args&... rest) {
    printf("%d %d", t, t2);
}

class hehe {
 public:
    void fff(int a) {
        printf("fff%d", a);
    }
};

template<class T>
void run(T f) {
    (hehe().*f)(1);
}

void ff(int a) {
    printf("ff%d", a);
}

int main() {
    int s[3] = {1, 2, 3};
    int *a = s;
//    printf("%d\n", *++a);
//    printf("%d\n", ++*a);
    printf("%d\n", *a++);
    printf("%d %d %d", s[0], s[1], s[2]);
}
