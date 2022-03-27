#include <iostream>
#include <fstream>
#include <chrono>
#include "../../../DataStructures/StandardArray.h"
#include "../../../DA_Bidirectional/DAbidirectional.h"
#include "../../../DataStructures/CountedBtree.h"
#include "../../../DataStructures/LinkedList.h"

using namespace std;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
TimeVar BeginTime;
int RandomInt(int s, int e) {
    TimeVar now = timeNow();
    srand(duration(now - BeginTime));
    return s + rand() % (e-s+1);
}

int* RangeDistributionRandom( int num, int min, int max) {
    srand(time(0));
    int * n = new int[num];
    for (int j = 0; j < num; ++j) {
        n[j] = RandomInt(min, max);
    }
    random_shuffle(&n[0], &n[num]);
    return n;
}

int main() {
    //var time1, time2 time.Time
    string filepath[3] = {"../Experiments/AveBaseline/1M/1_instant.csv", "../Experiments/AveBaseline/1M/1_average.csv", "../Experiments/AveBaseline/1M/1_final.csv"};
    ofstream finstant, faverage, ffinal;
    finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
    faverage.open(filepath[1], ios::out | ios::in | ios::trunc);
    ffinal.open(filepath[2], ios::out | ios::in | ios::trunc);

    int iniNum = 1000000; //0.03 = 30000
    int danodesize = 3500;
    int m = 5000;  //for linked list
    int NumOfOutput = 10;
    int operations = 10000;
    int InsertActions = operations * 4 / 10;
    int  DeleteActions = operations * 1 / 10;
    int  QueryActions = operations * 44 / 100;
    int ReorderActions = operations * 2 / 100;
    int SwapActions = operations * 2 / 100;
    int MoveActions = operations * 2 / 100;
    int LongQueryActions = QueryActions / 2;
    int ShortQueryActions = QueryActions / 2;
    int TotalActions = DeleteActions + InsertActions + QueryActions + ReorderActions+SwapActions + MoveActions; // 550 000
    int ComputeInterval = TotalActions / NumOfOutput;
    printf("%d, %d, %d, %d, %d, %d\n", InsertActions, DeleteActions, QueryActions, ReorderActions, SwapActions, MoveActions);
    printf("%d\n", TotalActions);
    int CurOutputNum = 0;

    int *a = new int[TotalActions];
    int ua = 0;

    for (int y = 0; y < ShortQueryActions; y++) {
        a[ua] = 1;
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
    for (int y = 0; y < LongQueryActions; y++) {
        a[ua] = 7;
        ua++;
    }
    random_shuffle(&a[0], &a[TotalActions]);

    int * shortqueryrange = RangeDistributionRandom(ShortQueryActions, 1, 100);
    int * reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
    int iq = 0;
    int ir = 0;
    int * longqueryrange = RangeDistributionRandom(ShortQueryActions, 100, iniNum);
    int ilq = 0;

    TimeVar time1, time2;
    int Tsa[8] = {0,0,0,0,0,0,0,0};
    int Tda[8] = {0,0,0,0,0,0,0,0};
    int Tll[8] = {0,0,0,0,0,0,0,0};
    int Tcbt[8] = {0,0,0,0,0,0,0,0};

    int Inssa = 0, Insda = 0, Insll = 0, Incbt = 0;


    int NowTotalNum = iniNum;
    int ToInsert = iniNum + 1;
    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, iniNum, 8);
    StandardArray *sa = NewStandardArray(array, iniNum);
    LinkedList * ll = NewLinkedListForArray(m, array, iniNum);
    int NumSA, NumDA, Numll, NumCBT;

    for (int lt = 0; lt < TotalActions; lt++) {
        if (lt % 5000 == 0) {
            cout<<"lt = "<<lt<<endl;
        }
        switch (a[lt]) {
            case 1: //query
            {
                int len = shortqueryrange[iq];
                if (len >= NowTotalNum) {
                    len = NowTotalNum - 1;
                }
                iq++;
                int start = RandomInt(1, NowTotalNum - len);
                int end = start + len - 1;
                if (end >= NowTotalNum) {
                    end = NowTotalNum - 1;
                }

                time1 = timeNow();
                int *AnsSA = sa->RangeQuery(start, end, &NumSA);
                time2 = timeNow();
                int tsa = duration(time2 - time1);
                Tsa[1] += tsa;
                Tsa[0] += tsa;
                Inssa = tsa;

                time1 = timeNow();
                int *AnsDA = da->RangeQuery(start, end, &NumDA);
                time2 = timeNow();
                int tda = duration(time2 - time1);
                Tda[1] += tda;
                Tda[0] += tda;
                Insda = tda;


                time1 = timeNow();
                int *Ansll = ll->RangeQuery(start, end, &Numll);
                time2 = timeNow();
                int tll = duration(time2 - time1);
                Tll[1] += tll;
                Tll[0] += tll;
                Insll = tll;

                break;
            }
            case 7: //query
            {
                int len = longqueryrange[ilq];
                if (len >= NowTotalNum) {
                    len = NowTotalNum - 1;
                }
                ilq++;
                int start = RandomInt(1, NowTotalNum - len);
                int end = start + len - 1;
                if (end >= NowTotalNum) {
                    end = NowTotalNum - 1;
                }

                time1 = timeNow();
                int * AnsSA = sa->RangeQuery(start, end, &NumSA);
                time2 = timeNow();
                int tsa = duration(time2 - time1);
                Tsa[7] += tsa;
                Tsa[0] += tsa;
                Inssa = tsa;

                time1 = timeNow();
                int * AnsDA = da->RangeQuery(start, end, &NumDA);
                time2 = timeNow();
                int tda = duration(time2 - time1);
                Tda[7] += tda;
                Tda[0] += tda;
                Insda = tda;


                time1 = timeNow();
                int * Ansll = ll->RangeQuery(start, end, &Numll);
                time2 = timeNow();
                int tll = duration(time2 - time1);
                Tll[7] += tll;
                Tll[0] += tll;
                Insll = tll;

                break;
            }

            case 2: //insert
            {
                int pos = RandomInt(1, NowTotalNum);
                time1 = timeNow();
                sa->Insert(ToInsert, pos);
                time2 = timeNow();
                int tsa = duration(time2 - time1);
                Tsa[2] += tsa;
                Tsa[0] += tsa;
                Inssa = tsa;

                time1 = timeNow();
                da->Insert(ToInsert, pos);
                time2 = timeNow();
                int tda = duration(time2 - time1);
                Tda[2] += tda;
                Tda[0] += tda;
                Insda = tda;


                time1 = timeNow();
                ll->Insert(ToInsert, pos);
                time2 = timeNow();
                int tll = duration(time2 - time1);
                Tll[2] += tll;
                Tll[0] += tll;
                Insll = tll;

                ToInsert++;
                NowTotalNum++;
                break;
            }

            case 3: //delete
            {
                int pos = RandomInt(1, NowTotalNum);
                time1 = timeNow();
                sa->Delete(pos);
                time2 = timeNow();
                int tsa = duration(time2 - time1);
                Tsa[3] += tsa;
                Tsa[0] += tsa;
                Inssa = tsa;

                time1 = timeNow();
                da->Delete(pos);
                time2 = timeNow();
                int tda = duration(time2 - time1);
                Tda[3] += tda;
                Tda[0] += tda;
                Insda = tda;


                time1 = timeNow();
                ll->Delete(pos);
                time2 = timeNow();
                int tll = duration(time2 - time1);
                Tll[3] += tll;
                Tll[0] += tll;
                Insll = tll;

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

                time1 = timeNow();
                int * AnsSA = sa->RangeQuery(start, end, &NumSA);
                time2 = timeNow();
                int tsa = duration(time2 - time1);

                time1 = timeNow();
                sa->Reorder(start, end, newArray);
                time2 = timeNow();
                tsa += duration(time2 - time1);

                Tsa[4] += tsa;
                Tsa[0] += tsa;
                Inssa = tsa;


                time1 = timeNow();
                int * AnsDA = da->RangeQuery(start, end, &NumDA);
                time2 = timeNow();
                int tda = duration(time2 - time1);

                time1 = timeNow();
                da->Reorder(start, end, newArray);
                time2 = timeNow();
                tda += duration(time2-time1);

                Tda[4] += tda;
                Tda[0] += tda;
                Insda = tda;


                time1 = timeNow();
                int * Ansll = ll->RangeQuery(start, end, &Numll);
                time2 = timeNow();
                int tll = duration(time2 - time1);

                time1 = timeNow();
                ll->Reorder(start, end, newArray);
                time2 = timeNow();
                tll += duration(time2-time1);

                Tll[4] += tll;
                Tll[0] += tll;
                Insll = tll;

                break;
            }

            case 5: //swap
            {
                //printf("here is swap, lt = %d\n", lt);
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
                int tsa = duration(time2 - time1);
                Tsa[5] += tsa;
                Tsa[0] += tsa;
                Inssa = tsa;

                time1 = timeNow();
                da->Swap(start1, end1, start2, end2);
                time2 = timeNow();
                int tda = duration(time2 - time1);
                Tda[5] += tda;
                Tda[0] += tda;
                Insda = tda;


                time1 = timeNow();
                ll->Swap(start1, end1, start2, end2);
                time2 = timeNow();
                int tll = duration(time2 - time1);
                Tll[5] += tll;
                Tll[0] += tll;
                Insll = tll;

                break;
            }

            case 6: //move
            {
                int a[3] = {};
                for (int j = 0; j < 3; ++j) {
                    a[j] = RandomInt(1, NowTotalNum);
                }
                sort(a, a+3);
                if (a[1] == a[2]) {
                    continue;
                }
                int start1, end1, start2;
                srand(time(0));
                int tmp = rand();
                if (tmp % 2 == 1) {
                    start1 = a[0];
                    end1 = a[1];
                    start2 = a[2];
                } else {
                    start1 = a[1];
                    end1 = a[2];
                    start2 = a[0];
                }

                time1 = timeNow();
                sa->Move(start1, end1, start2);
                time2 = timeNow();
                int tsa = duration(time2 - time1);
                Tsa[6] += tsa;
                Tsa[0] += tsa;
                Inssa = tsa;

                time1 = timeNow();
                da->Move(start1, end1, start2);
                time2 = timeNow();
                int tda = duration(time2 - time1);
                Tda[6] += tda;
                Tda[0] += tda;
                Insda = tda;


                time1 = timeNow();
                ll->Move(start1, end1, start2);
                time2 = timeNow();
                int tll = duration(time2 - time1);
                Tll[6] += tll;
                Tll[0] += tll;
                Insll = tll;

                break;
            }

        }

        if ((lt+1)%ComputeInterval == 0) {
            CurOutputNum ++;
            double fl = CurOutputNum * 1.0 /(NumOfOutput * 1.0);
            finstant<<fl<<" ,"<<Insda<<","<<Inssa<<","<<Insll<<endl;
        }


    } //for lt <= loopTime
    faverage<<"Data Structure,short query,long query,insert,delete,reorder,swap,move"<<endl;
    faverage<<"Dynamic Array,"<< Tda[1]/ShortQueryActions<<","
            << Tda[7]/LongQueryActions<<"," << Tda[2]/InsertActions<<","
            << Tda[3]/DeleteActions<<","<<Tda[4]/ReorderActions<<","
            << Tda[5]/SwapActions<<","<<Tda[6]/SwapActions<<endl;

    faverage<<"Standard Array,"<< Tsa[1]/ShortQueryActions<<","
            << Tsa[7]/LongQueryActions<<"," << Tsa[2]/InsertActions<<","
            << Tsa[3]/DeleteActions<<","<<Tsa[4]/ReorderActions<<","
            << Tsa[5]/SwapActions<<","<<Tsa[6]/SwapActions<<endl;

    faverage<<"Linked List,"<< Tll[1]/ShortQueryActions<<","
            << Tll[7]/LongQueryActions<<"," << Tll[2]/InsertActions<<","
            << Tll[3]/DeleteActions<<","<<Tll[4]/ReorderActions<<","
            << Tll[5]/SwapActions<<","<<Tll[6]/SwapActions<<endl;

}

