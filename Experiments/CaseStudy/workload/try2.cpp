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
#include "../../../DataStructures/DynamicArray.h"
#include "../../../DataStructures/StandardArray.h"
#include "../../../DataStructures/CountedBtree.h"
#include "../../../DataStructures/LinkedList.h"
#include "../../../DataStructures/tiered-vector.h"
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

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

string RandomString(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    string s;
    for (int i = 0; i < len; ++i) {
        s += alphanum[RandomInt(1, 100000000) % (sizeof(alphanum) - 1)];
    }
    return s;
}

sql::Driver *driver;
struct SQLCONN {
public:
    sql::Connection* sqlconnection{};
    sql::PreparedStatement *pstmtQuery{};
    sql::PreparedStatement* pstmtInsert{};
    sql::PreparedStatement* pstmtDelete{};

};
sql::ResultSet *res0;
sql::ResultSet *res1;
vector<SQLCONN*> SqlConVector;
vector<atomic_flag*> SqlConAvaliable;
atomic_int numSqlCons;
atomic_flag available0, available1;
int TqueryIdx = 0; // useless

const int operations = 50;
const int iniNum = 225675; // movie dataset 110k awards
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
Seq::Tiered<int, LayerItr<LayerEnd, Layer<22, Layer<22, Layer<22>>>>> tv;


class MysqlRow {
public:
    int id{};
    string director_name, ceremony, year, category, outcome, original_language;
};


auto * rows0 = new MysqlRow[5*prewindow];
auto * rows1 = new MysqlRow[5*prewindow];

// time query db once
long long Thisdb = 0;
// TqueryDB: query time for db in asyn threads, only for DB!!!
// TqueryADA, TquerySA, TqueryCBT, TqueryTV: query time for syn main thread
long long TqueryDB = 0, TqueryADA = 0, TquerySA = 0, TqueryCBT=0, TqueryLL=0, TqueryTV=0;



void FetchSqlRes0(MysqlRow * rows) {
    int i = 0;
    while (res0->next()) {
        // You can use either numeric offsets...
        rows[i].id = res0->getInt(1);
        rows[i].director_name = res0->getString(2);
        rows[i].ceremony = res0->getString(3);
        rows[i].year = res0->getInt(4);
        rows[i].category = res0->getString(5);
        rows[i].outcome = res0->getString(6);
        rows[i].original_language = res0->getString(7);
        i++;
        //cout << "id = " << res->getInt(1); // getInt(1) returns the first column
        // ... or column names for accessing results.
        // The latter is recommended.
        //cout << ", label = '" << res->getString("label") << "'" << endl;
    }
}

void FetchSqlRes1(MysqlRow * rows) {
    cout << "Here is FetchSqlRes1" << endl;
    int i = 0;
    while (res1->next()) {
        // You can use either numeric offsets...
        cout << "in which loop, i = " << i << endl;
        rows[i].id = res0->getInt("id");
        cout<< rows[i].id << endl;
        rows[i].director_name = res0->getString(2);
        cout<< rows[i].director_name << endl;
        rows[i].ceremony = res0->getString(3);
        rows[i].year = res0->getString(4);
        rows[i].category = res0->getString(5);
        rows[i].outcome = res0->getString(6);
        rows[i].original_language = res0->getString(7);
        cout<< rows[i].original_language << endl;
        i++;
        //cout << "id = " << res->getInt(1); // getInt(1) returns the first column
        // ... or column names for accessing results.
        // The latter is recommended.
        //cout << ", label = '" << res->getString("label") << "'" << endl;
    }
}


