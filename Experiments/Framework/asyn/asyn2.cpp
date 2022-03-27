/*
 * this file is different from asyn that
 * 1. data set is only 100
 * 2. it only has insert and delete, no other operations
 * can change number of operations
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
#include "../../../DataStructures/CountedBtree.h"
#include "../../../DataStructures/StandardArray.h"
#include "../../../DataStructures/DynamicArray.h"
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

#define STRING_SIZE 30
/*
 * ID int,
    Age int,
    Name varchar(30),
    c4 varchar(30),
    c5 varchar(30),
    c6 varchar(30),
    c7 varchar(30),
    c8 varchar(30),
    c9 varchar(30),
    c10 varchar(30)
*/

using namespace std;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

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
const int operations = 500;
const int iniNum = 100; //0.03 = 30000
const int danodesize = 4;
int TinsertIdx = 0, TdeleteIdx = 0;
long long TinsertDB[operations+1000], TdeleteDB[operations+1000], Tinsert[operations+1000], Tdelete[operations+1000];


sql::Driver *driver;
struct SQLCONN {
public:
    sql::Connection* sqlconnection{};
    sql::PreparedStatement* pstmtInsert{};
    sql::PreparedStatement* pstmtDelete{};
};
vector<SQLCONN*> SqlConVector;
vector<atomic_flag*> SqlConAvaliable;
atomic_int numSqlCons;


