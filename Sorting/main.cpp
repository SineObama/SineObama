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

#define string(s) #s
#define test(sort, start, end, comp, type) {\
    type *tem = new type[end - start];\
    for (int i = 0; i < (end - start); i++)\
        tem[i] = start[i];\
    clock_t start_t, end_t;\
    start_t = clock();\
    sort(tem, tem + (end - start), comp);\
    end_t = clock();\
    printf("%30s:%5d ms\n", string(sort), int(end_t - start_t));\
    std::ofstream ofs("result.txt", std::ios::app);\
    for (int i = 0; i < (end - start); i++)\
        ofs << tem[i] << ' ';\
    ofs << '\n';\
    ofs.close();\
    delete[] tem;\
}

typedef int test_t;

void writeRandInt(int n, test_t max);
int readRan(test_t *s);

#define SIZE 200000

bool comp(const test_t &a, const test_t &b) {
    return a <= b;
}

int main() {
    int len;
    test_t *s = new test_t[SIZE];
    int t = 1;
    bool foundError = false;
    while (t-- && !foundError) {
        std::ofstream ofs("result.txt");  // clear
        ofs.close();
        writeRandInt(SIZE, 200000);
        len = readRan(s);
//        test(BubbleSort, s, s + len, comp, test_t);
//        test(HeapSort, s, s + len, comp, test_t);
//        test(InsertionSort, s, s + len, comp, test_t);
//        test(MergeSort, s, s + len, comp, test_t);
//        test(QuickSort, s, s + len, comp, test_t);
//        test(QuickSort1, s, s + len, comp, test_t);
        test(QuickSortStandard, s, s + len, comp, test_t);
//        test(QuickSort1_1, s, s + len, comp, test_t);
//        test(QuickSort1_2, s, s + len, comp, test_t);
//        test(QuickSort2, s, s + len, comp, test_t);
//        test(SelectionSort, s, s + len, comp, test_t);
//        test(ShellSort, s, s + len, comp, test_t);
//        test(ShellSort1, s, s + len, comp, test_t);
        test(std::sort, s, s + len, comp, test_t);
        test(std::stable_sort, s, s + len, comp, test_t);

        {  // standard generator
//            std::sort(s, s + len, comp);
            ShellSort(s, s + len, comp);
            std::ofstream ofs("standard.txt");
            for (int i = 0; i < len; i++)
                ofs << s[i] << ' ';
            ofs << '\n';
            ofs.close();
        }

        {  // checking
            std::ifstream ifs("result.txt");
            for (std::size_t times = 0; times < 7; times++) {
                int tem;
                for (int i = 0; i < len; i++) {
                    ifs >> tem;
                    if (ifs.rdstate())
                        break;
                    if (tem != s[i]) {
                        std::cerr << times << ' ' << i << ' ' << tem << ' '
                                  << s[i] << '\n';
                        foundError = true;
                        break;
                    }
                }
                if (foundError)
                    break;
            }
            ifs.close();
        }
    }
    delete[] s;
}

void writeRandInt(int n, test_t max) {
    srand(time(NULL));
    std::ofstream ofs("test.txt");
    for (int i = 0; i < n; i++)
        ofs << rand() % max << ' ';
//        ofs << (n - i) << ' ';
    ofs.close();
}

int readRan(test_t *s) {
    int len = -1;
    std::ifstream ifs("test.txt");
    while (ifs >> s[++len]) {
    }
    ifs.close();
    return len;
}
