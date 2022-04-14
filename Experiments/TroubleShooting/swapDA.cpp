#include <iostream>
#include <fstream>
#include <chrono>
#include "../../DataStructures/StandardArray.h"
#include "../../DA_Bidirectional/DAbidirectional.h"
#include "../../DataStructures/DAnolimit.h"
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
    int length = 7000;
    int * AnsSA, * AnsDA, * AnsDANL;
    int * array = new int[length];
    for (int i = 0; i < length; ++i) {
        array[i] = i+1;
    }
    DAbidirectional *da = NewDynamicArray(array, length, 6);
    StandardArray *sa = NewStandardArray(array, length);
    //DAnolimit *daNoLimit = NewDAnolimit(array, length, 6);
    delete []array;
    int index[4];
    int NumAnsDA, NumAnsSA, NumAnsDANL;
    int k = 0;
    for (k = 0; k < 20000; ++k) {
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
        da->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time2 = timeNow();
        int daAll = duration(time2-time1);
        //printf("CutArrayTime = %d, FindLeafTime = %d, SwapLeafTime = %d, CheckMinTime = %d\n", CutArrayTime, FindLeafTime, SwapLeafTime, CheckMinTime);

       // daNoLimit->Move(index[0], index[1], index[2]);

        TimeVar time3 = timeNow();
        sa->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time4 = timeNow();
        int saAll = duration(time4-time3);

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
       // AnsDANL = daNoLimit->RangeQuery(1, length, &NumAnsDANL);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            cout<< "not equal"<<endl;
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
        index[0] = RandomInt(1, length);
        index[1] = RandomInt(1, length);
        index[2] = RandomInt(1, length);
        index[3] = RandomInt(1, length);
        sort(index, index+4);
        cout<<"swap            "<<index[0]<<" "<<index[1]<<" "<<index[2]<<" "<<index[3]<<endl;
        if (index[1] == index[2]) {
            continue;
        }
        cout<<"before swap"<<endl;
      //  da->PrintTree();
        int CutArrayTime, FindLeafTime, SwapLeafTime, CheckMinTime;
        TimeVar time1 = timeNow();
        da->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time2 = timeNow();
        int daAll = duration(time2-time1);

       // daNoLimit->Move(index[0], index[1], index[2]);

        TimeVar time3 = timeNow();
        sa->Swap(index[0], index[1], index[2], index[3]);
        TimeVar time4 = timeNow();
        int saAll = duration(time4-time3);
        printf("after swap\n");
       // da->PrintTree();
      //  daNoLimit->PrintTree();
        //sa->PrintArray();

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
       // AnsDANL = daNoLimit->RangeQuery(1, length, &NumAnsDANL);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            cout<< "not equal"<<endl;
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
