#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>


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
    int a[30];
    for (int & i : a) {
        i = RandomInt(1, 10);
        cout<<i<<" ";
    }
    cout<<endl;
    for (int & i : a) {
        i = RandomInt(1, 10);
        cout<<a[i]<<" ";
    }
    cout<<endl;

    shuffle(a, a+30, generator());
    for (int & i : a) {
        cout<<a[i]<<" ";
    }
    cout<<endl;

    shuffle(a, a+30, generator());
    for (int & i : a) {
        cout<<a[i]<<" ";
    }
    cout<<endl;
    return 0;
}

