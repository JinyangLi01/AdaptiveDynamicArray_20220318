#include <iostream>
#include <fstream>
#include <chrono>
#include "DataStructures/DynamicArray.h"
#include "DA_Bidirectional/DAbidirectional.h"
#include "DataStructures/CountedBtree.h"
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
    int numItems = 20;
    int * array = new int[numItems];
    for (int i = 0; i < numItems; ++i) {
        array[i] = i;
    }
    DynamicArray * da = new DynamicArray(array, numItems, 4);
    da->PrintDA();

    DynamicArray * da2 = new DynamicArray();
    for (int i = 0; i < numItems; ++i) {
        da2->Insert(i+1, i+1);
    }
    da2->PrintDA();


    return 0;
}

/*
int main() {
    int length = 4000;
    int pos;
    int * AnsSA, * AnsDA, NumAnsDA, NumAnsSA;
    int * array = new int[length];
    for (int i = 0; i < length; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, length, 55);
    StandardArray *sa = NewStandardArray(array, length);
    int newID = length;
    int NumInsert = 10000;
    for (int k = 0; k < NumInsert; ++k) {
        newID++;
        int pos = RandomInt(1, length + 1);
        da->Insert(newID, pos);
        sa->Insert(newID, pos);
        length++;
        // da->PrintDA();
        // sa->PrintArray();

        int s = 1;
        int e = length;
        AnsDA = da->RangeQuery(s, e, &NumAnsDA);

        AnsSA = sa->RangeQuery(s, e, &NumAnsSA);

        if (CompareArray(AnsDA, AnsSA, NumAnsDA) == false) {
            cout << "not equal" << endl;
        }
    }

   // da->PrintDA();
   // sa->PrintArray();
    pos = RandomInt(1, length + 1);
    da->Insert(newID, pos);
    sa->Insert(newID, pos);
    length++;
   // da->PrintDA();
   // sa->PrintArray();

    int s = 1;
    int e = length;
    AnsDA = da->RangeQuery(s, e, &NumAnsDA);

    AnsSA = sa->RangeQuery(s, e, &NumAnsSA);

    if (CompareArray(AnsDA, AnsSA, NumAnsDA) == false) {
        cout << "not equal" << endl;
    }

    int DeleteNum = 9000;
    for (int j = 0; j < DeleteNum; ++j) {
        pos = RandomInt(1, length);
        length--;
        da->Delete(pos);
        sa->Delete(pos);
        int s = 1;
        int e = length;
        AnsDA = da->RangeQuery(s, e, &NumAnsDA);
        AnsSA = sa->RangeQuery(s, e, &NumAnsSA);
        if (CompareArray(AnsDA, AnsSA, NumAnsDA) == false) {
            cout << "not equal" << endl;
        }
    }
}
 */


//move
/*
int main() {
    int length = 20;
    int * AnsSA, * AnsDA, *AnsCBT;
    int * array = new int[length];
    for (int i = 0; i < length; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, length, 4);
    StandardArray *sa = NewStandardArray(array, length);
    CountedBtree *cbt = NewTreeForArray(4, length);
    delete []array;
    int index[3];
    int NumAnsDA, NumAnsSA, NumAnsCBT;
    int k = 0;
    for (k = 0; k < 1; ++k) {
        //cout<<"----------------------------------------------------  " <<k<<endl;
        index[0] = RandomInt(1, length);
        index[1] = RandomInt(1, length);
        index[2] = RandomInt(1, length);
        sort(index, index+3);

        //fprintf(f, "%d %d %d %d\n", index[0], index[1], index[2], index[3]);

        if (index[1] == index[2]) {
            continue;
        }
        da->Move(index[0], index[1], index[2]);
        sa->Move(index[0], index[1], index[2]);
        printf("before move, cbt:\n");
        cbt->PrintTree();
        cbt->Move(index[0], index[1], index[2]);
        printf("after move, cbt:\n");
        cbt->PrintTree();

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        AnsCBT = cbt->RangeQuery(1, length, &NumAnsCBT);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            cout<< "not equal"<<endl;
        }
        if (!CompareArray(AnsDA, AnsCBT, NumAnsCBT)) {
            cout<<"not equal cbt"<<endl;
        }
    }

    for (k = 0; k < 1; ++k) {
        index[0] = RandomInt(1, length);
        index[1] = RandomInt(1, length);
        index[2] = RandomInt(1, length);
        sort(index, index+3);
        cout<<"move            "<<index[0]<<" "<<index[1]<<" "<<index[2]<<endl;
        if (index[1] == index[2]) {
            continue;
        }
        cout<<"before move"<<endl;
        da->PrintTree();
        sa->PrintArray();

        da->Move(index[0], index[1], index[2]);
        sa->Move(index[0], index[1], index[2]);
        cbt->Move(index[0], index[1], index[2]);

        da->PrintTree();
        sa->PrintArray();
        cbt->PrintTree();

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        AnsCBT = cbt->RangeQuery(1, length, &NumAnsCBT);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            cout<< "not equal"<<endl;
        }
        if (!CompareArray(AnsDA, AnsCBT, NumAnsCBT)) {
            cout<<"not equal cbt"<<endl;
        }
    }
    return 0;
}
 */

