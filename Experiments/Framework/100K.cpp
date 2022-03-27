/*
 * # items = 100,000
 * # operations = 100,000
 *
 */
#include <iostream>
#include <fstream>
#include <chrono>
#include <random>
#include <algorithm>
#include "../../DataStructures/CountedBtree.h"
#include "../../DataStructures/StandardArray.h"
#include "../../DataStructures/LinkedList.h"
#include <cstdio>
#include <unistd.h>
#include <cstdlib>
#include <cstring>
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

char* RandomString(const int len) {
    static const char alphanum[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
    char * s = new char[STRING_SIZE];
    for (int i = 0; i < len; ++i) {
        s[i] = alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    s[len] = 0;
}

int main() {

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::Statement *stmt;
        sql::ResultSet *res;
        sql::PreparedStatement *pstmtInsert;
        sql::PreparedStatement *pstmtDelete;
        sql::PreparedStatement *pstmtQuery;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("tcp://127.0.0.1:3306", "root", "greeN7^Tr33");
        /* Connect to the MySQL test database */
        con->setSchema("end2end");


        pstmtInsert = con->prepareStatement("INSERT INTO 100K VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?)");
        pstmtDelete = con->prepareStatement("DELETE FROM 100K WHERE ID = ?");
        string queryString = "SELECT * FROM 100K WHERE ID in (?";
        for (int l = 1; l < 5000; ++l) {
            queryString += ", ?";
        }
        queryString += ")";
        pstmtQuery = con->prepareStatement(queryString);

        //var time1, time2 time.Time
        string filepath[3] = {"100K.csv", "100KLog.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);


        int iniNum = 1000000; //0.03 = 30000
        int NumOfOutput = 10;
        int operations = 100000;
        int InsertActions = operations;
        int DeleteActions = operations;
        int QueryAction = operations;
        int TotalActions = DeleteActions + InsertActions + QueryAction ; // 550 000
        int ComputeInterval = TotalActions / NumOfOutput; // 10%
        int Interval100 = TotalActions / 100;
        printf("%d, %d, %d\n", InsertActions, DeleteActions, QueryAction);
        printf("# of operations = %d\n", operations);
        int CurOutputNum = 0;

        int *a = new int[TotalActions];
        int ua = 0;
        for (int y = 0; y < QueryAction; y++) {
            a[ua] = 1;
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
        //random_shuffle(&a[DeleteActions+InsertActions], &a[TotalActions]);
        //random_shuffle(&a[0], &a[MoveActions+SwapActions+ReorderActions]);
        //random_shuffle(&a[0], &a[operations]);
        //int * shortqueryrange = RangeDistributionRandom(, 1, 100);

        TimeVar time1, time2;
        long long Tinsert = 0, Tdelete = 0, Tquery = 0;
        int NowTotalNum = iniNum;
        int ToInsert = iniNum + 1;
        int *array = new int[iniNum];
        for (int i = 0; i < iniNum; ++i) {
            array[i] = i + 1;
        }
        StandardArray *sa = NewStandardArray(array, iniNum);
        delete[]array;
        int NumSA, NumDA, Numll, NumCBT;
        //da->PrintTree();
        int depth = 1;
        int numUpdate = 0;
        for (int lt = 0; lt < TotalActions; lt++) {
            if (lt % 5000 == 0) {
                cout << "lt = " << lt;
                flog << "lt = " << lt << endl;
            }
            switch (a[lt]) {
                case 1: // query
                {
                    int start1 = 30000, end1 = 35000;
                    int num = 0;
                    int * ans = sa->RangeQuery(start1, end1, &num);
                    time1 = timeNow();
                    for (int k = 1; k <= 5000; ++k) {
                        pstmtQuery->setInt(k, ans[k-1]);
                    }
                    pstmtQuery->execute();
                    time2 = timeNow();
                    Tquery += duration(time2-time1);
                }
                case 2: //insert
                {
                    int pos = RandomInt(1, NowTotalNum);
                    ToInsert++;
                    NowTotalNum++;

                    time1 = timeNow();
                    pstmtInsert->setInt(1, ToInsert);
                    int Age_insert = RandomInt(1, 50);
                    pstmtInsert->setInt(2, Age_insert);
                    for (int k = 3; k <= 10; ++k) {
                        int thisLen = RandomInt(1, STRING_SIZE);
                        pstmtInsert->setString(k, RandomString(thisLen));
                    }
                    pstmtInsert->execute();
                    time2 = timeNow();
                    Tinsert += duration(time2-time1);
                    break;
                }
                case 3: //delete
                {
                    time1 = timeNow();
                    int pos = RandomInt(1, NowTotalNum);
                    int IDtoDelete = sa->Query(pos);
                    NowTotalNum--;
                    pstmtDelete->setInt(1, IDtoDelete);
                    pstmtDelete->execute();
                    time2 = timeNow();
                    Tdelete += duration(time2-time1);
                    break;
                }

            }

            numUpdate++;
        } //for lt <= loopTime

        delete[]a;
        flog.close();
        finstant.close();
        delete pstmtDelete;
        delete pstmtInsert;
        delete con;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    return 0;
}

