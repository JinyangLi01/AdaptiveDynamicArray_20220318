/*
 * 225675 dataset size
 * always sleep 500 ms
 * 220K real-world dataset movie dataset 110k awards
 * about 5.5 hours
 * 100K op, 100K dataset
 * danodesize = 50
 *
 */

#include <iostream>
#include <chrono>
#include <algorithm>
#include "../../../DataStructures/DynamicArray.h"
#include "../../../DataStructures/tiered-vector.h"
using namespace std;
using namespace Seq;
#define timeNow() std::chrono::high_resolution_clock::now()
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()


int main() {

    Seq::Tiered<int, LayerItr<LayerEnd, Layer<70, Layer<70, Layer<70>>>>> tv;
    DynamicArray *da;
    int iniNum = 2256750;
    int danodesize = 50;
    int ToInsert = iniNum + 1;

    int *array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i + 1;
    }

    da = NewDynamicArray(array, iniNum, danodesize);
    tv.initialize(array, iniNum);

    TimeVar time1, time2;

    int insert_pos = 100;
    delete[]array;
    array = nullptr;
    int depth = 1, NumDA;
    int timeADA = 0, timeTV=0;
    int num_insert = 100;
    for (int i = 0; i < num_insert; ++i) {
        time1 = timeNow();
        da->Insert(ToInsert, insert_pos);
        time2 = timeNow();
        timeADA += duration(time2-time1);

        time1 = timeNow();
        tv.insert(ToInsert, insert_pos);
        time2 = timeNow();
        timeTV += duration(time2-time1);
        ToInsert ++;
    }

    cout << "da: " << timeADA / num_insert << " tv: " << timeTV / num_insert << endl;

    cout << tv.Query(500) << " " << da->Query(500) << endl;

    return 0;
}

