#include <cstdio>
#include <algorithm>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <cstring>
#include <string>
#include "BubbleSort.h"
#include "HeapSort.h"
#include "InsertionSort.h"
#include "MergeSort.h"
#include "QuickSort.h"
#include "SelectionSort.h"
#include "ShellSort.h"

using namespace Sine;

#define test(sort, start, end, comp, type) {\
    std::size_t len = end - start;\
    type *tem = new type[len];\
    for (std::size_t i = 0; i < len; i++)\
        tem[i] = start[i];\
    clock_t start_t, end_t;\
    start_t = clock();\
    sort(tem, tem + len, comp);\
    end_t = clock();\
    printf("%d\n", int(end_t - start_t));\
    std::ofstream ofs("result.txt", std::ios::app);\
    for (std::size_t i = 0; i < len; i++)\
        ofs << tem[i] << ' ';\
    ofs << '\n';\
    ofs.close();\
    delete[] tem;\
}
void writeRand(int n, int max);
int readRan(int *s);

#define SIZE 10000

bool comp(const int &a, const int &b) {
    return a < b;
}

#define sss(s) s aaa;

int main() {
    std::ofstream ofs("result.txt");  // clear
    ofs.close();
    typedef int test_t;
    int t = 1;
    bool foundError = false;
    while (t-- && !foundError) {
        int s[SIZE] = { };
        writeRand(SIZE, 100000);
        int len = readRan(s);
        test(BubbleSort, s, s + len, comp, test_t);
//        test(HeapSort, s, s + len, comp);
//        test(InsertionSort, s, s + len, comp);
//        test(MergeSort, s, s + len, comp);
//        test(QuickSort, s, s + len, comp);
//        test(SelectionSort, s, s + len, comp);
//        test(ShellSort, s, s + len, comp);
//        test(std::sort, s, s + len, comp);
        {  // standard generator
            std::sort(s, s + len, comp);
//            BubbleSort(s, len, comp);
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
                    if (ifs.eof())
                        break;
                    if (tem != s[i]) {
                        std::cerr << times << ' ' << i << ' ' << tem << ' '
                                  << s[i] << '\n';
                        foundError = true;
                        break;
                    }
                }
            }
            ifs.close();
        }
    }
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
