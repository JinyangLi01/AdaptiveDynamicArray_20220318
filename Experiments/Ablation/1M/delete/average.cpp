#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <sstream> // std::stringstream
#include <string>
#include <algorithm>
#include "../../../../DataStructures/DAnolimit.h"
#include "../../../../DataStructures/DAstatic.h"
#include "../../../../DataStructures/DynamicArray.h"
using namespace std;

long long Tda[100], Tdanl[100], Tst[100];
string proportion[100];
int colNum = 0;
string title ;
void readCSV(int i, char * name) {
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
            title = line;
            idx = 0;
            continue;
        }
        stringstream sin(line);
        string word;
        int k = 0;
        while (getline(sin, word, ','))
        {
            switch (k) {
                case 0:
                    if (i == 1) {
                        proportion[idx] = word;
                    }
                    break;
                case 1:
                    Tda[idx] += stoi(word);
                    break;
                case 2:
                    Tdanl[idx] += stoi(word);
                    break;
                case 3:
                    Tst[idx] += stoi(word);
                    break;
                default:
                    break;
            }
            k++;
        }
        idx ++;
    }
    colNum = idx;
}


int main(int argc, char** argv) {
    for (int i = 1; i <= 10; ++i) {
        readCSV(i, argv[1]);
    }
    ofstream fout;
    fout.open("avg.csv", ios::out | ios::in | ios::trunc);

    for (int j = 0; j < colNum; ++j) {
        Tda[j] /= 10;
        Tst[j] /= 10;
        Tdanl[j] /= 10;
        fout << proportion[j]<<","<<Tda[j]<<","<<Tdanl[j]<<","<<Tst[j]<<endl;
    }
    return 0;
}