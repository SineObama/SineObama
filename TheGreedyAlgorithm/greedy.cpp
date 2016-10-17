/*
 * greedy.cpp
 *
 *  Created on: 2016年10月17日
 *      Author: Sine
 */

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "View.h"

using namespace std;

int main(int argc, char **argv) {

    // 解析参数
    int k = 0;
    string inputFile;
    for (int i = 0; i < argc; i++) {
        if (string(argv[i]) == "-f") {
            inputFile = argv[i + 1];
        } else if (string(argv[i]) == "-k") {
            stringstream ss;
            ss << argv[i + 1];
            ss >> k;
            if (ss.fail())
                throw exception();
        }
    }

    // 从参数指定的文件读入数据
    ifstream ifs(inputFile.c_str());
    int n;  // view的数目
    ifs >> n;
    View *views = new View[n], *top = NULL;
    int maxCost = 0;  // 假定最大消耗的是top view，并假设数据中top view不一定在第一个
    for (int i = 0; i < n; i++) {
        ifs >> views[i].id >> views[i].cost;
        if (maxCost < views[i].cost) {
            maxCost = views[i].cost;
            top = &views[i];
        }
    }
    int m;  // 边的数目
    ifs >> m;
    for (int i = 0; i < m; i++) {
        int src, dst;
        ifs >> src >> dst;
        views[src - 1].childs.insert(&views[dst - 1]);
    }

    // 算法开始
    int *output = new int[k + 1], count = 0;  // 输出的id及个数
    output[count++] = top->id;  // 默认加入top view
    top->choosed = true;
    // 设置每个view的“当前消耗”
    for (int i = 0; i < n; i++) {
        views[i].curCost = maxCost;
    }
    // k次循环寻找k个view
    for (int i = 0; i < k; i++) {
        int maxB = 0;
        View *choosed = NULL;  //
        for (int j = 0; j < n; j++) {
            if (views[j].choosed)
                continue;
            int B = views[j].getB(views[j].cost);  // get the B(v, S)
//            cout << i + 1 << " " << views[j].id << " " << B << "\n";
            if (maxB < B) {
                maxB = B;
                choosed = &views[j];
            }
            views[j].clearCounted();
        }
        output[count++] = choosed->id;
        choosed->choosed = true;
        choosed->refreshCost(choosed->cost);
    }

    // 输出
    cout << output[0];
    for (int i = 1; i <= k; i++) {
        cout << " -> " << output[i];
    }
    cout << "\n";

    // 现在才来释放内存
    delete[] output;
    delete[] views;
    return 0;

}
