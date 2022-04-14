#include <iostream>
#include <fstream>
#include <chrono>
#include "StandardArray.h"
#include "../../DA_Bidirectional/DAbidirectional.h"
#include "LinkedList.h"
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


int RandomInt(int s, int e) {
    return (rand() % (e-s+1))+ s;
}


int main() {
    int length = 5000;
    int * AnsSA, * AnsDA, * AnsLL;
    int * array = new int[length];
    for (int i = 0; i < length; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, length, 4);
    StandardArray *sa = NewStandardArray(array, length);
    LinkedList * ll = NewLinkedListForArray(10, array, length);
    delete []array;
    int index[4];
    int NumAnsDA, NumAnsSA;
    int k = 0;
    for (k = 0; k < 5000; ++k) {
        //cout<<"---------------------------------------------------- k =  " <<k<<endl;
        index[0] = RandomInt(1, length);
        index[1] = RandomInt(1, length);
        index[2] = RandomInt(1, length);
        index[3] = RandomInt(1, length);
        sort(index, index+4);
        //printf("swap %d %d %d %d\n", index[0], index[1], index[2], index[3]);

        if (index[1] == index[2]) {
            continue;
        }
        int CutArrayTime, FindLeafTime, SwapLeafTime, CheckMinTime;
        TimeVar time1 = timeNow();
        ll->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time2 = timeNow();
        int llAll = duration(time2-time1);
        //printf("CutArrayTime = %d, FindLeafTime = %d, SwapLeafTime = %d, CheckMinTime = %d\n", CutArrayTime, FindLeafTime, SwapLeafTime, CheckMinTime);

        TimeVar time3 = timeNow();
        sa->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time4 = timeNow();
        int saAll = duration(time4-time3);

        AnsLL = ll->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        if (!CompareArray(AnsLL, AnsSA, NumAnsDA)) {
            cout<< "============== not equal"<<endl;
            break;
        }
        //printf("ll: %d, sa: %d\n", llAll, saAll);
    }

    for (k = 0; k < 1; ++k) {
        index[0] = RandomInt(1, length);
        index[1] = RandomInt(1, length);
        index[2] = RandomInt(1, length);
        index[3] = RandomInt(1, length);
        sort(index, index+4);
        //printf("swap %d %d %d %d\n", index[0], index[1], index[2], index[3]);

        if (index[1] == index[2]) {
            continue;
        }
        int CutArrayTime, FindLeafTime, SwapLeafTime, CheckMinTime;
        TimeVar time1 = timeNow();
        ll->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time2 = timeNow();
        int llAll = duration(time2-time1);
        //printf("CutArrayTime = %d, FindLeafTime = %d, SwapLeafTime = %d, CheckMinTime = %d\n", CutArrayTime, FindLeafTime, SwapLeafTime, CheckMinTime);

        TimeVar time3 = timeNow();
        sa->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time4 = timeNow();
        int saAll = duration(time4-time3);

        AnsLL = ll->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        if (!CompareArray(AnsLL, AnsSA, NumAnsDA)) {
            cout<< "not equal"<<endl;
            break;
        }
        printf("da: %d, sa: %d\n", llAll, saAll);
    }
    //fclose(f);
    return 0;
}
