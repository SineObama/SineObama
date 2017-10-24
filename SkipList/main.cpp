/*
 * main.cpp
 *
 *  Created on: 2017年10月22日
 *      Author: Sine
 */

#include <cstdio>
#include <cstdlib>
#include <cstddef>
#include <ctime>
#include "SkipList.h"
#include "helpers.h"
#include "Counter.h"
#include "skip.h"
#include "TestDataGenerator.h"
#include <map>
#include <set>

typedef Sine::SkipList<int, int> List;

List list;
std::set<int> se;
Sine::Counter counter;
skiplist sk;

void testInsertWithSet(int max) {

    // 乱序自然数插入测试
    Sine::RandomNatural s(max);

    counter.start();
    for (int i = 0; i < max - 1; i++)
        sk.insert_element(s[i]);
    counter.stop();
    printf("sk:%f\n", counter.time());

    counter.start();
    for (int i = 0; i < max; i++)
        se.insert(s[i]);
    counter.stop();
    printf("set:%f\n", counter.time());

    counter.start();
    for (int i = 0; i < max; i++)
        list.insert(std::make_pair(s[i], s[i]));
    counter.stop();
    printf("SkipList:%f\n", counter.time());

    List::iterator it = list.begin();
    std::set<int>::iterator it2 = se.begin();
    while (it != list.end()) {
        if (it->second != *it2) {
            printf("insert wrong %d %d\n", it->second, *it2);
            break;
        }
        it++;
        it2++;
    }
}

void testFind(int time, int max) {
    Sine::RandomInt s(time, 0, max * 3);

    counter.start();
    for (int i = 0; i < time; i++)
        sk.contains(s[i]);
    counter.stop();
    printf("sk:%f\n", counter.time());

    counter.start();
    for (int i = 0; i < time; i++)
        se.find(s[i]);
    counter.stop();
    printf("set:%f\n", counter.time());

    counter.start();
    for (int i = 0; i < time; i++) {
        List::iterator it = list.find(s[i]);
        if ((s[i] < max) ^ (it != list.end()))
            printf("find wrong %d\n", s[i]);
        if (it != list.end() && s[i] != it->second)
            printf("find wrong %d %d\n", it->second, s[i]);
    }
    counter.stop();
    printf("SkipList:%f\n", counter.time());
}

void testRemove(int max) {

    // 乱序自然数删除测试
    Sine::RandomNatural s(max);

    counter.start();
    for (int i = 0; i < max; i++)
        sk.delete_element(s[i]);
    counter.stop();
    printf("sk:%f\n", counter.time());

    counter.start();
    for (int i = 0; i < max; i++)
        se.erase(s[i]);
    counter.stop();
    printf("set:%f\n", counter.time());

    counter.start();
    for (int i = 0; i < max; i++)
        list.remove(s[i]);
    counter.stop();
    printf("SkipList:%f\n", counter.time());

    if (list.begin() != list.end())
        printf("not empty\n");
}

int main() {
    srand(0);
//    srand(time(NULL));
    int max = 100000;
    int find = 100000;
    printf("---insert %d---\n", max);
    testInsertWithSet(max);
    printf("---find %d---\n", find);
    testFind(find, max);
    printf("---remove %d---\n", max);
    testRemove(max);
}
