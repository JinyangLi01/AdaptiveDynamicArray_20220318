/*
 * this is the final asyn file
 * cannot detach threads, otherwise the main thread finishes before others
 * sleep for 0.5-1s to simulate user response time
 * initial number of connections = 500
 * # of operations = 10000
 * about 1.1 hour
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

const int operations = 100000;
const int iniNum = 100000; //0.03 = 30000
const int danodesize = 50;


sql::Driver *driver;
struct SQLCONN {
public:
    sql::Connection* sqlconnection{};
    sql::PreparedStatement* pstmtInsert{};
    sql::PreparedStatement* pstmtDelete{};
};


int main() {

    try {
        /* Create a connection */
        driver = get_driver_instance();


        auto * sqlconn = new SQLCONN();
        sqlconn->sqlconnection = driver->connect("tcp://127.0.0.1:3306", "root", "greeN7^Tr33");
        /* Connect to the MySQL test database */
        sqlconn->sqlconnection->setSchema("end2end");
        sqlconn->pstmtInsert = sqlconn->sqlconnection->prepareStatement("INSERT INTO syn VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        sqlconn->pstmtDelete = sqlconn->sqlconnection->prepareStatement("DELETE FROM syn WHERE ID = ?");


        //var time1, time2 time.Time
        string filepath[3] = {"syn.csv", "synLog.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);
        finstant<<" ,Tinsert,TinsertDB+Tinsert,Tdelete,TdeleteDB+Tdelete"<<endl;


        int InsertActions = operations * 2 / 10;
        int DeleteActions = operations * 2 / 10;
        int ReorderActions = operations * 2 / 10;
        int SwapActions = operations * 2 / 10;
        int MoveActions = operations * 2 / 10;
        int TotalActions = DeleteActions + InsertActions + ReorderActions + SwapActions + MoveActions; // 550 000
        printf("%d, %d, %d, %d, %d\n", InsertActions, DeleteActions, ReorderActions, SwapActions, MoveActions);
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
        for (int y = 0; y < MoveActions; y++) {
            a[ua] = 6;
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
        int *reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
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
            switch (a[lt]) {
                case 2: //insert
                {
                    int pos = RandomInt(1, NowTotalNum);
                    da->Insert(ToInsert, pos);
                    sql::PreparedStatement* insertStmt = sqlconn->pstmtInsert;
                    insertStmt->setInt(1, ToInsert);
                    int Age_insert = RandomInt(1, 50);
                    insertStmt->setInt(2, Age_insert);
                    for (int k = 3; k <= 10; ++k) {
                        int thisLen = RandomInt(1, STRING_SIZE);
                        insertStmt->setString(k, RandomString(thisLen));
                    }
                    insertStmt->execute();
                    ToInsert++;
                    NowTotalNum++;
                    //threads.emplace_back(thread(insertToDB, ToInsert));
                    //std::this_thread::sleep_for(std::chrono::seconds(1));
                    break;
                }
                case 3: //delete
                {
                    int pos = RandomInt(1, NowTotalNum);
                    int IDtoDelete = da->Query(pos);
                    da->Delete(pos);
                    sql::PreparedStatement* deleteStmt = sqlconn->pstmtDelete;
                    deleteStmt->setInt(1, IDtoDelete);
                    deleteStmt->execute();
                    NowTotalNum--;
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
                case 6: //move
                {
                    int b[3] = {};
                    for (int &j : b) {
                        j = RandomInt(1, NowTotalNum);
                    }
                    sort(b, b + 3);
                    if (b[1] == b[2]) {
                        continue;
                    }
                    int start1, end1, start2;

                    if (RandomInt(1, 10) % 2 == 1) {
                        start1 = b[0];
                        end1 = b[1];
                        start2 = b[2];
                    } else {
                        start1 = b[1];
                        end1 = b[2];
                        start2 = b[0];
                    }
                    da->Move(start1, end1, start2);
                    break;
                }
            }
            if ((lt + 1 <= 10) || ((lt + 1 <= 100) && ((lt + 1) % 10 == 0)) ||
                ((lt + 1 <= 1000) && ((lt + 1) % 100 == 0))
                || ((lt + 1 <= 10000) && ((lt + 1) % 1000 == 0)) || ((lt + 1 <= 100000) && ((lt + 1) % 10000 == 0))
                || ((lt + 1 <= 1000000) && ((lt + 1) % 100000 == 0))) {
                double fl = (lt+1)*1.0/operations ;
                finstant << fl << ",";

                long long Tinsert, TinsertDB, Tdelete, TdeleteDB;


                int pos = RandomInt(1, NowTotalNum);
                time1 = timeNow();
                da->Insert(ToInsert, pos);
                time2 = timeNow();
                sql::PreparedStatement* insertStmt = sqlconn->pstmtInsert;
                insertStmt->setInt(1, ToInsert);
                int Age_insert = RandomInt(1, 50);
                insertStmt->setInt(2, Age_insert);
                for (int k = 3; k <= 10; ++k) {
                    int thisLen = RandomInt(1, STRING_SIZE);
                    insertStmt->setString(k, RandomString(thisLen));
                }
                insertStmt->execute();
                time3 = timeNow();
                ToInsert++;
                NowTotalNum++;

                Tinsert = duration(time2 - time1);
                TinsertDB = duration(time3 - time1);


                pos = RandomInt(1, NowTotalNum);
                time1 = timeNow();
                int IDtoDelete = da->Delete(pos);
                time2 = timeNow();
                sql::PreparedStatement* deleteStmt = sqlconn->pstmtDelete;
                deleteStmt->setInt(1, IDtoDelete);
                deleteStmt->execute();
                time3 = timeNow();
                Tdelete = duration(time2 - time1);
                TdeleteDB = duration(time3 - time1);
                NowTotalNum--;

                numUpdate += 2;

                finstant << fl << "," << Tinsert << ","<<Tinsert+TinsertDB
                         <<","<<Tdelete << "," << Tdelete + TdeleteDB << endl;
                cout << fl << "," << Tinsert << ","<<Tinsert+TinsertDB
                     <<","<<Tdelete << "," << Tdelete + TdeleteDB << endl;
            }
        } //for lt <= loopTime
        delete da;
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

