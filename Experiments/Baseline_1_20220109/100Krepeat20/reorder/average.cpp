#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <sstream> // std::stringstream
#include <string>
#include <algorithm>
using namespace std;

long long T[100][10];
string proportion[100];
int colNum = 0, rowNum = 0;
string title ;

void readColNumTitle(int i, const char * name) {
    fstream fi;
    string s = name;
    s += std::to_string(i);
    s += ".csv";
    fi.open(s, ios::in);
    string line;
    getline(fi, line);
    title = line;
    stringstream sin(line);
    string word;
    int k = 0;
    while (getline(sin, word, ',')) {
        k++;
    }
    colNum = k;
}

void readCSV(int i, const char * name) {
    fstream fi;
    string s = name;
    s += std::to_string(i);
    s += ".csv";
    fi.open(s, ios::in);
    string line;
    int idx = -1;
    while (getline(fi, line))
    {
        if (idx == -1) {
            idx = 0;
            continue;
        }
        stringstream sin(line);
        string word;
        int k = 0;
        while (getline(sin, word, ','))
        {
            if (k == 0) {
                if (i == 1) {
                    proportion[idx] = word;
                }
            } else {
                T[idx][k] += stoi(word);
            }
            k++;
        }
        idx ++;
    }
    rowNum = idx;
    fi.close();
}


int main(int argc, char** argv) {
    readColNumTitle(1, argv[1]);
    int average_out_of = 20;
    for (int i = 1; i <= average_out_of; ++i) {
        readCSV(i, argv[1]);
    }
    ofstream fout;
    fout.open("avg.csv", ios::out | ios::in | ios::trunc);
    fout << title << endl;
    for (int k = 0; k < rowNum; ++k) {
        fout<<proportion[k]<<",";
        for (int j = 1; j < colNum; ++j) {
            T[k][j] /= average_out_of;
            fout<<T[k][j]<<",";
        }
        fout <<endl;
    }
    fout.close();
    return 0;
}
