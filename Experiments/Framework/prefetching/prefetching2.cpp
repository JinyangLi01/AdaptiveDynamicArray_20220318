/*
 * operations = 10,000
 * sleep for 500-1000ms
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

sql::Driver *driver;
struct SQLCONN {
public:
    sql::Connection* sqlconnection{};
    sql::PreparedStatement *pstmtQuery{};

};
sql::ResultSet *res0;
sql::ResultSet *res1;
const int iniNum = 100000; //0.03 = 30000
const int prewindow = 30;
const int danodesize = 50;
const int operations = 100000;
vector<SQLCONN*> SqlConVector;
vector<atomic_flag*> SqlConAvaliable;
atomic_int numSqlCons;
atomic_flag available0, available1;


class MysqlRow {
public:
    int ID{}, Age{};
    string Name, c4, c5, c6, c7, c8, c9, c10;
};


void FetchSqlRes0(MysqlRow * rows) {
    int i = 0;
    while (res0->next()) {
        // You can use either numeric offsets...
        rows[i].ID = res0->getInt(1);
        rows[i].Age = res0->getInt(2);
        rows[i].Name = res0->getString(3);
        rows[i].c4 = res0->getString(4);
        rows[i].c5 = res0->getString(5);
        rows[i].c6 = res0->getString(6);
        rows[i].c7 = res0->getString(7);
        rows[i].c8 = res0->getString(8);
        rows[i].c9 = res0->getString(9);
        rows[i].c10 = res0->getString(10);
        i++;
        //cout << "id = " << res->getInt(1); // getInt(1) returns the first column
        // ... or column names for accessing results.
        // The latter is recommended.
        //cout << ", label = '" << res->getString("label") << "'" << endl;
    }
}

void FetchSqlRes1(MysqlRow * rows) {
    int i = 0;
    while (res1->next()) {
        // You can use either numeric offsets...
        rows[i].ID = res1->getInt(1);
        rows[i].Age = res1->getInt(2);
        rows[i].Name = res1->getString(3);
        rows[i].c4 = res1->getString(4);
        rows[i].c5 = res1->getString(5);
        rows[i].c6 = res1->getString(6);
        rows[i].c7 = res1->getString(7);
        rows[i].c8 = res1->getString(8);
        rows[i].c9 = res1->getString(9);
        rows[i].c10 = res1->getString(10);
        i++;
        //cout << "id = " << res->getInt(1); // getInt(1) returns the first column
        // ... or column names for accessing results.
        // The latter is recommended.
        //cout << ", label = '" << res->getString("label") << "'" << endl;
    }
}

long long Tdb = 0;
long long Thisdb = 0;

void QueryDB0(MysqlRow * rows0, int * IDs, int num) {
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
    Tdb += Thisdb;
    (*(SqlConAvaliable[i])).clear();
}

void QueryDB1(MysqlRow * rows1, int * IDs, int num) {
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
    res1 = queryStmt->executeQuery();
    FetchSqlRes1(rows1);
    available1.test_and_set();
    TimeVar timeb = timeNow();
    Thisdb = duration(timeb - timea);
    Tdb += Thisdb;
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


int main() {

    try {
        vector<thread> threads;
        numSqlCons.operator=(100);
        //int threadIdx = 1;

        /* Create a connection */
        driver = get_driver_instance();
        sql::Connection* con = driver->connect("tcp://127.0.0.1:3306", "root", "greeN7^Tr33");
        /* Connect to the MySQL test database */
        con->setSchema("end2end");


        sql::PreparedStatement* pstmtInsert = con->prepareStatement("INSERT INTO prefetching2 VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        sql::PreparedStatement* pstmtDelete = con->prepareStatement("DELETE FROM prefetching2 WHERE ID = ?");

        string queryString = "SELECT * FROM prefetching2 WHERE ID in (?";
        for (int l = 1; l < 5*prewindow; ++l) {
            queryString += ", ?";
        }
        queryString += ")";

        for (int l = 0; l < numSqlCons; ++l) {
            auto * sqlconn = new SQLCONN();
            sqlconn->sqlconnection = driver->connect("tcp://127.0.0.1:3306", "root", "greeN7^Tr33");
            /* Connect to the MySQL test database */
            sqlconn->sqlconnection->setSchema("end2end");
            sqlconn->pstmtQuery = sqlconn->sqlconnection->prepareStatement(queryString);
            SqlConVector.push_back(sqlconn);
            auto * ai = new atomic_flag; (*ai).clear();
            SqlConAvaliable.push_back(ai);
        }
        printf("size of SqlConVector = %lu\n", SqlConVector.size());

        //var time1, time2 time.Time
        string filepath[3] = {"prefetching2.csv", "prefetching2Log.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);
        finstant<<" ,Tquery,TqueryDB"<<endl;


        int InsertActions = operations * 2 / 10;
        int DeleteActions = operations * 2 / 10;
        int ReorderActions = operations * 2 / 10;
        int SwapActions = operations * 2 / 10;
        int MoveActions = operations * 2 / 10;
        int TotalActions = DeleteActions + InsertActions + ReorderActions + SwapActions + MoveActions; // 550 000
        printf("%d, %d, %d, %d, %d\n", InsertActions, DeleteActions, ReorderActions, SwapActions, MoveActions);
        printf("# of operations = %d\n", operations);

        auto * rows0 = new MysqlRow[5*prewindow];
        auto * rows1 = new MysqlRow[5*prewindow];


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

        //shuffle(a, a + TotalActions, generator());
        unsigned useed = std::chrono::system_clock::now().time_since_epoch().count();
        shuffle (a, a+ TotalActions, std::default_random_engine(useed));

        int *reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
        int ir = 0;
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
        int depth = 1, NumDA;
        for (int lt = 0; lt < TotalActions; lt++) {
            //printf("lt = %d\n", lt);
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
                    ToInsert++;
                    NowTotalNum++;

                    pstmtInsert->setInt(1, ToInsert);
                    int Age_insert = RandomInt(1, 50);
                    pstmtInsert->setInt(2, Age_insert);
                    for (int k = 3; k <= 10; ++k) {
                        int thisLen = RandomInt(1, STRING_SIZE);
                        pstmtInsert->setString(k, RandomString(thisLen));
                    }
                    pstmtInsert->execute();
                    break;
                }
                case 3: //delete
                {
                    int pos = RandomInt(1, NowTotalNum);
                    int IDtoDelete = da->Query(pos);
                    da->Delete(pos);
                    NowTotalNum--;
                    pstmtDelete->setInt(1, IDtoDelete);
                    pstmtDelete->execute();
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
                available0.clear();
                available1.clear();


                int pos = NowTotalNum/2;
                int winstart = pos - 14, winend = pos + 15;
                int bufferstart = winstart - 2*prewindow;
                int bufferend = winend + 2*prewindow;
                int num = 0;
                int* IDs = da->RangeQuery(bufferstart, bufferend, &num);

                thread th(QueryDB1, rows1, IDs, num);
                th.join();
                //printf("finish ini fetching\n");

                long long Tda = 0;
                Tdb = 0;

                for (int numPre = 0; numPre < 10; ++numPre) {
                    //printf("numPre = %d\n", numPre);
                    int randi = RandomInt(1, 100000);
                    if (randi % 2 == 0) { // scroll up
                        pos -= prewindow;
                        winstart = pos - 14, winend = pos + 15;
                        bufferstart = winstart - 2*prewindow;
                        bufferend = winend + 2*prewindow;
                        if (winstart < 1) {
                            pos += prewindow * 2;
                            winstart = pos - 14, winend = pos + 15;
                            bufferstart = winstart - 2*prewindow;
                            bufferend = winend + 2*prewindow;
                        }
                        if (bufferstart < 1) {
                            bufferstart = 1;
                        }
                    } else {//scroll down
                        pos += prewindow;
                        winstart = pos - 14, winend = pos + 15;
                        bufferstart = winstart - 2*prewindow;
                        bufferend = winend + 2*prewindow;
                        if (winend > NowTotalNum) {
                            pos -= prewindow * 2;
                            winstart = pos - 14, winend = pos + 15;
                            bufferstart = winstart - 2*prewindow;
                            bufferend = winend + 2*prewindow;
                        }
                        if (bufferend > NowTotalNum) {
                            bufferstart = NowTotalNum;
                        }
                    }

                    TimeVar timea = timeNow();
                    IDs = da->RangeQuery(bufferstart, bufferend, &num);
                    TimeVar timeb = timeNow();
                    Tda += duration(timeb - timea);

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
                            threads.emplace_back(QueryDB0, rows0, IDs, num);
                        } else {
                            threads.emplace_back(QueryDB1, rows1, IDs, num);
                        }
                        simulateUserResponseTime();
                    } else {
                        if (numPre % 2 == 0) {
                            time1 = timeNow();
                            thread th0(QueryDB0, rows0, IDs, num);
                            th0.join();
                            time2 = timeNow();
                            Tda += duration(time2-time1);
                        } else {
                            time1 = timeNow();
                            thread th1(QueryDB1, rows1, IDs, num);
                            th1.join();
                            time2 = timeNow();
                            Tda += duration(time2-time1);
                        }
                    }

                }
                double fl = (lt+1)*1.0/operations ;
                finstant << fl << ",";
                cout << fl << ",";

                finstant <<Tda/10 << ","<< (Tda + Tdb)/10 <<endl;
                cout <<Tda/10 << ","<< (Tda + Tdb)/10 <<endl;
            }

        } //for lt <= loopTime

        printf("finish lt loop, threads size = %lu\n", threads.size());
        int joinNum = 1;
        vector<thread>::iterator it;
        for (it = threads.begin(); it != threads.end(); ++it) {
            //printf("before joining th %d\n", joinNum);
            it->join();
            //printf("after joining th %d\n", joinNum);
            joinNum++;
        }
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

