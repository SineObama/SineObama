/*
 * main.cpp
 *
 *  Created on: 2017年4月18日
 *      Author: Sine
 */

#include "SVM.h"
#include <iostream>
#include <cstdio>

#define n 201  // # features
#define BUFFER_SIZE 1000
#define MODEL_FILE "model.txt"
#define OUTPUT_FILE "result.txt"

using namespace std;

void outputUsageAndExit() {
    cout << "Usage 1: -train training_set_file\n";
    cout << "Usage 2: -predict test_file\n";
    exit(1);
}

void readFile(const char *filename, SVM::data_t **yx) {
    FILE *pFile = fopen(filename, "r");
    int count = -1;
    int index;
    SVM::data_t value;
    int read = 0;
    while (read != -1) {
        read = fscanf(pFile, " %d:%lf", &index, &value);
        if (read == 2)
            yx[count][index] = value;
        else if (read == 1)
            yx[++count][0] = index == 1 ? 1 : -1;
    }
}

int main(int argc, char **argv) {
    if (argc < 3)
        outputUsageAndExit();
    int type = 0;
    if (strcmp(argv[1], "-train") == 0)
        type = 1;
    else if (strcmp(argv[1], "-predict") == 0)
        type = 2;
    if (type == 0)
        outputUsageAndExit();

    int l = type == 1 ? 1866819 : 282796;
    SVM::data_t *bw = new SVM::data_t[n + 1];
    SVM::data_t **yx = new SVM::data_t*[l];
    for (int i = 0; i < l; i++) {
        yx[i] = new SVM::data_t[n + 1];
        memset(yx[i], 0, sizeof(SVM::data_t) * (n + 1));
    }

    readFile(argv[2], yx);

    if (type == 1) {
        memset(bw, 0, sizeof(SVM::data_t) * (n + 1));
        SVM::train(yx, l, n, bw);
        FILE *pFile = fopen(MODEL_FILE, "w");
        for (int i = 0; i < n + 1; i++)
            fprintf(pFile, "%lf\n", bw[i]);
        fclose(pFile);
    } else {
        FILE *pFile = fopen(MODEL_FILE, "r");
        for (int i = 0; i < n + 1; i++)
            fscanf(pFile, " %lf", &bw[i]);
        fclose(pFile);
        SVM::predict(yx, l, n, bw);
        pFile = fopen(OUTPUT_FILE, "w");
        fprintf(pFile, "id,label\n");
        for (int i = 0; i < l; i++)
            fprintf(pFile, "%d,%lf\n", i, yx[i][0]);
//            fprintf(pFile, "%d,%lf\n", i, (yx[i][0] + 1) / 2);  // todo 变换吗
        fclose(pFile);
    }

    for (int i = 0; i < l; i++)
        delete[] yx[i];
    delete[] yx;
    delete[] bw;

}
