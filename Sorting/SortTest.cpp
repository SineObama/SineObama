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

template<class T, typename Comp>
void test(Comp func, T *s, std::size_t len) {
    T *tem = new T[len];
    for (int i = 0; i < len; i++)
        tem[i] = s[i];
    clock_t st, e;
    st = clock();
    (*func)(tem, len, comp);
    e = clock();
    printf("%d\n", int(e - st));
    std::ofstream ofs("result.txt", std::ios::app);
    for (int i = 0; i < len; i++)
        ofs << tem[i] << ' ';
    ofs << '\n';
    ofs.close();
    delete[] tem;
}

void writeRand(int n, int max) {
    srand(time(NULL));
    std::ofstream ofs("test.txt");
    for (int i = 0; i < n; i++)
        ofs << rand() % max << ' ';
    ofs.close();
}

int readRan(int *s) {
    int len = -1;
    std::ifstream ifs("test.txt");
    while (ifs >> s[++len]) {
    }
    ifs.close();
    return len;
}

template<typename Sort>
void ttt(Sort s) {

}

#define SIZE 1000

int main() {
    ttt(std::sort);
    std::ofstream ofs("result.txt");  // clear
    ofs.close();
    int t = 1;
    bool foundError = false;
    while (t-- && !foundError) {
        int s[SIZE] = { };
//        writeRand(SIZE, 1);
        int len = readRan(s);
        test(BubbleSort<int>, s, len);
        test(HeapSort<int>, s, len);
        test(InsertionSort<int>, s, len);
        test(MergeSort<int>, s, len);
//        test(QuickSort<int>, s, len);
        test(SelectionSort<int>, s, len);
        test(ShellSort<int>, s, len);

        {  // standard generator
//        std::sort(s, s + len, comp);
            BubbleSort(s, len, comp);
            std::ofstream ofs("standard.txt");
            for (int i = 0; i < len; i++)
                ofs << s[i] << ' ';
            ofs << '\n';
            ofs.close();
        }

        {  // checking
            std::ifstream ifs("result.txt");
            for (int times = 0; times < 7; times++) {
                int tem;
                for (int i = 0; i < len; i++) {
                    ifs >> tem;
                    if (tem != s[i]) {
                        std::cerr << i << ' ' << times;
                        foundError = true;
                        break;
                    }
                }
            }
            ifs.close();
        }
    }
}
