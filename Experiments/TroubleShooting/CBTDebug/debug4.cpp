

#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
//#include "CountedBtree.hpp"
#include "../../../DataStructures/CountedBtree.h"
#include "../../../DataStructures/DynamicArray.h"
#include "../../../DataStructures/LinkedList.h"
#include "../../../DataStructures/tiered-vector.h"
//#include "tiered-vector.h"
#include <vector>
#include <cstdio>
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
    // std::uniform_int_distribution<int> dist(min, max);
    // return dist(generator());
    return (rand() % (max-min+1))+ min;
}

int* RangeDistributionRandom( int num, int min, int max) {
    int * n = new int[num];
    for (int j = 0; j < num; ++j) {
        n[j] = RandomInt(min, max);
    }
    //random_shuffle(n, n + num);
    std::shuffle(n, n + num, std::mt19937(std::random_device()()));
    return n;
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
    //  delete []newarray;
}


void SAInsert(int * & array, int ToInsert, int pos, int NumItems, int & Length) {
    if (pos > NumItems) {
        pos = NumItems;
    }
//    printf("before SAInsert:\n");
//    for (int k = 0; k < NumItems; k++) {
//        printf("%d ", array[k]);
//    }
//    printf("\n");
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
//        printf("after SAInsert:\n");
//        for (int k = 0; k < NumItems; k++) {
//            printf("%d ", array[k]);
//        }
//        printf("\n");
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

int * SARangeQuery(const int * & array, int start, int end) {
    int queryLength = end - start + 1;
    int * qans = new int[queryLength];
    for (int i = start-1; i <= end-1; i++){
        qans[i-start+1] = array[i];
    }
    return qans;
}



int main() {
    //var time1, time2 time.Time
    string filepath[3] = {"swap1K.csv", "swap1KLog.txt"};
    ofstream finstant, flog, ffinal;
    finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
    flog.open(filepath[1], ios::out | ios::in | ios::trunc);

    finstant<<" ,DA,SA,LL,CBT,TV,VEC"<<endl;
    long long Tsa = 0, Tda = 0, Tll = 0, Tcbt = 0, Ttv = 0, Tvec = 0;

    int iniNum = 40;
    int danodesize = 10;
    int CbtOrder = 8;
    int m = 50;  //for linked list
    int operations = 100;

    int a[220] =
            {5,4,3,2,2,3,4,4,4,2,2,2,2,3,2,4,2,2,3,3,2,5,4,5,4,4,2,2,4,5,
             2,3,2,3,5,4,5,4,2,4,5,5,2,3,4,2,3,4,5,5,5,3,2,4,3,4,2,2,4,5,
             3,2,4,3,4,5,4,5,2,2,5,3,3,3,2,5,3,4,4,5,4,2,2,2,5,2,2,2,2,4,
             2,3,4,5,5,5,5,4,3,3,4,3,4,3,3,4,4,3,4,2,2,4,4,5,2,4,3,2,3,5,
             2,2,4,2,5,5,5,3,5,3,5,4,3,3,4,3,5,4,4,4,5,3,5,3,4,3,2,2,3,4,
             4,2,2,5,3,4,5,4,2,2,5,5,3,3,5,2,5,5,3,2,3,3,3,5,5,5,3,5,3,5,
             3,4,5,3,3,5,4,2,5,2,4,4,3,4,5,3,5,3,5,2};

//    int * a = new int[500];
//    char read_num[100]; // = nullptr;
//    // FILE * file = fopen("/Users/jinyangli/Desktop/ADADebug/ADADebug/operations.txt", "r");
//    FILE * file = fopen("/Users/jinyangli/Desktop/ADADebug/ADADebug/operations.txt", "r");
//    if (file == nullptr) {
//        perror("Failed: ");
//        return 1;
//    }
//    int p = 0;
//    while(fscanf(file, "%s", read_num) > 0 ) // parse %d followed by ','
//    {
//        char* endread = nullptr;
//        int num = int(strtol(read_num, &endread, 10));
//        a[p++] = num;
//    }
//
//    fclose(file);

//    FILE * file_insert_pos = fopen("/Users/jinyangli/Desktop/ADADebug/ADADebug/insert_pos2.txt", "wb");
//    if (file_insert_pos == nullptr) {
//        perror("Failed: ");
//        return 1;
//    }
//    fprintf(file_insert_pos, "xxxx\n");

    for (int j = 0; j < operations; j ++) {
        cout << a[j] << " " ;
    }
    cout << endl;
    int * reorderrange = RangeDistributionRandom(1000, 1, 100);
    int ir = 0;

    TimeVar time1, time2;

    int NowTotalNum = iniNum;
    int ToInsert = iniNum + 1;

    int * standard_array = new int[iniNum];
    //  int standard_array[1000];
    for (int i = 0; i < iniNum; i ++) {
        standard_array[i] = i+1;
    }
    int length_SA = iniNum;

    CountedBtree * cbt = NewCBTreeForArray(CbtOrder, iniNum);

    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, iniNum, danodesize);
    LinkedList * ll = NewLinkedListForArray(m, array, iniNum);
//    Seq::Tiered<int, LayerItr<LayerEnd, Layer<11, Layer<11, Layer<11>>>>> tiered;
//    tiered.initialize(array, iniNum);

    //StandardArray *sa = NewStandardArray(array, iniNum);
    vector<int> vec;
    vec.reserve(iniNum);
    for (int i = 0; i < iniNum; ++i) {
        vec.push_back(i+1);
    }

    int depth = 1;
    int numUpdate = 0;
    for (int lt = 0; lt < operations; lt++) {
       // std::cout << lt << " " << a[lt] << endl;
        switch (a[lt]) {

            case 2: //insert
            {
                int pos = RandomInt(1, NowTotalNum);
                //fprintf(file_insert_pos, "lt=%d, ToInsert=%d, pos=%d\n", lt, ToInsert, pos);
                SAInsert(standard_array, ToInsert, pos, NowTotalNum, length_SA);
                da->Insert(ToInsert, pos);
                cbt->Insert(ToInsert, pos);
                ll->Insert(ToInsert, pos);
                vec.insert(vec.begin() + pos - 1, ToInsert);
                ToInsert++;
                NowTotalNum++;
                break;
            }
            case 3: //delete
            {
                int pos = RandomInt(1, NowTotalNum);
                SADelete(standard_array, pos, NowTotalNum);
                da->Delete(pos);
                cbt->Delete(pos);
                ll->Delete(pos);
                vec.erase(vec.begin() + pos - 1);
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

                int * oldArray = new int[len];
                for (int j = 0; j < len; ++j) {
                    oldArray[j] = standard_array[start+j];
                }
                // sa->RangeQuery(start, end, &NumSA);
                int * newArray = new int[len];
                for (int j = 0; j < len; ++j) {
                    newArray[j] = oldArray[len-j-1];
                }

                SAReorder(standard_array, start, end, newArray);
                da->Reorder(start, end, newArray);
                cbt->Reorder(start, end, newArray);
                ll->Reorder(start, end, newArray);
                for (int i = start-1; i <= end-1; ++i) {
                    vec[i] = newArray[i-start+1];
                }
                delete []newArray;
                delete []oldArray;
                break;
            }

            case 5: //swap
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
                SASwap(standard_array, start1, end1, start2, end2, NowTotalNum);
                da->Swap(start1, end1, start2, end2);
                cbt->Swap(start1, end1, start2, end2);
                ll->Swap(start1, end1, start2, end2);
                VectorSwap(vec, start1, end1, start2, end2);
                break;
            }
        }

        numUpdate++;
    } //for lt <= loopTime

    finstant <<Tda/10 << ","<< Tsa/10<<","<<Tll/10<<","<<Tcbt/10<<","<<Ttv/10<<","<<Tvec/10<<endl;
    // delete []reorderrange;

    flog.close();
    finstant.close();
    //  fclose(file_insert_pos);
    // delete []standard_array;
    return 0;
}

