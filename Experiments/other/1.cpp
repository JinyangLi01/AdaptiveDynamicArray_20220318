#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include "../../DataStructures/StandardArray.h"
#include "../../DA_Bidirectional/DAbidirectional.h"
#include "../../DataStructures/CountedBtree.h"
#include "../../DataStructures/LinkedList.h"
using namespace std;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

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

int main() {
    //var time1, time2 time.Time
    string filepath[3] = {"1.csv", "1.txt"};
    ofstream finstant, flog, ffinal;
    finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
    flog.open(filepath[1], ios::out | ios::in | ios::trunc);
    //ffinal.open(filepath[2], ios::out | ios::in | ios::trunc);
    finstant<<" ,SA,DA,LL,CBT"<<endl;

    int iniNum = 1000; //0.03 = 30000
    int danodesize = 10;
    int CbtOrder = 10;
    int m = 10;  //for linked list
    int NumOfOutput = 10;
    int operations = 1000;
    int repeat = 100;
    int InsertActions = operations * 2 / 10;
    int DeleteActions = operations * 2 / 10;
    int ReorderActions = operations * 2 / 10;
    int SwapActions = operations * 2 / 10;
    int MoveActions = operations * 2 / 10;
    int TotalActions = DeleteActions + InsertActions + ReorderActions+SwapActions + MoveActions; // 550 000
    int ComputeInterval = TotalActions / NumOfOutput; // 10%
    int Interval100 = TotalActions / 100;
    printf("%d, %d, %d, %d, %d\n", InsertActions, DeleteActions, ReorderActions, SwapActions, MoveActions);
    printf("%d\n", TotalActions);
    int CurOutputNum = 0;

    int *a = new int[TotalActions];
    int ua = 0;
    for (int y = 0; y < InsertActions; y++) {
        a[ua] = 2;
        ua++;
    }
    for (int y = 0; y < DeleteActions; y++) {
        a[ua] = 3;
        ua++;
    }
    for (int y = 0; y < ReorderActions; y++) {
        a[ua] = 5; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
    shuffle(a, a + TotalActions, generator());

    int * shortqueryrange = RangeDistributionRandom(repeat, 1, 100);
    int * reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
    int ir = 0;

    TimeVar time1, time2;


    int NowTotalNum = iniNum;
    int ToInsert = iniNum + 1;
    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }
    DAbidirectional *da = NewDynamicArray(array, iniNum, danodesize);
    StandardArray *sa = NewStandardArray(array, iniNum);
    //CountedBtree * cbt = NewCBTreeForArray(CbtOrder, iniNum);
    LinkedList * ll = NewLinkedListForArray(m, array, iniNum);
    int NumSA, NumDA, Numll, NumCBT;
    //da->PrintTree();
    cout<<"da depth = "<<da->Depth() << endl;
    for (int lt = 0; lt < operations; lt++) {
        //cout<<"lt = "<<lt;
        //cout<<"  da depth = "<<da->Depth() << endl;
        //da->PrintTree();

        if (lt % 500 == 0) {
            cout<<"lt = "<<lt<<endl;
            cout<<"da depth = "<<da->Depth() << endl;
            flog<<"lt = "<<lt<<endl;
            flog<<"da depth = "<<da->Depth() << endl;
        }

        switch (a[lt]) {
            case 2: //insert
            {
                int pos = RandomInt(1, NowTotalNum);
                sa->Insert(ToInsert, pos);
                da->Insert(ToInsert, pos);
               // cbt->Insert(ToInsert, pos);
                ll->Insert(ToInsert, pos);
                ToInsert++;
                NowTotalNum++;
                break;
            }
            case 3: //delete
            {
                int pos = RandomInt(1, NowTotalNum);
                sa->Delete(pos);
                da->Delete(pos);
             //   cbt->Delete(pos);
                ll->Delete(pos);
                NowTotalNum--;
                break;
            }
            case 4: //reorder
            {
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
                }
                sa->Reorder(start, end, newArray);
                da->Reorder(start, end, newArray);
              //  cbt->Reorder(start, end, newArray);
                ll->Reorder(start, end, newArray);
                delete []newArray;
                delete []oldArray;
                break;
            }
            case 5: //swap
            {
                int a[4] = {};
                for (int & j : a) {
                    j = RandomInt(1, NowTotalNum);
                }
                sort(a, a+4);
                if (a[1] == a[2]) {
                    continue;
                }
                int start1 = a[0];
                int end1 = a[1];
                int start2 = a[2];
                int end2 = a[3];
                sa->Swap(start1, end1, start2, end2);
                da->Swap(start1, end1, start2, end2);
              //  cbt->Swap(start1, end1, start2, end2);
                ll->Swap(start1, end1, start2, end2);
                break;
            }
            case 6: //move
            {
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
                sa->Move(start1, end1, start2);
                da->Move(start1, end1, start2);
              //  cbt->Move(start1, end1, start2);
                ll->Move(start1, end1, start2);
                break;
            }
        }
        if ( (lt+1 <= 10) || ((lt+1<=100)) || ((lt+1<=1000) && ((lt+1)%100 == 0))
             || ((lt+1<=10000) && ((lt+1)%1000 == 0)) || ((lt+1<=100000) && ((lt+1)%10000 == 0))
             || ((lt+1<=1000000) && ((lt+1)%100000 == 0))) {
            long long Tsa = 0, Tda = 0, Tll = 0, Tcbt = 0;
            CurOutputNum ++;
            double fl = (lt+1)*1.0/TotalActions ;
            finstant << fl << ",";
            int start1 = 30, end1 = 1000, start2 = 4500, end2 = 8200;
            for (int i = 0; i < 1; ++i) {

                int a[4] = {};
                for (int j = 0; j < 4; ++j) {
                    a[j] = RandomInt(1, NowTotalNum);
                }
                sort(a, a+4);
                if (a[1] == a[2]) {
                    continue;
                }
                int start1 = a[0];
                int end1 = a[1];
                int start2 = a[2];
                int end2 = a[3];


                time1 = timeNow();
                sa->Swap(start1, end1, start2, end2);
                time2 = timeNow();
                Tsa += duration(time2-time1);

                time1 = timeNow();
                da->Swap(start1, end1, start2, end2);
                time2 = timeNow();
                Tda += duration(time2-time1);

                time1 = timeNow();
                ll->Swap(start1, end1, start2, end2);
                time2 = timeNow();
                Tll += duration(time2 - time1);

                time1 = timeNow();
              //  cbt->Swap(start1, end1, start2, end2);
                time2 = timeNow();
                Tcbt += duration(time2-time1);
            }
            finstant << Tsa/repeat<<","<<Tda/repeat << ","<<Tll/repeat<<","<<Tcbt/repeat<<endl;
            //cout<<"after swapping, lt = "<<lt<<" da depth = "<<da->Depth()<<endl;
            //da->PrintTree();
        }

    } //for lt <= loopTime
    cout<<"da depth = "<<da->Depth() << endl;
    da->PrintTree();
    flog<<"da depth = "<<da->Depth() << endl;
    delete []shortqueryrange;
    delete []reorderrange;
    delete []a;
    delete ll, da, sa;
    flog.close();
    finstant.close();
    return 0;
}

