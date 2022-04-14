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
#include <vector>
using namespace std;
using namespace Seq;
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
            cout << "different for "<< i << endl;
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


int main() {
    const int iniNum = 100000;
    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }
    StandardArray *sa = NewStandardArray(array, iniNum);
    vector<int> vec;
    int * unwrapped_array = new int[iniNum];
    vec.reserve(iniNum);
    for (int i = 0; i < iniNum; ++i) {
        vec.push_back(array[i]);
        unwrapped_array[i] = array[i];
    }
    TimeVar time1, time2;
    cout << "point query" << endl;
    int Tvec = 0, Tsa = 0, Tunwrapped = 0;

    for (int i=0; i < 100; i ++) {
        int pos = RandomInt(1, iniNum);

        time1 = timeNow();
        int qans = vec[pos-1];
        time2 = timeNow();
        Tvec += duration(time2 - time1);

        time1 = timeNow();
        qans = unwrapped_array[pos-1];
        time2 = timeNow();
        Tunwrapped += duration(time2 - time1);

        time1 = timeNow();
        qans = sa->Query(pos);
        time2 = timeNow();
        Tsa += duration(time2 - time1);
    }

    printf("time for vec, unwrapped_array, sa = %d, %d, %d (ns)", Tvec/100, Tunwrapped/100, Tsa/100);

    cout << "range query" << endl;
    int start = 100, end=400;
    int * lens_of_ans;
    for (int i=0; i < 1000; i ++) {
        int pos = RandomInt(1, iniNum);

        time1 = timeNow();
        int * qans = new int[500];
        for (int j = start; j < end; j ++) {
            qans[j-start] = vec[j];
        }
        time2 = timeNow();
        Tvec += duration(time2 - time1);

        time1 = timeNow();
        for (int j = start; j < end; j ++) {
            qans[j-start] = unwrapped_array[j];
        }
        time2 = timeNow();
        Tunwrapped += duration(time2 - time1);

        time1 = timeNow();
        qans = sa->RangeQuery(start, end, lens_of_ans);
        time2 = timeNow();
        Tsa += duration(time2 - time1);
    }

    printf("time for vec, unwrapped_array, sa = %d, %d, %d (ns)", Tvec, Tunwrapped, Tsa);

    return 0;
}



