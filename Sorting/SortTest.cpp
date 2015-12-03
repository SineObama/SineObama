#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>
#include "BubbleSort.h"
#include "HeapSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "SelectionSort.h"
#include "ShellSort.h"

using namespace Sine;

bool comp(const int &a, const int &b) {
    return a <= b;
}

template<class T>
void test(T *(*func)(T *, std::size_t, bool (*func)(const T &, const T &)),
          T *s, std::size_t len) {
    T *tem = new T[len];
    for (int i = 0; i < len; i++)
        tem[i] = s[i];
    (*func)(tem, len, comp);
    std::ofstream ofs("result");
    for (int i = 0; i < len; i++)
        ofs << tem[i] << ' ';
    ofs << '\n';
    ofs.close();
    delete[] tem;
}

void writeRand(int n, int max) {
    srand(time(NULL));
    std::ofstream ofs("test");
    for (int i = 0; i < n; i++)
        ofs << rand() % max << ' ';
    ofs.close();
}

int readRan(int *s) {
    int len = -1;
    std::ifstream ifs("test");
    while (ifs >> s[++len]) {
    }
    ifs.close();
    return len;
}

#define SIZE 10000

int main() {
    int t = 10;
    bool flag = true;
    while (t-- && flag) {
        int s[SIZE] = { };
        writeRand(SIZE, 1000000);
        int len = readRan(s);
        test(BubbleSort, s, len);
        test(HeapSort, s, len);
        test(InsertionSort, s, len);
        test(MergeSort, s, len);
        test(QuickSort, s, len);
        test(SelectionSort, s, len);
        test(ShellSort, s, len);
        std::sort(s, s + len, comp);
        std::ofstream ofs("standard");
        for (int i = 0; i < len; i++)
            ofs << s[i] << ' ';
        ofs << '\n';
        ofs.close();
        int result[SIZE] = { };
        std::ifstream ifs("result");
        for (int times = 0; times < 7; times++) {
            for (int i = 0; i < len; i++) {
                ifs >> result[i];
                if (result[i] != s[i]) {
                    std::cerr << i << ' ' << times;
                    flag = false;
                    break;
                }
            }
        }
        ifs.close();
    }
}
