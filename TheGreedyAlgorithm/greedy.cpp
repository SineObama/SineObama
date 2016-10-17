/*
 * greedy.cpp
 *
 *  Created on: 2016��10��17��
 *      Author: Sine
 */

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include "View.h"

using namespace std;

int main(int argc, char **argv) {

    // ��������
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

    // �Ӳ���ָ�����ļ���������
    ifstream ifs(inputFile.c_str());
    int n;  // view����Ŀ
    ifs >> n;
    View *views = new View[n], *top = NULL;
    int maxCost = 0;  // �ٶ�������ĵ���top view��������������top view��һ���ڵ�һ��
    for (int i = 0; i < n; i++) {
        ifs >> views[i].id >> views[i].cost;
        if (maxCost < views[i].cost) {
            maxCost = views[i].cost;
            top = &views[i];
        }
    }
    int m;  // �ߵ���Ŀ
    ifs >> m;
    for (int i = 0; i < m; i++) {
        int src, dst;
        ifs >> src >> dst;
        views[src - 1].childs.insert(&views[dst - 1]);
    }

    // �㷨��ʼ
    int *output = new int[k + 1], count = 0;  // �����id������
    output[count++] = top->id;  // Ĭ�ϼ���top view
    top->choosed = true;
    // ����ÿ��view�ġ���ǰ���ġ�
    for (int i = 0; i < n; i++) {
        views[i].curCost = maxCost;
    }
    // k��ѭ��Ѱ��k��view
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

    // ���
    cout << output[0];
    for (int i = 1; i <= k; i++) {
        cout << " -> " << output[i];
    }
    cout << "\n";

    // ���ڲ����ͷ��ڴ�
    delete[] output;
    delete[] views;
    return 0;

}
