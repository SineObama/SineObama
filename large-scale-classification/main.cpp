/*
 * main.cpp
 *
 *  Created on: 2017年4月18日
 *      Author: Sine
 */

#include "SVM.h"
#include <iostream>
#include <cstdio>
#include <cstring>
#include <sstream>

#define n 201  // # features
#define BUFFER_SIZE 1000

typedef SVM::data_t data_t;

using namespace std;

int type = 0;
char *dataFilename = NULL;
char *modelFilename = NULL;
char *outputFilename = NULL;
int start = 1;  // start line
int size = 0;  // alloc length
int iterations = 1000;

void readParam(int argc, char **argv);
void outputUsageAndExit() {
    cout << "Usage 1: -train    train_file  model_file  iterations   [start_line]    [length]\n";
    cout << "Usage 2: -predict  test_file   model_file  output_file  [start_line]    [length]\n";
    exit(1);
}
int readFile(const char *filename, data_t **yx, const int start, const int len);

int main(int argc, char **argv) {
    if (argc < 3)
        outputUsageAndExit();
    readParam(argc, argv);
    if (type == 0)
        outputUsageAndExit();

    static const data_t C = 0.5;
    setbuf(stdout, NULL);

    data_t **yx = new data_t*[size];
    for (int i = 0; i < size; i++) {
        yx[i] = new data_t[n + 1];
        memset(yx[i], 0, sizeof(data_t) * (n + 1));
    }
    cout << "alloc finish\n";

    int len = readFile(dataFilename, yx, start, size);
    cout << "read file <" << dataFilename << "> finish\n";

    if (type == 1) {

        SVM svm(n);
        svm.train(yx, len, C, iterations, modelFilename);
        cout << "<" << iterations << "> iterations train finish\n";
        cout << "model saved in <" << modelFilename << ">\n";

    } else if (type == 2) {

        SVM svm(modelFilename);
        svm.predict(yx, len, outputFilename);
        cout << "predict finish\n";
        cout << "result saved in <" << outputFilename << ">\n";
    }

    for (int i = 0; i < size; i++)
        delete[] yx[i];
    delete[] yx;

}

void readParam(int argc, char **argv) {
    if (strcmp(argv[1], "-train") == 0)
        type = 1;
    else if (strcmp(argv[1], "-predict") == 0)
        type = 2;
    else
        return;
    dataFilename = argv[2];
    modelFilename = argv[3];
    stringstream ss;
    if (type == 1) {
        ss << argv[4];
        ss >> iterations;
        ss.str("");
        ss.clear();
    } else {
        outputFilename = argv[4];
    }
    size = type == 1 ? 1866819 : 282796;  // 训练集和测试集的样本数
    if (argc > 5) {
        ss << argv[5];
        ss >> start;
        ss.str("");
        ss.clear();
    }
    if (argc > 6) {
        ss << argv[6];
        ss >> size;
        ss.str("");
        ss.clear();
    }
}

/*
 * read <len> lines from the data file's <start>'th line
 */
int readFile(const char *filename, data_t **yx, const int start,
             const int len) {
    const int skip = start - 1;
    FILE *pFile = fopen(filename, "r");
    int count = -1;
    int index = 0;
    int read = 0;
    while (read != -1) {
        read = fscanf(pFile, " %d:%*"FORMAT, &index);
        if (read == 1)
            if (++count >= skip)
                break;
    }
    if (read != -1)
        yx[0][0] = index;
    data_t value;
    count = 0;
    while (true) {
        read = fscanf(pFile, " %d:%"FORMAT, &index, &value);
        if (read == 2)
            yx[count][index] = value;
        else if (read == 1) {
            if (++count >= len)
                break;
            yx[count][0] = index == 1 ? 1 : -1;
        } else if (read == -1) {
            count++;
            break;
        }
    }
    fclose(pFile);
    return count;
}
