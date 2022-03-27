#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
#include <random>
#include <algorithm>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
#include "../../DataStructures/CountedBtree.h"
#include "../../DataStructures/StandardArray.h"
#include "../../DataStructures/DynamicArray.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define STRING_SIZE 30

using namespace std;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

bool CompareArray(const int *a, const int *b, int len) {
    if (a == nullptr || b == nullptr) {
        cout<<"null pointer error!" << endl;
        return false;
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
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator());
}

int* RangeDistributionRandom( int num, int min, int max) {
    int * n = new int[num];
    for (int j = 0; j < num; ++j) {
        n[j] = RandomInt(min, max);
    }
    shuffle(n, n + num, generator());
    return n;
}

string RandomString(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    string s;
    for (int i = 0; i < len; ++i) {
        s += alphanum[RandomInt(1, 100000000) % (sizeof(alphanum) - 1)];
    }
    return s;
}
const int operations = 20;
const int iniNum = 100; //0.03 = 30000
const int danodesize = 5;
int TinsertIdx = 0, TdeleteIdx = 0;
long long TinsertDB[operations], TdeleteDB[operations], Tinsert[operations], Tdelete[operations];

vector<atomic_flag*> InsertAvaliable;
vector<atomic_flag*> DeleteAvaliable;
atomic_int totalInsert, totalDelete;

void insertToDB(int ToInsert, int insertindex, int threadIdx) {
    TimeVar time1 = timeNow();

    bool find = false;
    int i = 0;
    for (; i < totalInsert; ++i) {
        if ((*(InsertAvaliable[i])).test_and_set()) { // already set
          //  printf("thread %d i=%d already set\n", threadIdx, i);
            continue;
        } else { // new set
            find = true;
            printf("thread %d, find insert stmt %d\n", threadIdx, i);
            break;
        }
    }

    if (!find) {
        atomic_flag af;
        af.test_and_set();
        InsertAvaliable.push_back(&af);
        i = totalInsert.operator int();
        totalInsert.operator++();
        printf("thread %d, create new insert stmt %d\n", threadIdx, i+1);
    }
    //std::this_thread::sleep_for(std::chrono::seconds(1));
    TimeVar time2 = timeNow();
    long long t = duration(time2 - time1);
    if (insertindex >= 0) {
        TinsertDB[insertindex] = t;
    }
    (*(InsertAvaliable[i])).clear();
}

void deleteFromDB(int IDtoDelete, int deleteindex, int threadIdx) {
    TimeVar time1 = timeNow();
    bool find = false;
    int i = 0;
    for (; i < totalDelete; ++i) {
        if ((*DeleteAvaliable[i]).test_and_set()) { // already set
           // printf("thread %d i=%d already set\n", threadIdx, i);
            continue;
        } else { // new set
            find = true;
            printf("thread %d, find delete stmt %d\n", threadIdx, i);
            break;
        }
    }
    if (!find) {
        atomic_flag af = ATOMIC_FLAG_INIT;
        af.test_and_set();
        DeleteAvaliable.push_back(&af);
        i = totalDelete.operator int();
        totalDelete.operator++();
        printf("thread %d, create new delete stmt %d\n", threadIdx, i+1);
    }
    std::this_thread::sleep_for(std::chrono::seconds(1));

    TimeVar time2 = timeNow();
    long long t = duration(time2 - time1);
    if (deleteindex >= 0) {
        TdeleteDB[deleteindex] = t;
    }
    (*(DeleteAvaliable[i])).clear();
}

