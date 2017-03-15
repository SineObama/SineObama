/*
 * main.cpp
 *
 *  Created on: 2017Äê3ÔÂ14ÈÕ
 *      Author: Sine
 */

#include <cstdio>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <string>
#include <sstream>
#include "Machine.h"

#define m 25000
#define n 384
#define buffer_size 10000
#define cache_filename "temp.csv"
#define postfix ".csv"

typedef double data_t;

using namespace std;

void readcsv(const char *filename, data_t **data, int row, int column) {
    FILE *pFile = fopen(filename, "r");
    fscanf(pFile, "%*s");
    for (int j = 0; j < n + 1; j++)
        fscanf(pFile, ",%*s");
    for (int i = 0; i < m; i++) {
        fscanf(pFile, " %*d");
        for (int j = 0; j < n + 1; j++)
            fscanf(pFile, ",%lf", &data[i][j]);
    }
    fclose(pFile);
}

int main(int argc, char **argv) {
    if (argc != 4)
        return -1;

    setbuf(stdout, NULL);

    data_t **train = new data_t*[m];
    data_t **test = new data_t*[m];
    for (int i = 0; i < m; i++) {
        train[i] = new data_t[n + 1];
        test[i] = new data_t[n];
    }

    cout << "start" << endl;

    // read training set
    readcsv(argv[1], train, m, n);
    cout << "finish reading train" << endl;

    // read test set
    readcsv(argv[2], test, m, n);
    cout << "finish reading test" << endl;

    // machine learning
    Machine machine(n);
    double alpha = 1;
    const int times = 100;
    double scale = 1.1;
    for (int i = 0; i < 100; i++) {
        alpha = machine.learn(train, m, alpha, times, scale);
        cout << "finish training" << endl;

        // guessing and writing
        FILE *pFile = fopen(cache_filename, "w");
        fprintf(pFile, "Id,reference\n");
        for (int j = 0; j < m; j++)
            fprintf(pFile, "%d,%lf\n", j, machine.guess(test[j]));
        fclose(pFile);
        stringstream ss;
        ss << (string("move ") + cache_filename + " " + argv[3]) << (i + 1)
           << "x" << times << "x" << scale << postfix;
        system(ss.str().c_str());
        cout << "finish writing " << (i + 1) << endl;
    }

    cout << "finish" << endl;

    for (int i = 0; i < m; i++) {
        delete[] test[i];
        delete[] train[i];
    }
    delete[] test;
    delete[] train;
}

//    ifstream ifs(argv[1]);
//    ifs.getline(buffer, buffer_size);
//    for (int i = 0; i < m; i++) {
//        ifs.getline(buffer, buffer_size);
//        stringstream ss;
//        ss << buffer;
//        ss >> unused;
//        for (int j = 0; j < n + 1; j++) {
//            ss >> c >> train[i][j];
//        }
//    }