void insertToDB(int ToInsert, int insertindex, int threadIdx) {
    TimeVar time1 = timeNow();
    SQLCONN * sqlconn;
    bool find = false;
    int i = 0;
    for (; i < numSqlCons; ++i) {
        if ((*(SqlConAvaliable[i])).test_and_set()) { // already set
            //printf("thread %d i=%d already set\n", threadIdx, i);
            continue;
        } else { // new set
            sqlconn = SqlConVector.operator[](i);
            find = true;
            //printf("thread %d, find insert stmt %d\n", threadIdx, i);
            break;
        }
    }
    if (!find) { // build a new connection
        printf("thread %d need to build a new connection\n", threadIdx);
        sqlconn = new SQLCONN();
        sqlconn->sqlconnection = driver->connect("tcp://127.0.0.1:3306", "root", "greeN7^Tr33");
        /* Connect to the MySQL test database */
        sqlconn->sqlconnection->setSchema("end2end");
        sqlconn->pstmtInsert = sqlconn->sqlconnection->prepareStatement("INSERT INTO asyn2 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        sqlconn->pstmtDelete = sqlconn->sqlconnection->prepareStatement("DELETE FROM asyn2 WHERE ID = ?");
        SqlConVector.push_back(sqlconn);
        auto * ai = new atomic_flag; (*ai).test_and_set();
        SqlConAvaliable.push_back(ai);
        i = numSqlCons.operator int();
        numSqlCons.operator++();
        printf("thread %d, create new insert stmt %d\n", threadIdx, i+1);
    }
    sql::PreparedStatement* insertStmt = sqlconn->pstmtInsert;
    insertStmt->setInt(1, ToInsert);
    int Age_insert = RandomInt(1, 50);
    insertStmt->setInt(2, Age_insert);
    for (int k = 3; k <= 10; ++k) {
        int thisLen = RandomInt(1, STRING_SIZE);
        insertStmt->setString(k, RandomString(thisLen));
    }
    //printf("thread %d before execute\n", threadIdx);
    //printf("thread %d insert %d\n",threadIdx, ToInsert);
    insertStmt->execute();
    //printf("thread %d after execute\n", threadIdx);
    TimeVar time2 = timeNow();
    long long t = duration(time2 - time1);
    if (insertindex >= 0) {
        TinsertDB[insertindex] = t;
        //printf("TinsertDB[%d] = %lld\n", insertindex, t);
    }
    (*(SqlConAvaliable[i])).clear();
    //printf("thread %d finishes\n", threadIdx);
}

void deleteFromDB(int IDtoDelete, int deleteindex, int threadIdx) {
    TimeVar time1 = timeNow();
    SQLCONN * sqlconn;
    bool find = false;
    int i = 0;
    for (; i < numSqlCons; ++i) {
        if ((*(SqlConAvaliable[i])).test_and_set()) { // already set
            //printf("thread %d i=%d already set\n", threadIdx, i);
            continue;
        } else { // new set
            sqlconn = SqlConVector.operator[](i);
            find = true;
            //printf("thread %d, find delete stmt %d\n", threadIdx, i);
            break;
        }
    }
    if (!find) { // build a new connection
        printf("thread %d need to build a new connection\n", threadIdx);
        sqlconn = new SQLCONN();
        sqlconn->sqlconnection = driver->connect("tcp://127.0.0.1:3306", "root", "greeN7^Tr33");
        /* Connect to the MySQL test database */
        sqlconn->sqlconnection->setSchema("end2end");
        sqlconn->pstmtInsert = sqlconn->sqlconnection->prepareStatement("INSERT INTO asyn2 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        sqlconn->pstmtDelete = sqlconn->sqlconnection->prepareStatement("DELETE FROM asyn2 WHERE ID = ?");
        SqlConVector.push_back(sqlconn);
        auto * ai = new atomic_flag; (*ai).test_and_set();
        SqlConAvaliable.push_back(ai);
        i = numSqlCons.operator int();
        numSqlCons.operator++();
        printf("thread %d, create new delete stmt %d\n", threadIdx, i+1);
    }
    sql::PreparedStatement* deleteStmt = sqlconn->pstmtDelete;
    deleteStmt->setInt(1, IDtoDelete);
    //printf("thread %d before execute\n", threadIdx);
    //printf("thread %d delete %d\n",threadIdx, IDtoDelete);
    deleteStmt->execute();
    //printf("thread %d after execute\n", threadIdx);
    TimeVar time2 = timeNow();
    long long t = duration(time2 - time1);
    if (deleteindex >= 0) {
        TdeleteDB[deleteindex] = t;
        //printf("TdeleteDB[%d] = %lld\n", deleteindex, t);
    }
    (*(SqlConAvaliable[i])).clear();
    //printf("thread %d finishes\n", threadIdx);
}

void simulateUserResponseTime() {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::default_random_engine generator (seed);

    std::normal_distribution<double> distribution (1000.0,10.0);

    int wait = distribution(generator);
    while (wait < 500) {
        wait = distribution(generator);
    }
    if (wait > 1500) {
        wait = 1500;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(wait));
}

int main() {

    try {
        vector<thread> threads;
        numSqlCons.operator=(10);
        int threadIdx = 1;
        /* Create a connection */
        driver = get_driver_instance();

        for (int l = 0; l < numSqlCons; ++l) {
            auto * sqlconn = new SQLCONN();
            sqlconn->sqlconnection = driver->connect("tcp://127.0.0.1:3306", "root", "greeN7^Tr33");
            /* Connect to the MySQL test database */
            sqlconn->sqlconnection->setSchema("end2end");
            sqlconn->pstmtInsert = sqlconn->sqlconnection->prepareStatement("INSERT INTO asyn2 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            sqlconn->pstmtDelete = sqlconn->sqlconnection->prepareStatement("DELETE FROM asyn2 WHERE ID = ?");
            SqlConVector.push_back(sqlconn);
            auto * ai = new atomic_flag; (*ai).clear();
            SqlConAvaliable.push_back(ai);
        }
        printf("size of SqlConVector = %lu\n", SqlConVector.size());

        //var time1, time2 time.Time
        string filepath[3] = {"asyn2.csv", "asyn2Log.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);
        finstant<<" ,Tinsert,TinsertDB+Tinsert,Tdelete,TdeleteDB+Tdelete"<<endl;


        printf("# of operations = %d\n", operations);

        TimeVar time1, time2, time3;

        int NowTotalNum = iniNum;
        int ToInsert = iniNum + 1;
        int *array = new int[iniNum];
        for (int i = 0; i < iniNum; ++i) {
            array[i] = i + 1;
        }

        DynamicArray *da = NewDynamicArray(array, iniNum, danodesize);
        delete[]array;
        array = nullptr;
        int numUpdate = 0;
        int depth = 1;
        for (int lt = 0; lt < operations; lt++) {
            if (lt % 100 == 0) {
                cout << "lt = " << lt;
                cout << "da depth = " << da->Depth() << endl;
                flog << "lt = " << lt << endl;
                flog << "da depth = " << da->Depth() << endl;
            }
            if (da->Depth() > depth) {
                depth++;
                flog << "numUpdate = " << numUpdate << " da depth = " << depth << endl;
            }

            time1 = timeNow();
            int pos = RandomInt(1, NowTotalNum);
            da->Insert(ToInsert, pos);
            ToInsert++;
            NowTotalNum++;
            threads.emplace_back(insertToDB, ToInsert, TinsertIdx, threadIdx);
            threadIdx++;
            time2 = timeNow();
            Tinsert[TinsertIdx] = duration(time2 - time1);
            TinsertIdx++;
            simulateUserResponseTime();

            time1 = timeNow();
            pos = RandomInt(1, NowTotalNum);
            int IDtoDelete = da->Query(pos);
            NowTotalNum--;
            da->Delete(pos);
            threads.emplace_back(deleteFromDB, IDtoDelete, TdeleteIdx, threadIdx);
            threadIdx++;
            time2 = timeNow();
            Tdelete[TdeleteIdx] = duration(time2 - time1);
            TdeleteIdx++;
            simulateUserResponseTime();


        } //for lt <= loopTime
        printf("finish lt loop, threads size = %lu\n", threads.size());
        vector<thread>::iterator it;
        for (it = threads.begin(); it != threads.end(); ++it) {
            it->join();
        }
        for (int idx = 0; idx < operations; ++idx) {
            cout << "," << Tinsert[idx] << ","<<Tinsert[idx]+TinsertDB[idx]
                 <<","<<Tdelete[idx] << "," << Tdelete[idx] + TdeleteDB[idx] << endl;
            idx++;
        }
        flog.close();
        finstant.close();
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    } catch (std::exception& e){
        cout<<"running task, with exception..."<<e.what()<<endl;
    }
    return 0;
}

