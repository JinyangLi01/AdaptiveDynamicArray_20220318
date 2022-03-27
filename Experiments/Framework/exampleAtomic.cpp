#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
#include <random>
#include <algorithm>
#include <vector>
#include <string>
#include <mutex>
#include <atomic>
using namespace std;

int main() {
    vector<atomic_bool*> flags;
    atomic_bool af1(true);
    flags.push_back(&af1);
    atomic_bool af2(false);
    flags.push_back(&af2);
    for (auto & flag : flags) {
        if (*flag) {
            printf("set ");
        } else {
            printf("unset ");
        }
    }
    return 0;
}