void QueryDB0(MysqlRow * rows0, int * IDs, int num, int queryindex, int threadIdx) {
    TimeVar timea = timeNow();
    SQLCONN * sqlconn;
    bool find = false;
    int i = 0;
    while (!find) {
        for (; i < numSqlCons; ++i) {
            if ((*(SqlConAvaliable[i])).test_and_set()) { // already set
                continue;
            } else { // new set
                sqlconn = SqlConVector.operator[](i);
                find = true;
                break;
            }
        }
    }
    sql::PreparedStatement* queryStmt = sqlconn->pstmtQuery;
    for (int k = 1; k <= num; ++k) {
        queryStmt->setInt(k, IDs[k-1]);
    }
    for (int k = num+1; k <= 5*prewindow; ++k) {
        queryStmt->setInt(k, 0);
    }
    res0 = queryStmt->executeQuery();
    FetchSqlRes0(rows0);
    available0.test_and_set();
    TimeVar timeb = timeNow();
    Thisdb = duration(timeb - timea);
    if (queryindex >= 0) {
        TqueryDB += Thisdb;
    }
    (*(SqlConAvaliable[i])).clear();
}

void QueryDB1(MysqlRow * rows1, int * IDs, int num, int queryindex, int threadIdx) {
    cout<<"Here is QueryDB1"<<endl;
    TimeVar timea = timeNow();
    SQLCONN * sqlconn;
    bool find = false;
    int i = 0;
    while (!find) {
        for (; i < numSqlCons; ++i) {
            if ((*(SqlConAvaliable[i])).test_and_set()) { // already set
                continue;
            } else { // new set
                sqlconn = SqlConVector.operator[](i);
                find = true;
                break;
            }
        }
    }
    cout<<"1"<<endl;
    sql::PreparedStatement* queryStmt = sqlconn->pstmtQuery;
    printf("queryStmt: %s", queryStmt);
    for (int k = 1; k <= num; ++k) {
        queryStmt->setInt(k, IDs[k-1]);
    }
    for (int k = num+1; k <= 5*prewindow; ++k) {
        queryStmt->setInt(k, 0);
    }
    cout<<"2"<<endl;
    
    res1 = queryStmt->executeQuery();
    FetchSqlRes1(rows1);
    cout << "after fetch sql res" << endl;
    available1.test_and_set();
    TimeVar timeb = timeNow();
    Thisdb = duration(timeb - timea);
    if (queryindex >= 0) {
        TqueryDB += Thisdb;
    }
    (*(SqlConAvaliable[i])).clear();
}

void simulateUserResponseTime() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::normal_distribution<double> distribution (1000.0,10.0);

    int wait = distribution(generator);
    while (wait < 500) {
        wait = distribution(generator);
    }
    if (wait > 1000) {
        wait = 1000;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));
}

