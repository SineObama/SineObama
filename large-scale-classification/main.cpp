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
#define FORMAT "f"

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
    cout << "Usage 1: -train training_set_file model_file iterations [start_line] [length] \n";
    cout << "Usage 2: -predict test_file model_file output_file [start_line] [length]\n";
    exit(1);
}
int readFile(const char *filename, data_t **yx, const int start, const int len);

int main(int argc, char **argv) {
    if (argc < 3)
        outputUsageAndExit();
    readParam(argc, argv);
    if (type == 0)
        outputUsageAndExit();

    static const data_t C = 0.09;
    setbuf(stdout, NULL);

    data_t *bw = new data_t[n + 1];
    memset(bw, 0, sizeof(data_t) * (n + 1));
    data_t **yx = new data_t*[size];
    for (int i = 0; i < size; i++) {
        yx[i] = new data_t[n + 1];
        memset(yx[i], 0, sizeof(data_t) * (n + 1));
    }
    cout << "alloc finish\n";

    int len = readFile(dataFilename, yx, start, size);
    cout << "read file <" << dataFilename << "> finish\n";

    if (type == 1) {

        SVM::train(yx, len, n, C, bw, iterations);
        cout << "<" << iterations << "> iterations train finish\n";

        data_t J = 0;
        for (int j = 0; j < n + 1; j++)
            J += bw[j] * bw[j];
        J /= 2;
        for (int i = 0; i < len; i++) {
            data_t wx = bw[0];
            for (int j = 1; j < n + 1; j++)
                wx += bw[j] * yx[i][j];
            J += C * (1 - yx[i][0] * wx);
        }

        FILE *pFile = fopen(modelFilename, "w");
        fprintf(pFile, "J=%"FORMAT"\n", J);
        for (int i = 0; i < n + 1; i++)
            fprintf(pFile, "%"FORMAT"\n", bw[i]);
        fclose(pFile);
        cout << "model saved in <" << modelFilename << ">\n";

    } else if (type == 2) {

        FILE *pFile = fopen(modelFilename, "r");
        fscanf(pFile, "J=%*"FORMAT"\n");
        for (int i = 0; i < n + 1; i++)
            fscanf(pFile, "%"FORMAT, &bw[i]);
        fclose(pFile);

        data_t *y = new data_t[len];
        for (int i = 0; i < len; i++)
            y[i] = yx[i][0];

        SVM::predict(yx, len, n, bw);
        cout << "predict finish\n";

        int count = 0;
        for (int i = 0; i < len; i++)
            if (y[i] * yx[i][0] > 0)
                count++;
        data_t correct = (float) count / len * 100;
        cout << correct << "% correct\n";
        delete[] y;

//        ss << modelFilename << "__" << dataFilename << "_" << start << "_"
//           << len << "__" << correct << "%.o";
        pFile = fopen(outputFilename, "w");
        fprintf(pFile, "id,label,correctness=%.2"FORMAT"%%\n", correct);
        for (int i = 0; i < len; i++) {
            if (yx[i][0] < -1)
                yx[i][0] = 0;
            else if (yx[i][0] > 1)
                yx[i][0] = 1;
            else
                yx[i][0] = (yx[i][0] + 1) / 2;
            fprintf(pFile, "%d,%"FORMAT"\n", i, yx[i][0]);
        }
        fclose(pFile);
        cout << "result saved in <" << outputFilename << ">\n";

    }

    for (int i = 0; i < size; i++)
        delete[] yx[i];
    delete[] yx;
    delete[] bw;

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
