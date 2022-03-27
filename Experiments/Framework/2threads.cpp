
// CPP program to demonstrate multithreading 
// using three different callables. 
#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include "../../DataStructures/DynamicArray.h"
using namespace std;


vector<int*> ve;

// A dummy function 
void foo(int Z)
{
    for (int i = 0; i < Z; i++) {
        cout << "Thread using function"
                " pointer as callable\n";
    }
    int *c = new int;
    *c = 3;
    ve.push_back(c);
}

// A callable object 
class thread_obj {
public:
    void operator()(int x)
    {
        for (int i = 0; i < x; i++)
            cout << "Thread using function"
                    " object as  callable\n";
    }
};


void simulateUserResponseTime() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::normal_distribution<double> distribution (1000.0,10.0);

    int wait = distribution(generator);
    while (wait < 500) {
        wait = distribution(generator);
    }
    if (wait > 15000) {
        wait = 15000;
    }
    printf("to sleep for %d ms\n", wait);
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));
}

int main()
{
    int * a = new int;
    *a = 1;
    int * b = new int;
    *b = 2;
    ve.push_back(a);
    ve.push_back(b);
    cout << "Threads 1 and 2 and 3 "
            "operating independently" << endl;
    // This thread is launched by using  
    // function pointer as callable 
    thread th1(foo, 3);

    // This thread is launched by using 
    // function object as callable 
    thread th2(thread_obj(), 3);

    // Define a Lambda Expression 
    auto f = [](int x) {
        for (int i = 0; i < x; i++)
            cout << "Thread using lambda"
                    " expression as callable\n";
    };

    // This thread is launched by using  
    // lamda expression as callable 
    thread th3(f, 3);

    simulateUserResponseTime();
    simulateUserResponseTime();
    simulateUserResponseTime();


    // Wait for the threads to finish 
    // Wait for thread t1 to finish 
    //th1.join();
    th1.detach();

    // Wait for thread t2 to finish 
    th2.join();

    // Wait for thread t3 to finish 
    th3.join();

    for (int i = 0; i < 3; ++i) {
        int * j = ve[i];
        printf("%d ", *j);
    }

    return 0;
} 