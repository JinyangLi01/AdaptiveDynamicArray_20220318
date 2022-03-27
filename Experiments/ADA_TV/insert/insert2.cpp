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
#include "../../../DataStructures/CountedBtree.h"
#include "../../../DataStructures/StandardArray.h"
#include "../../../DataStructures/DynamicArray.h"
#include "../../../DataStructures/LinkedList.h"
#include "../../../DataStructures/tiered-vector.h"
using namespace std;
using namespace Seq;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
bool CompareArray(const int *a, const int *b, int len) {
    if (a == nullptr || b == nullptr) {
        printf("null pointer error!\n");
        return false ;
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
    //random_shuffle(&n[0], &n[num]);
    return n;
}
DynamicArray *da ;
StandardArray *sa;
CountedBtree * cbt;
LinkedList * ll;
Seq::Tiered<int, LayerItr<LayerEnd, Layer<50, Layer<50, Layer<50>>>>> tiered;

// void insertToTV(int ToInsert, int pos) {
//     tiered.insert();

// }

int main(int argc, char** argv) {
    string filepath[3];
    filepath[0] = "./insert1.csv";
    filepath[1] = "./insert1Log.txt";
    ofstream finstant, flog, ffinal;
    finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
    flog.open(filepath[1], ios::out | ios::in | ios::trunc);
    cout << filepath[0] << " " << filepath[1] << endl;
    finstant<<" ,DA,SA,LL,CBT,TV"<<endl;

    int iniNum = 225820;
    int danodesize = 50;
    int CbtOrder = 50;
    int m = 100;  //for linked listf
    int operations = 100;
    int InsertActions = 3;
    int DeleteActions = 3;
    int ReorderActions = 1;
    int SwapActions = 1;
    int TotalActions = DeleteActions + InsertActions + ReorderActions+SwapActions ;
    finstant << "iniNum = " << iniNum <<" da node size = " << danodesize << " cbt order ";
    std::cout << CbtOrder << " m = " << m << endl;
    
    int CurOutputNum = 0;

    int * reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
    int ir = 0;

    TimeVar time1, time2;
    int NowTotalNum = iniNum;
    int ToInsert = iniNum + 1;
    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, iniNum, danodesize);
    StandardArray *sa = NewStandardArray(array, iniNum);
    CountedBtree * cbt = NewCBTreeForArray(CbtOrder, iniNum);
    LinkedList * ll = NewLinkedListForArray(m, array, iniNum);
    // Seq::Tiered<int, LayerItr<LayerEnd, Layer<50, Layer<50, Layer<50>>>>> tiered;
    tiered.initialize(array, iniNum);
    delete []array;
    int NumSA;
    long long TinsertSA = 0, TinsertDA = 0, TinsertLL = 0, TinsertCBT = 0, TinsertTV = 0;
    long long TdeleteSA = 0, TdeleteDA = 0, TdeleteLL = 0, TdeleteCBT = 0, TdeleteTV = 0;

    int pos = 10000;
    for (int i = 0; i < InsertActions; i++)
    {
        time1 = timeNow();
        sa->Insert(ToInsert, pos);
        time2 = timeNow();
        TinsertSA += duration(time2-time1);

        time1 = timeNow();
        da->Insert(ToInsert, pos);
        time2 = timeNow();
        TinsertDA += duration(time2-time1);
        cout << "da insert time = " << duration(time2-time1);

        time1 = timeNow();
        ll->Insert(ToInsert, pos);
        time2 = timeNow();
        TinsertLL += duration(time2 - time1);

        time1 = timeNow();
        cbt->Insert(ToInsert, pos);
        time2 = timeNow();
        TinsertCBT += duration(time2 - time1);

        time1 = timeNow();
        tiered.insert( pos, ToInsert);
        time2 = timeNow();
        cout << "tv insert time = " << duration(time2-time1) << endl;
        TinsertTV += duration(time2 - time1);

        int insertedID = tiered.Query(pos);
        // printf("query tv after insert, inserted ID = %d, query result = %d\n", ToInsert, insertedID);

        pos ++ ;
        ToInsert++;
        NowTotalNum++;
    }

    pos = 30000;
    for (int i = 0; i < DeleteActions; i++)
    {
        cout << "delete pos = " << pos << endl;
        time1 = timeNow();
        sa->Delete(pos);
        time2 = timeNow();
        TdeleteSA += duration(time2-time1);

        time1 = timeNow();
        da->Delete(pos);
        time2 = timeNow();
        TdeleteDA += duration(time2-time1);
        cout << "da delete time = " << duration(time2-time1);

        time1 = timeNow();
        ll->Delete(pos);
        time2 = timeNow();
        TdeleteLL += duration(time2 - time1);

        time1 = timeNow();
        cbt->Delete(pos);
        time2 = timeNow();
        TdeleteCBT += duration(time2 - time1);

        time1 = timeNow();
        tiered.remove(pos);
        time2 = timeNow();
        TdeleteTV += duration(time2 - time1);
        cout << "tv delete time = " << duration(time2-time1) << endl;
        NowTotalNum--;
        pos ;
    }

    int len = reorderrange[ir];
    ir++;
    if (len >= NowTotalNum) {
        len = NowTotalNum-1;
    }
    int start = RandomInt(1, NowTotalNum - len);
    int end = start + len - 1;
    if (end >= NowTotalNum) {
        end = NowTotalNum - 1;
    }
    int * oldArray = sa->RangeQuery(start, end, &NumSA);
    int * newArray = new int[len];
    for (int j = 0; j < len; ++j) {
        newArray[j] = oldArray[len-j-1];
        //cout<<newArray[j]<<" ";
    }
    // cout<<endl;

    sa->Reorder(start, end, newArray);
    da->Reorder(start, end, newArray);
    cbt->Reorder(start, end, newArray);
    ll->Reorder(start, end, newArray);
    tiered.Reorder(start, end, newArray);
    delete []newArray;
    delete []oldArray;
    

    int a[4] = {};
    a[0] = pos;
    a[1] = a[0] + 200;
    a[2] = a[0] + 300;
    a[3] = a[0] + 480;
    int start1 = a[0];
    int end1 = a[1];
    int start2 = a[2];
    int end2 = a[3];
    sa->Swap(start1, end1, start2, end2);
    da->Swap(start1, end1, start2, end2);
    cbt->Swap(start1, end1, start2, end2);
    tiered.Swap(start1, end1, start2, end2);
    ll->Swap(start1, end1, start2, end2);



    finstant <<TinsertDA/InsertActions << ","<< TinsertSA/InsertActions<<","<<TinsertLL/InsertActions
             <<","<<TinsertCBT/InsertActions<<","<<TinsertTV/InsertActions<<endl;
    cout <<TinsertDA/InsertActions << ","<< TinsertSA/InsertActions<<","<<TinsertLL/InsertActions
         <<","<<TinsertCBT/InsertActions<<","<<TinsertTV/InsertActions<<endl;


    finstant <<TdeleteDA/DeleteActions << ","<< TdeleteSA/DeleteActions<<","<<TdeleteLL/DeleteActions
             <<","<<TdeleteCBT/DeleteActions<<","<<TdeleteTV/DeleteActions<<endl;
    cout <<TdeleteDA/DeleteActions << ","<< TdeleteSA/DeleteActions<<","<<TdeleteLL/DeleteActions
         <<","<<TdeleteCBT/DeleteActions<<","<<TdeleteTV/DeleteActions<<endl;

    //cout<<"da depth = "<<da->Depth() << endl;
    flog<<"da depth = "<<da->Depth() << endl;
    delete []reorderrange;
    flog.close();
    finstant.close();
    return 0;
}

