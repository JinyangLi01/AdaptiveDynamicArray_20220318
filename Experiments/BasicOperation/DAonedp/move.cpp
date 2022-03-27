/*
 * # items = 100,000
 * # operations = 100,000
 *
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include "../../../DA_Bidirectional/DAbidirectional.h"
#include "../../../DataStructures/DynamicArray.h"
using namespace std;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
bool CompareArray(const int *a, const int *b, int len) {
    if (a == nullptr || b == nullptr) {
        cout<<"null pointer error!" << endl;
        return 0 ;
    }
    for (int i = 0; i < len; ++i) {
        if (a[i] != b[i]) {
            return false;
        }
    }
    return true;
}
// A function to return a seeded random number generator.
inline std::mt19937& generator() {
    // the generator will only be seeded once (per thread) since it's static
    static thread_local std::mt19937 gen(std::random_device{}());
    return gen;
}

// A function to generate integers in the range [min, max]
int RandomInt(int min, int max) {
    //  std::uniform_int_distribution<int> dist(min, max);
    //  return dist(generator());
    return (rand() % (max-min+1))+ min;
}


int* RangeDistributionRandom( int num, int min, int max) {
    int * n = new int[num];
    for (int j = 0; j < num; ++j) {
        n[j] = RandomInt(min, max);
    }
    shuffle(n, n + num, generator());
    return n;
}

int main() {
    //var time1, time2 time.Time
    string filepath[3] = {"DAonedpmove.csv", "DAonedpmoveLog.txt"};
    ofstream finstant, flog, ffinal;
    finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
    flog.open(filepath[1], ios::out | ios::in | ios::trunc);
    //ffinal.open(filepath[2], ios::out | ios::in | ios::trunc);
    int TotalActions = 60000;
    int iniNum = 5000;
    int danodesize = 100;
    printf("# of operations = %d\n", TotalActions);


    TimeVar time1, time2;

    int NowTotalNum = iniNum;
    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }
    DAbidirectional *da = NewDynamicArray(array, iniNum, danodesize);
    DynamicArray * daonedp = NewDAonedp(array, iniNum, danodesize);
    delete []array;
    cout<<"da depth = "<<da->Depth() << endl;

    for (int lt = 0; lt < TotalActions; lt++) {
        printf("lt=%d\n", lt);
        int a[3] = {};
        for (int & j : a) {
            j = RandomInt(1, NowTotalNum);
        }
        sort(a, a+3);
        if (a[1] == a[2]) {
            continue;
        }
        int start1, end1, start2;
        if (RandomInt(1, 10) % 2 == 1) {
            start1 = a[0];
            end1 = a[1];
            start2 = a[2];
        } else {
            start1 = a[1];
            end1 = a[2];
            start2 = a[0];
        }
        da->Move(start1, end1, start2);
        daonedp->Move(start1, end1, start2);
    }
    da->PrintTree();
    daonedp->PrintTree();
    for (int lt = 0; lt < 1; lt++) {
        int a[3] = {};
        for (int & j : a) {
            j = RandomInt(1, NowTotalNum);
        }
        sort(a, a+3);
        if (a[1] == a[2]) {
            continue;
        }
        int start1, end1, start2;

        if (RandomInt(1, 10) % 2 == 1) {
            start1 = a[0];
            end1 = a[1];
            start2 = a[2];
        } else {
            start1 = a[1];
            end1 = a[2];
            start2 = a[0];
        }
        da->Move(start1, end1, start2);
        daonedp->Move(start1, end1, start2);
    }
    da->PrintTree();
    daonedp->PrintTree();
    flog<<"da depth = "<<da->Depth() << endl;

    delete da;
    delete daonedp;
    flog.close();
    finstant.close();
    return 0;
}

