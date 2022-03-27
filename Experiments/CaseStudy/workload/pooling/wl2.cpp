/*
 * 225675 dataset size
 * always sleep 500 ms
 * 220K real-world dataset movie dataset 110k awards
 * about 5.5 hours
 * 100K op, 100K dataset
 * danodesize = 50
 * 
 */

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
#include "../../../../DataStructures/DynamicArray.h"
#include "../../../../DataStructures/StandardArray.h"
#include "../../../../DataStructures/CountedBtree.h"
#include "../../../../DataStructures/LinkedList.h"
#include "../../../../DataStructures/tiered-vector.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "ThreadPool.h"
#include "mysql_connection.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define STRING_SIZE 100

using namespace std;
using namespace std::chrono;
using namespace Seq;
#define timeNow() std::chrono::high_resolution_clock::now()
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()



const int operations = 50;
const int iniNum = 225760; // movie dataset 110k awards
const int danodesize = 50;
const int m = 50;
const int CbtOrder=danodesize;
int TinsertIdx = 0, TdeleteIdx = 0;
long long TinsertADA=0, TdeleteADA=0, TinsertSA = 0, TdeleteSA=0, TinsertCBT=0, TdeleteCBT=0,
        TinsertLL=0, TdeleteLL=0, TinsertTV=0, TdeleteTV=0;
long long TinsertDB = 0, TdeleteDB = 0;
long long TreorderADA=0, TreorderSA=0, TreorderCBT=0, TreorderLL=0, TreorderTV=0;
long long TswapADA=0, TswapSA=0, TswapCBT=0, TswapLL=0, TswapTV=0;
int NowTotalNum = iniNum;
int ToInsert = iniNum + 1;
const int prewindow = 30;
int pos = 0; // middle line of the window currently

vector<thread> threads;
int threadIdx = 1;

DynamicArray *da;
StandardArray *sa;
CountedBtree * cbt;
LinkedList * ll;
Seq::Tiered<int, LayerItr<LayerEnd, Layer<500, Layer<500, Layer<500>>>>> tv;

string table_name = "awards6";

// time query db once
long long Thisdb = 0;
// TqueryDB: query time for db in asyn threads, only for DB!!!
// TqueryADA, TquerySA, TqueryCBT, TqueryTV: query time for syn main thread
long long TqueryDB = 0, TqueryADA = 0, TquerySA = 0, TqueryCBT=0, TqueryLL=0, TqueryTV=0;




int main() {
    ThreadPool pool(4);
    std::vector< std::future<int> > results;
    for(int i = 0; i < 8; ++i) {
        // results.emplace_back(
        TimeVar time1 = timeNow();
        pool.enqueue([i] {
            std::cout << "hello " << i << std::endl;
            // std::this_thread::sleep_for(std::chrono::seconds(1));
            std::cout << "world " << i << std::endl;
            return i*i;
        });
        TimeVar time2 = timeNow();
        cout << duration(time2-time1) << endl;
        // );
    }
    TimeVar time1 = timeNow();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    TimeVar time2 = timeNow();


    return 0;
}

