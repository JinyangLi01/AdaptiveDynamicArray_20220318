#include <iostream>
#include <fstream>
#include <chrono>
#include "../../DataStructures/StandardArray.h"
#include "../../DataStructures/DynamicArray.h"
#include "../../DataStructures/DAnolimit.h"
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


int RandomInt(int s, int e) {
    return (rand() % (e-s+1))+ s;
}


int main() {
    int length = 1700;
    int nowTotalItems = length;
    int toInsert = nowTotalItems + 1;
    int * AnsSA, * AnsDA, * AnsDANL;
    int * array = new int[length];
    for (int i = 0; i < length; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, length, 4);
    StandardArray *sa = NewStandardArray(array, length);
    //DAnolimit *daNoLimit = NewDAnolimit(array, length, 6);
    delete []array;
    int index[4];
    int NumAnsDA, NumAnsSA, NumAnsDANL;
    int k = 0;
    for (k = 0; k < 5000; ++k) {
        int pos = RandomInt(1, nowTotalItems);
        da->Insert(toInsert, pos);
        sa->Insert(toInsert, pos);
        toInsert++;

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        // AnsDANL = daNoLimit->RangeQuery(1, length, &NumAnsDANL);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            printf("===================== k = %d, not equal\n", k);
            break;
        }
        /*
        if (!CompareArray(AnsDA, AnsDANL, NumAnsDA)) {
            cout<< "not equal"<<endl;
            break;
        }
         */
        //printf("da: %d, sa: %d\n", daAll, saAll);
    }

    for (k = 0; k < 1; ++k) {
        int pos = RandomInt(1, nowTotalItems);

        da->PrintDA();
        printf("insert %d into pos %d\n", toInsert, pos);

        da->Insert(toInsert, pos);
        sa->Insert(toInsert, pos);

        printf("after insert\n");
        da->PrintDA();

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        // AnsDANL = daNoLimit->RangeQuery(1, length, &NumAnsDANL);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            printf("===================== k = %d, not equal\n", k);
            da->PrintArray();
            sa->PrintArray();
            break;
        }
        /*
        if (!CompareArray(AnsDA, AnsDANL, NumAnsDA)) {
            cout<< "not equal"<<endl;
            break;
        }
         */
        //printf("da: %d, sa: %d\n", daAll, saAll);
    }
    //fclose(f);
    return 0;
}
