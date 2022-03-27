/*
 * This is uses 1M dataset, 1M operations
 * always sleep 500 ms
 * about ?? hours
 * danodesize = 100
 * detach threads in insert/delete within the lt loop
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
#include "../../../../DataStructures/CountedBtree.h"
#include "../../../../DataStructures/StandardArray.h"
#include "../../../../DataStructures/DynamicArray.h"
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
const int operations = 1000000;
const int iniNum = 1000000;
const int danodesize = 50;
int TinsertIdx = 0, TdeleteIdx = 0;
long long TinsertDB[operations+1000], TdeleteDB[operations+1000],
Tinsert1[operations+1000], Tdelete1[operations+1000], Tinsert2[operations+1000], Tdelete2[operations+1000];


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
    //printf("numSqlConn = %d\n", numSqlCons.operator int());
    SQLCONN * sqlconn;
    bool find = false;
    int i = 0;
    while (!find) {
        for (; i < numSqlCons; ++i) {
            atomic_flag * af = SqlConAvaliable.operator[](i);
            if ((*af).test_and_set()) { // already set
                //printf("thread %d i=%d already set\n", threadIdx, i);
                continue;
            } else { // new set
                sqlconn = SqlConVector.operator[](i);
                find = true;
                //printf("thread %d, find insert stmt %d\n", threadIdx, i);
                break;
            }
        }
    }
    sql::PreparedStatement* insertStmt = sqlconn->pstmtInsert;
    TimeVar time1 = timeNow();
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
    }
    (*(SqlConAvaliable[i])).clear();
    //printf("thread %d finishes\n", threadIdx);
}

void deleteFromDB(int IDtoDelete, int deleteindex, int threadIdx) {
    //printf("numSqlConn = %d\n", numSqlCons.operator int());
    SQLCONN * sqlconn;
    bool find = false;
    int i = 0;
    while (!find) {
        for (; i < numSqlCons; ++i) {
            atomic_flag * af = SqlConAvaliable.operator[](i);
            if ((*af).test_and_set()) { // already set
                //printf("thread %d i=%d already set\n", threadIdx, i);
                continue;
            } else { // new set
                sqlconn = SqlConVector.operator[](i);
                find = true;
                //printf("thread %d, find delete stmt %d\n", threadIdx, i);
                break;
            }
        }
    }
    sql::PreparedStatement* deleteStmt = sqlconn->pstmtDelete;
    TimeVar time1 = timeNow();
    deleteStmt->setInt(1, IDtoDelete);
    //printf("thread %d before execute\n", threadIdx);
    //printf("thread %d delete %d\n",threadIdx, IDtoDelete);
    deleteStmt->execute();
    //printf("thread %d after execute\n", threadIdx);
    TimeVar time2 = timeNow();
    long long t = duration(time2 - time1);
    if (deleteindex >= 0) {
        TdeleteDB[deleteindex] = t;
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
        numSqlCons.operator=(100);
        int threadIdx = 1;
        /* Create a connection */
        driver = get_driver_instance();

        for (int l = 0; l < numSqlCons; ++l) {
            auto * sqlconn = new SQLCONN();
            sqlconn->sqlconnection = driver->connect("tcp://127.0.0.1:3306", "root", "f7npYEUEd!");
            /* Connect to the MySQL test database */
            sqlconn->sqlconnection->setSchema("end2end");
            sqlconn->pstmtInsert = sqlconn->sqlconnection->prepareStatement("INSERT INTO asyn1M VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
            sqlconn->pstmtDelete = sqlconn->sqlconnection->prepareStatement("DELETE FROM asyn1M WHERE ID = ?");
            SqlConVector.push_back(sqlconn);
            auto * ai = new atomic_flag; (*ai).clear();
            SqlConAvaliable.push_back(ai);
        }
        printf("size of SqlConVector = %lu\n", SqlConVector.size());

        //var time1, time2 time.Time
        string filepath[3] = {"asyn1M.csv", "asyn1MLog.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);
        finstant<<" ,Tda,Tlaunchthread,Tinsert,TinsertDB+Tinsert,Tda,Tlaunchthread,Tdelete,TdeleteDB+Tdelete"<<endl;


        int InsertActions = operations * 25 / 100;
        int DeleteActions = operations * 25 / 100;
        int ReorderActions = operations * 25 / 100;
        int SwapActions = operations * 25 / 100;
        int TotalActions = DeleteActions + InsertActions + ReorderActions + SwapActions ;
        printf("%d, %d, %d, %d\n", InsertActions, DeleteActions, ReorderActions, SwapActions);
        printf("# of operations = %d\n", operations);

        int *a = new int[TotalActions];
        int ua = 0;
        for (int y = 0; y < ReorderActions; y++) {
            a[ua] = 4;
            ua++;
        }
        for (int y = 0; y < SwapActions; y++) {
            a[ua] = 5;
            ua++;
        }
        for (int y = 0; y < InsertActions; y++) {
            a[ua] = 2;
            ua++;
        }
        for (int y = 0; y < DeleteActions; y++) {
            a[ua] = 3;
            ua++;
        }

        shuffle(a, a + TotalActions, generator());
        int *reorderrange = RangeDistributionRandom(ReorderActions, 1, 1000);
        int ir = 0;

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
        int depth = 1, NumDA;
        for (int lt = 0; lt < operations; lt++) {
            if (lt % 500 == 0) {
                cout << "lt = " << lt;
                cout << "da depth = " << da->Depth() << endl;
                flog << "lt = " << lt << endl;
                flog << "da depth = " << da->Depth() << endl;
            }
            if (da->Depth() > depth) {
                depth++;
                flog << "numUpdate = " << numUpdate << " da depth = " << depth << endl;
            }
            switch (a[lt]) {
                case 2: //insert
                {
                    int pos = RandomInt(1, NowTotalNum);
                    da->Insert(ToInsert, pos);
                    thread th1(insertToDB, ToInsert, -1, threadIdx);
                    th1.detach();
                    threadIdx++;
                    ToInsert++;
                    NowTotalNum++;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    //threads.emplace_back(thread(insertToDB, ToInsert));
                    //std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                case 3: //delete
                {
                    int pos = RandomInt(1, NowTotalNum);
                    int IDtoDelete = da->Delete(pos);
                    thread th2(deleteFromDB, IDtoDelete, -1, threadIdx);
                    th2.detach();
                    threadIdx++;
                    NowTotalNum--;
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    //threads.emplace_back(thread(deleteFromDB));
                    //std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }

                case 4: //reorder
                {
                    int len = reorderrange[ir];
                    ir++;
                    if (len >= NowTotalNum) {
                        len = NowTotalNum - 1;
                    }
                    int start = RandomInt(1, NowTotalNum - len);
                    int end = start + len - 1;
                    if (end >= NowTotalNum) {
                        end = NowTotalNum - 1;
                    }
                    int *oldArray = da->RangeQuery(start, end, &NumDA);
                    int *newArray = new int[len];
                    for (int j = 0; j < len; ++j) {
                        newArray[j] = oldArray[len - j - 1];
                    }

                    da->Reorder(start, end, newArray);
                    delete[]newArray;
                    delete[]oldArray;
                    break;
                }
                case 5: //swap
                {
                    int b[4] = {};
                    for (int &j : b) {
                        j = RandomInt(1, NowTotalNum);
                    }
                    sort(b, b + 4);
                    if (b[1] == b[2]) {
                        continue;
                    }
                    int start1 = b[0];
                    int end1 = b[1];
                    int start2 = b[2];
                    int end2 = b[3];
                    da->Swap(start1, end1, start2, end2);
                    break;
                }
            }
            if ((lt + 1 <= 10) || ((lt + 1 <= 100) && ((lt + 1) % 10 == 0)) ||
                ((lt + 1 <= 1000) && ((lt + 1) % 100 == 0))
                || ((lt + 1 <= 10000) && ((lt + 1) % 1000 == 0)) || ((lt + 1 <= 100000) && ((lt + 1) % 10000 == 0))
                || ((lt + 1 <= 1000000) && ((lt + 1) % 100000 == 0))) {
                double fl = (lt+1)*1.0/operations ;
                finstant << fl << ",";
                cout << fl << ",";
                int pos = RandomInt(1, NowTotalNum);
                time1 = timeNow();
                da->Insert(ToInsert, pos);
                time2 = timeNow();
                threads.emplace_back(insertToDB, ToInsert, TinsertIdx, threadIdx);
                time3 = timeNow();
                ToInsert++;
                NowTotalNum++;
                threadIdx++;
                Tinsert1[TinsertIdx] = duration(time2 - time1);
                Tinsert2[TinsertIdx] = duration(time3 - time2);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                finstant << Tinsert1[TinsertIdx] << "," << Tinsert2[TinsertIdx] <<","<< Tinsert1[TinsertIdx]+Tinsert2[TinsertIdx] << ","<<Tinsert1[TinsertIdx]+Tinsert2[TinsertIdx]+TinsertDB[TinsertIdx];
                cout  << Tinsert1[TinsertIdx] << "," << Tinsert2[TinsertIdx] <<"," << Tinsert1[TinsertIdx]+Tinsert2[TinsertIdx] << ","<<Tinsert1[TinsertIdx]+Tinsert2[TinsertIdx]+TinsertDB[TinsertIdx];
                TinsertIdx++;


                pos = RandomInt(1, NowTotalNum);
                time1 = timeNow();
                int IDtoDelete = da->Delete(pos);
                time2 = timeNow();
                threads.emplace_back(deleteFromDB, IDtoDelete, TdeleteIdx, threadIdx);
                time3 = timeNow();
                NowTotalNum--;
                threadIdx++;
                Tdelete1[TdeleteIdx] = duration(time2 - time1);
                Tdelete2[TdeleteIdx] = duration(time3 - time2);
                std::this_thread::sleep_for(std::chrono::milliseconds(500));
                finstant <<"," << Tdelete1[TdeleteIdx] << "," << Tdelete2[TdeleteIdx] << ","<<Tdelete1[TdeleteIdx]+Tdelete2[TdeleteIdx] << "," << Tdelete1[TdeleteIdx] +Tdelete2[TdeleteIdx]+ TdeleteDB[TdeleteIdx] << endl;
                cout <<"," << Tdelete1[TdeleteIdx] << "," << Tdelete2[TdeleteIdx] <<","<<Tdelete1[TdeleteIdx]+Tdelete2[TdeleteIdx] << "," << Tdelete1[TdeleteIdx]+Tdelete2[TdeleteIdx] + TdeleteDB[TdeleteIdx] << endl;
                TdeleteIdx++;
                numUpdate += 2;
            }
        } //for lt <= loopTime
        printf("finish lt loop, threads size = %lu\n", threads.size());
        vector<thread>::iterator it;
        for (it = threads.begin(); it != threads.end(); ++it) {
            it->join();
        }
        finstant<<endl<<endl;
        cout<<endl<<endl;
        int idx = 0;
        for (int lt = 0; lt < TotalActions; lt++) {
            if ((lt + 1 <= 10)) {
                double fl = (lt + 1) * 1.0 / operations;
                finstant << fl <<"," << Tdelete1[idx] << "," << Tdelete2[idx] << ","<<Tdelete1[idx]+Tdelete2[idx] << "," << Tdelete1[idx] +Tdelete2[idx]+ TdeleteDB[idx] << endl;
                cout << fl <<"," << Tdelete1[idx] << "," << Tdelete2[idx] <<","<<Tdelete1[idx]+Tdelete2[idx] << "," << Tdelete1[idx]+Tdelete2[idx] + TdeleteDB[idx] << endl;
                idx++;
            }
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