int main() {

        totalInsert.operator=(8);
        totalDelete.operator=(8);
        int threadIdx = 1;


        for (int l = 0; l < totalDelete; ++l) {
            auto * ai = new atomic_flag; (*ai).clear();
            auto * ad = new atomic_flag; (*ad).clear();
            InsertAvaliable.push_back(ai);
            DeleteAvaliable.push_back(ad);
        }
        /*
        printf("size insertavaliable = %lu\n", InsertAvaliable.size());
    for (int j = 0; j < totalDelete; ++j) {
        if ((*(InsertAvaliable[j])).test_and_set()) {
            printf("%d already set\n", j);
        } else {
            printf("%d unset\n", j);
        }
    }
         */

        //var time1, time2 time.Time
        string filepath[3] = {"asyn.csv", "asynLog.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);
        finstant<<" ,Tinsert,TinsertDB+Tinsert,Tdelete,TdeleteDB+Tdelete"<<endl;


        int InsertActions = operations * 2 / 10;
        int DeleteActions = operations * 2 / 10;
        int ReorderActions = operations * 2 / 10;
        int SwapActions = operations * 2 / 10;
        int MoveActions = operations * 2 / 10;
        int TotalActions = DeleteActions + InsertActions + ReorderActions + SwapActions + MoveActions; // 550 000
        printf("%d, %d, %d, %d, %d\n", InsertActions, DeleteActions, ReorderActions, SwapActions, MoveActions);
        printf("# of operations = %d\n", operations);

        int *a = new int[TotalActions];
        int ua = 0;
        for (int y = 0; y < ReorderActions; y++) {
            a[ua] = 4;
            ua++;
        }
        for (int y = 0; y < SwapActions; y++) {
            a[ua] = 5;
            ua++;
        }
        for (int y = 0; y < MoveActions; y++) {
            a[ua] = 6;
            ua++;
        }
        for (int y = 0; y < InsertActions; y++) {
            a[ua] = 2;
            ua++;
        }
        for (int y = 0; y < DeleteActions; y++) {
            a[ua] = 3;
            ua++;
        }

        shuffle(a, a + TotalActions, generator());
        int *reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
        int ir = 0;
        printf("# of operations = %d\n", operations);

        TimeVar time1, time2, time3;

        int NowTotalNum = iniNum;
        int ToInsert = iniNum + 1;
        int *array = new int[iniNum];
        for (int i = 0; i < iniNum; ++i) {
            array[i] = i + 1;
        }

        DynamicArray *da = NewDynamicArray(array, iniNum, danodesize);
        delete[]array;
        array = nullptr;
        int numUpdate = 0;
        int depth = 1, NumDA;
        for (int lt = 0; lt < operations; lt++) {

                time1 = timeNow();
                int pos = RandomInt(1, NowTotalNum);
                da->Insert(ToInsert, pos);
                ToInsert++;
                NowTotalNum++;
                thread th1(insertToDB, ToInsert, TinsertIdx, threadIdx);
                th1.detach();
                threadIdx++;
                time2 = timeNow();
                Tinsert[TinsertIdx] = duration(time2 - time1);
                TinsertIdx++;


                time1 = timeNow();
                pos = RandomInt(1, NowTotalNum);
                int IDtoDelete = da->Query(pos);
                NowTotalNum--;
                da->Delete(pos);
                thread th2(deleteFromDB, IDtoDelete, TdeleteIdx, threadIdx);
                th2.detach();
                threadIdx++;
                time2 = timeNow();
                Tdelete[TdeleteIdx] = duration(time2 - time1);
                TdeleteIdx++;

                numUpdate += 2;

        } //for lt <= loopTime
        int idx = 0;
        for (int lt = 0; lt < TotalActions; lt++) {
            if ((lt + 1 <= 10) || ((lt + 1 <= 100) && ((lt + 1) % 10 == 0)) ||
                ((lt + 1 <= 1000) && ((lt + 1) % 100 == 0))
                || ((lt + 1 <= 10000) && ((lt + 1) % 1000 == 0)) || ((lt + 1 <= 100000) && ((lt + 1) % 10000 == 0))
                || ((lt + 1 <= 1000000) && ((lt + 1) % 100000 == 0))) {
                double fl = (lt + 1) * 1.0 / operations;
                finstant << fl << "," << Tinsert[idx] << ","<<Tinsert[idx]+TinsertDB[idx]
                <<","<<Tdelete[idx] << "," << Tdelete[idx] + TdeleteDB[idx] << endl;

            }
        }
        flog.close();
        finstant.close();

    return 0;
}