//swap
/*
int main() {
    FILE *f;
    f = fopen("/Users/jinyangli/Documents/CProjects/IndexStructure/indices", "a");

    int length = 500;
    int pos;
    int * AnsSA, * AnsDA;
    int * array = new int[length];
    for (int i = 0; i < length; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, length, 4);
    StandardArray *sa = NewStandardArray(array, length);
    delete []array;
    int index[4];
    int NumAnsDA, NumAnsSA;
    int k = 0;
    for (k = 0; k < 40000; ++k) {
        //cout<<"----------------------------------------------------  " <<k<<endl;
        index[0] = RandomInt(1, length);
        index[1] = RandomInt(1, length);
        index[2] = RandomInt(1, length);
        index[3] = RandomInt(1, length);
        sort(index, index+4);

        fprintf(f, "%d %d %d %d\n", index[0], index[1], index[2], index[3]);

        if (index[1] == index[2]) {
            continue;
        }
        da->Swap(index[0], index[1], index[2], index[3]);
        sa->Swap(index[0], index[1], index[2], index[3]);

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            cout<< "not equal"<<endl;
        }
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
        da->PrintTree();
        sa->PrintArray();

        da->Swap(index[0], index[1], index[2], index[3]);
        sa->Swap(index[0], index[1], index[2], index[3]);

        da->PrintTree();
        sa->PrintArray();

        AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        if (!CompareArray(AnsDA, AnsSA, NumAnsDA)) {
            cout<< "not equal"<<endl;
        }
    }
    fclose(f);
    return 0;
}
*/

/*
int  main() {
    int length = 5000;
    TimeVar t1, t2;
    int pos;
    int * AnsSA, * AnsDA;
    int * array = new int[length];
    for (int i = 0; i < length; ++i) {
        array[i] = i+1;
    }
    DynamicArray *da = NewDynamicArray(array, length, 50);
    StandardArray *sa = NewStandardArray(array, length);
    //da->PrintDA();
    //sa->PrintArray();
    int NumAnsDA, NumAnsSA;
    int k = 0;
    int TInsertda = 0, TInsertsa = 0, TQueryda = 0, TQuerysa = 0;
    int NumInsert = 2000;
    int TIniqueryda = 0, TIniquerysa = 0;

    int s = RandomInt(1, length);
    int e = RandomInt(1, length);
    if (s > e) {
        int t = e;
        e = s;
        s = t;
    }
    t1 = timeNow();
    AnsDA = da->RangeQuery(s, e, &NumAnsDA);
    t2 = timeNow();
    TIniqueryda = duration(t2-t1);

    t1 = timeNow();
    AnsSA = sa->RangeQuery(s, e, &NumAnsSA);
    t2 = timeNow();
    TIniquerysa = duration(t2-t1);
    cout<<"Query time without any update:"<<endl;
    cout<<"DA "<<TIniqueryda<<endl;
    cout<<"SA "<<TIniquerysa<<endl;

    for (k = 0; k < NumInsert; ++k) {
        int newID = 34+k;
        int pos = RandomInt(1, length+1);
        t1 = timeNow();
        da->Insert(newID, pos);
        t2 = timeNow();
        TInsertda += duration(t2-t1);

        t1 = timeNow();
        sa->Insert(newID, pos);
        t2 = timeNow();
        TInsertsa += duration(t2-t1);
        length ++;
        // da->PrintDA();
        // sa->PrintArray();
        int s = RandomInt(1, length);
        int e = RandomInt(1, length);
        if (s > e) {
            int t = e;
            e = s;
            s = t;
        }
        s = 1;
        e = length;
        t1 = timeNow();
        AnsDA = da->RangeQuery(s, e, &NumAnsDA);
        t2 = timeNow();
        TQueryda += duration(t2-t1);

        t1 = timeNow();
        AnsSA = sa->RangeQuery(s, e, &NumAnsSA);
        t2 = timeNow();
        TQuerysa += duration(t2-t1);

        if (CompareArray(AnsDA, AnsSA, NumAnsDA) == false) {
            cout<< "not equal"<<endl;
        }
    }

    printf("DA Insert time: %d\n", TInsertda/NumInsert);
    printf("SA Insert time: %d\n", TInsertsa/NumInsert);
    printf("DA Query time: %d\n", TQueryda/NumInsert);
    printf("SA Query time: %d\n", TQuerysa/NumInsert);




    for ( k = 0; k < 6; ++k) {
        pos = RandomInt(1, length);
        da->Delete(pos);
        sa->Delete(pos);
        length--;
        int* AnsDA = da->RangeQuery(1, length, &NumAnsDA);
        int * AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
        if (CompareArray(AnsDA, AnsSA, NumAnsDA) == false) {
            cout<< "not equal"<<endl;
        }
    }

    AnsDA = da->RangeQuery(1, length, &NumAnsDA);
    AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
    if (CompareArray(AnsDA, AnsSA, NumAnsDA) == false) {
        cout<< "not equal"<<endl;
    }
    pos = k + 10;
    da->Delete(pos);
    sa->Delete(pos);
    length--;

    AnsDA = da->RangeQuery(1, length, &NumAnsDA);
    AnsSA = sa->RangeQuery(1, length, &NumAnsSA);
    if (CompareArray(AnsDA, AnsSA, NumAnsDA) == false) {
        cout<< "not equal"<<endl;
    }
    return 0;
}
*/