void DoQueries(int num_queries, int pos) {
    int winstart = pos - 14, winend = pos + 15;
    int bufferstart = winstart - 2 * prewindow;
    int bufferend = winend + 2 * prewindow;
    int num = 0;
    TimeVar timea, timeb;
    for (int numPre = 0; numPre < num_queries; ++numPre) {
         // suppose use only scoll down
        pos += prewindow;
        winstart = pos - 14, winend = pos + 15;
        bufferstart = winstart - 2 * prewindow;
        bufferend = winend + 2 * prewindow;
        if (winend > NowTotalNum) {
            pos -= prewindow * 2;
            winstart = pos - 14, winend = pos + 15;
            bufferstart = winstart - 2 * prewindow;
            bufferend = winend + 2 * prewindow;
        }
        if (bufferend > NowTotalNum) {
            bufferstart = NowTotalNum;
        }

        TimeVar timea = timeNow();
        int * IDs = da->RangeQuery(bufferstart, bufferend, &num);
        TimeVar timeb = timeNow();
        TqueryADA += duration(timeb - timea);

        timea = timeNow();
        IDs = sa->RangeQuery(bufferstart, bufferend, &num);
        timeb = timeNow();
        TquerySA += duration(timeb - timea);

        timea = timeNow();
        IDs = cbt->RangeQuery(bufferstart, bufferend, &num);
        timeb = timeNow();
        TqueryCBT += duration(timeb - timea);

        timea = timeNow();
        IDs = ll->RangeQuery(bufferstart, bufferend, &num);
        timeb = timeNow();
        TqueryLL += duration(timeb - timea);

        timea = timeNow();
        IDs = tv.RangeQuery(bufferstart, bufferend, num);
        timeb = timeNow();
        TqueryTV += duration(timeb - timea);

        TimeVar time1, time2;
        bool ready = false;
        if (numPre % 2 == 0) {
            if (available1.test_and_set()) { // already set
                ready = true;
                available1.clear();
            } else {
                available1.clear();
            }
        } else {
            if (available0.test_and_set()) { // already set
                ready = true;
                available0.clear();
            } else {
                available0.clear();
            }
        }

        if (ready) {
            if (numPre % 2 == 0) {
                threads.emplace_back(QueryDB0, rows0, IDs, num, TqueryIdx, threadIdx);
                threadIdx++;
            } else {
                threads.emplace_back(QueryDB1, rows1, IDs, num, TqueryIdx, threadIdx);
                threadIdx++;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } else {
            if (numPre % 2 == 0) {
                time1 = timeNow();
                thread th0(QueryDB0, rows0, IDs, num, -1, threadIdx);
                th0.join();
                threadIdx++;
                time2 = timeNow();
                TqueryADA += duration(time2 - time1);
                TquerySA += duration(time2 - time1);
                TqueryCBT += duration(time2 - time1);
                TqueryLL += duration(time2 - time1);
                TqueryTV += duration(time2 - time1);
            } else {
                time1 = timeNow();
                thread th1(QueryDB1, rows1, IDs, num, -1, threadIdx);
                th1.join();
                threadIdx++;
                time2 = timeNow();
                TqueryADA += duration(time2 - time1);
                TquerySA += duration(time2 - time1);
                TqueryCBT += duration(time2 - time1);
                TqueryLL += duration(time2 - time1);
                TqueryTV += duration(time2 - time1);
            }
        }
    }
}


int main() {

    try {
        
        numSqlCons.operator=(100);


        /* Create a connection */
        driver = get_driver_instance();

        string queryString = "SELECT * FROM awards WHERE id in (?";
        for (int l = 1; l < 5*prewindow; ++l) {
            queryString += ", ?";
        }
        queryString += ")";

        for (int l = 0; l < numSqlCons; ++l) {
            auto * sqlconn = new SQLCONN();
            sqlconn->sqlconnection = driver->connect("localhost", "root", "LjyDq1998");
            /* Connect to the MySQL test database */
            sqlconn->sqlconnection->setSchema("ADA1");
            sqlconn->pstmtQuery = sqlconn->sqlconnection->prepareStatement(queryString);
            SqlConVector.push_back(sqlconn);
            auto * ai = new atomic_flag; (*ai).clear();
            SqlConAvaliable.push_back(ai);
        }
        printf("size of SqlConVector = %lu\n", SqlConVector.size());

        //var time1, time2 time.Time
        string filepath[3] = {"wl2.csv", "wl2Log.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);


        /*
         * 1: query, 2: insert, 3: delete, 4: reorder, 5: swap
         */
        printf("10 queries, 2 insert, 2 delete, 10 query, 1 reorder, 1 swap, 14 queries\n");
        printf("# of operations = %d\n", operations);

        

        TimeVar time1, time2, time3;

        NowTotalNum = iniNum;
        

        int depth = 1, NumDA;

        // 10 queries:
        cout << "10 queries" << endl;


        available0.clear();
        available1.clear();

        pos = 15;
        int winstart = pos - 14, winend = pos + 15;
        int bufferstart = winstart - 2 * prewindow;
        int bufferend = winend + 2 * prewindow;
        int num = 0;


        int *IDs = da->RangeQuery(bufferstart, bufferend, &num);

        thread th(QueryDB1, rows1, IDs, num, -1, threadIdx);
        th.join();
        threadIdx++;

        cout<<"before DoQueries(10)"<<endl;
        DoQueries(1, pos);


        //2 insert 
        cout << "2 insert" << endl;
        
        flog.close();
        finstant.close();
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    return 0;
}

