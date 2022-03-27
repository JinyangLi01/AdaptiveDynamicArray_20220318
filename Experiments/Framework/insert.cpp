
// thread::get_id / this_thread::get_id
#include <iostream>       // std::cout
#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
#include <chrono>         // std::chrono::seconds
#include <vector>
using namespace std;
void show(int n) {
        this_thread::sleep_for(chrono::seconds(5));
        cout<<"sleep_for(chrono::seconds(5)) end"<<endl;
}
int *s;

void add1() {
    for (int i = 0; i < 3; ++i) {
        s[i] += 1;
    }
}

int main() {
    int numTh = 2;
    s = new int[3];
    for (int i = 0; i < 3; ++i) {
        s[i] = i;
    }
    vector<thread> threads;
    threads.reserve(numTh);
    for (int i = 0; i < numTh; ++i) {
        threads.emplace_back(thread(add1));
    }
    vector<thread>::iterator it;
    int joinNum = 1;
    for (it = threads.begin(); it != threads.end(); ++it) {
        printf("before joining th %d\n", joinNum);
        it->join();
        printf("after joining th %d\n", joinNum);
        joinNum++;
    }
    printf("before return\n");
    return 0;
}