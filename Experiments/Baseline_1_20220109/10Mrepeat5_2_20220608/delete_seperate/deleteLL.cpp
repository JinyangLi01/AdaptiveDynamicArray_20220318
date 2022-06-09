#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include "../../../../DataStructures/LinkedList.h"
#include <vector>
using namespace std;
// using namespace Seq;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
bool CompareArray(const int *a, const int *b, int len) {
    if (a == nullptr || b == nullptr) {
        cout<<"null pointer error!" << endl;
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
    return n;
}



int * VectorRangeQuery(vector<int> &vec, int start, int end) {
    int queryLength = end - start + 1;
    int * qans = new int[queryLength];
    for (int i = start-1; i <= end-1; i++){
        qans[i-start+1] = vec[i];
    }
    return qans;
}


void VectorSwap(vector<int> &vec, int start1, int end1, int start2, int end2)  {
    int len = end2-start1+1;
    int * newarray = new int[len+5];// make([]int, len)
    int in = 0;
    for (int k = start2-1; k <= end2-1; k ++) {
        newarray[in] = vec[k];
        in++;
    }

    for (int k = end1; k < start2-1; k ++) {
        newarray[in] = vec[k];
        in++;
    }

    for (int k = start1-1; k <= end1-1; k ++) {
        newarray[in] = vec[k];
        in++;
    }
    in = 0;
    for (int k = start1-1; k <= end2-1; k ++) {
        vec[k] = newarray[in];
        in++;
    }
    delete []newarray;
}


void SAInsert(int * & array, int ToInsert, int pos, int NumItems, int & Length) {
    if (pos > NumItems) {
        pos = NumItems;
    }
    if (NumItems +1 > Length) {
        int oldLen = Length;
        int newLen = Length * 2;
        // printf("SAInsert, allocate %d\n", newLen+1);
        int * newarray = new int[newLen + 1];
        int j = 0;
        for (int i = 0; i < NumItems + 1; ++i) {
            if (i == pos-1) {
                continue;
            }
            newarray[i] = array[j];
            j++;
        }
        newarray[pos-1] = ToInsert;
        delete []array;
        array = newarray;
        NumItems += 1;
        Length = newLen;
        return;
    }
    int itemToStore = ToInsert;
    for (int i = pos-1; i <= NumItems; ++i) {
        int tmp = array[i];
        array[i] = itemToStore;
        itemToStore = tmp;
    }
}

void SADelete(int * & array, int pos, int NumItems) {
    if (pos > NumItems) {
        pos = NumItems;
    }
    for (int i = pos; i < NumItems; i++) {
        array[i-1] = array[i];
    }
}

void SAReorder(int * & array, int start, int end, const int * newID) {
    for (int i = start; i <= end; ++i) {
        array[i-1] = newID[i-start];
    }
}

void SASwap(int * & array, int start1, int end1, int start2, int end2, int NowTotalNum) {
    int len = end2-start1+1;
    int * newarray = new int[len+5];// make([]int, len)
    int in = 0;
    for (int k = start2-1; k <= end2-1; k ++) {
        newarray[in] = array[k];
        in++;
    }

    for (int k = end1; k < start2-1; k ++) {
        newarray[in] = array[k];
        in++;
    }

    for (int k = start1-1; k <= end1-1; k ++) {
        newarray[in] = array[k];
        in++;
    }
    in = 0;
    for (int k = start1-1; k <= end2-1; k ++) {
        array[k] = newarray[in];
        in++;
    }
    delete []newarray;
}

int * SARangeQuery(int * & array, int start, int end) {
    int queryLength = end - start + 1;
    int * qans = new int[queryLength];
    for (int i = start-1; i <= end-1; i++){
        qans[i-start+1] = array[i];
    }
    return qans;
}


int main(int argc, char** argv) {
    string filepath[3];
    // filepath[0] = argv[1];
    // filepath[1] = argv[2];
    // ofstream finstant, flog, ffinal;
    // finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
    // flog.open(filepath[1], ios::out | ios::in | ios::trunc);

   filepath[0] = "deleteLL.csv";
   filepath[1] = "deleteLLLog.txt";
   ofstream finstant, flog, ffinal;
   finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
   flog.open(filepath[1], ios::out | ios::in | ios::trunc);

    finstant<<" ,DA,SA,LL,TV,VEC"<<endl;

    int iniNum = 10000000;
    int danodesize = 200;
    int m = 2000;  //for linked list
    int operations = 100000;
    int InsertActions = operations * 25 / 100;
    int DeleteActions = operations * 25 / 100;
    int ReorderActions = operations * 25 / 100;
    int SwapActions = operations * 25 / 100;
    int TotalActions = DeleteActions + InsertActions + ReorderActions+SwapActions;
    printf("%d, %d, %d, %d\n", InsertActions, DeleteActions, ReorderActions, SwapActions);
    printf("# of operations = %d\n", operations);
    int CurOutputNum = 0;

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
    for (int y = 0; y < InsertActions; y++) {
        a[ua] = 2;
        ua++;
    }
    for (int y = 0; y < DeleteActions; y++) {
        a[ua] = 3;
        ua++;
    }

    shuffle(a, a + TotalActions, generator());
    //random_shuffle(&a[DeleteActions+InsertActions], &a[TotalActions]);
    //random_shuffle(&a[0], &a[MoveActions+SwapActions+ReorderActions]);
    //random_shuffle(&a[0], &a[operations]);
    //int * shortqueryrange = RangeDistributionRandom(, 1, 100);
    int * reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
    int ir = 0;

    TimeVar time1, time2;

    int NowTotalNum = iniNum;
    int ToInsert = iniNum + 1;
    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }

    int * standard_array = new int[iniNum];
    for (int i = 0; i < iniNum; i ++) {
        standard_array[i] = array[i];
    }
    int length_SA = iniNum;
    LinkedList * ll = NewLinkedListForArray(m, array, iniNum);
    

    delete []array;
    int NumSA, NumDA, Numll, NumCBT, NumTV;
    int depth = 1;
    int numUpdate = 0;
    for (int lt = 0; lt < TotalActions; lt++) {
    if (lt % 5000 == 0) {
        cout<<"lt = "<<lt<< endl;
    }
    switch (a[lt]) {

        case 2: //insert
        {
            int pos = RandomInt(1, NowTotalNum);
            ll->Insert(ToInsert, pos);
            ToInsert++;
            NowTotalNum++;
            break;
        }
        case 3: //delete
        {
            int pos = RandomInt(1, NowTotalNum);
            ll->Delete(pos);
            NowTotalNum--;
            break;
        }
        case 4: //reorder
        {
            int len = reorderrange[ir];
            if (len == 0) {
                len = 1;
            }
            ir++;
            if (len >= NowTotalNum) {
                len = NowTotalNum-1;
            }
            int start = RandomInt(1, NowTotalNum - len);
            int end = start + len - 1;
            if (end >= NowTotalNum) {
                end = NowTotalNum - 1;
            }
            int num = 0;

            int * oldArray = ll->RangeQuery(start, end, &num);
            // sa->RangeQuery(start, end, &NumSA);
            int * newArray = new int[len];
            for (int j = 0; j < len; ++j) {
                newArray[j] = oldArray[len-j-1];
            }
            ll->Reorder(start, end, newArray);
            delete []newArray;
            delete []oldArray;
            break;
        }
        case 5: //swap leaks happens here!!??
        {
            int b[4] = {};
            for (int & j : b) {
                j = RandomInt(1, NowTotalNum);
            }
            sort(b, b + 4);
            if (b[1] == b[2]) {
                continue;
            }
            int start1 = b[0];
            int end1 = b[1];
            int start2 = b[2];
            int end2 = b[3];
            ll->Swap(start1, end1, start2, end2);
            break;
        }
    }

    if ( (lt+1 <= 10) || ((lt+1<=100) && ((lt+1)%10 == 0)) || ((lt+1<=1000) && ((lt+1)%100 == 0))
         || ((lt+1<=10000) && ((lt+1)%1000 == 0)) || ((lt+1<=100000) && ((lt+1)%10000 == 0))
         || ((lt+1<=1000000) && ((lt+1)%100000 == 0))) {
        numUpdate ++;

        long long Tsa = 0, Tda = 0, Tll = 0, Ttv=0, Tvec = 0;
        CurOutputNum ++;
        double fl = (lt+1)*1.0/operations ;
        finstant << fl << ",";

        int pos = RandomInt(1, NowTotalNum);
        // printf("delete %d\n", pos);

        time1 = timeNow();
        ll->Delete(pos);
        time2 = timeNow();
        Tsa = duration(time2-time1);


        finstant << Tsa <<endl;

        NowTotalNum--;
    }
    numUpdate++;
} //for lt <= loopTime
    delete []reorderrange;
    delete []a;
    flog.close();
    finstant.close();
    return 0;
}
