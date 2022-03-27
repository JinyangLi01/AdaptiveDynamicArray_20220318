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
#include "../../../DataStructures/CountedBtree.h"
#include "../../../DataStructures/StandardArray.h"
#include "../../../DA_Bidirectional/DAbidirectional.h"
#include "../../../DataStructures/LinkedList.h"
#include <stdio.h>
#include <mysql.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
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
#define DROP_SAMPLE_TABLE "DROP TABLE IF EXISTS test_table"
#define CREATE_SAMPLE_TABLE "CREATE TABLE test_table(col1 INT,\
                                                 col2 VARCHAR(40),\
                                                 col3 SMALLINT,\
                                                 col4 TIMESTAMP)"
#define INSERT_SAMPLE "INSERT INTO \
                       delete100 VALUES (?,?,?,?,?,?,?,?,?,?)"

#define DELETE_SAMPLE "DELETE FROM delete100 WHERE ID = ?"

using namespace std;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
bool CompareArray(const int *a, const int *b, int len) {
    if (a == nullptr || b == nullptr) {
        cout<<"null pointer error!" << endl;
        return 0 ;
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
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return s;
}

int main() {
    MYSQL         *mysql;
    MYSQL_STMT    *insertStmt;
    MYSQL_STMT    *deleteStmt;
    MYSQL_BIND    bindInsert[10];
    MYSQL_BIND    bindDelete[1];
    my_ulonglong  affected_rows;
    int           param_count_insert;
    int           param_count_delete;
    short         small_data;
    int           ID_insert;
    int           ID_delete;
    int           Age_insert;
    char          str_data[11][STRING_SIZE];
    unsigned long str_length[11];
    my_bool       is_null;

    mysql = mysql_init(NULL);
    mysql_real_connect(mysql, "localhost", "root", "greeN7^Tr33", "end2end",0,NULL,0);
    insertStmt = mysql_stmt_init(mysql);
    deleteStmt = mysql_stmt_init(mysql);
    if (!(insertStmt && deleteStmt))
    {
        fprintf(stderr, " mysql_stmt_init(), out of memory\n");
        exit(0);
    }
    if (mysql_stmt_prepare(insertStmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))
    {
        fprintf(stderr, " mysql_stmt_prepare(), INSERT failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(insertStmt));
        exit(0);
    }
    if (mysql_stmt_prepare(deleteStmt, DELETE_SAMPLE, strlen(DELETE_SAMPLE)))
    {
        fprintf(stderr, " mysql_stmt_prepare(), DELETE failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(deleteStmt));
        exit(0);
    }
    fprintf(stdout, " prepare, INSERT DELETE successful\n");
/* Get the parameter count from the statement */
    param_count_insert = mysql_stmt_param_count(insertStmt);
    fprintf(stdout, " total parameters in INSERT: %d\n", param_count_insert);
    if (param_count_insert != 10) /* validate parameter count */
    {
        fprintf(stderr, " invalid insert parameter count returned by MySQL\n");
        exit(0);
    }
    param_count_delete = mysql_stmt_param_count(deleteStmt);
    fprintf(stdout, " total parameters in DELETE: %d\n", param_count_delete);
    if (param_count_delete != 1) /* validate parameter count */
    {
        fprintf(stderr, " invalid delete parameter count returned by MySQL\n");
        exit(0);
    }
/* Bind the data for all 3 parameters */
    memset(bindInsert, 0, sizeof(bindInsert));
    memset(bindDelete, 0, sizeof(bindDelete));
/* INTEGER PARAM */
/* This is a number type, so there is no need
   to specify buffer_length */
    bindInsert[0].buffer_type= MYSQL_TYPE_LONG;
    bindInsert[0].buffer= (char *)&ID_insert;
    bindInsert[0].is_null= 0;
    bindInsert[0].length= 0;

    bindInsert[1].buffer_type= MYSQL_TYPE_LONG;
    bindInsert[1].buffer= (char *)&Age_insert;
    bindInsert[1].is_null= 0;
    bindInsert[1].length= 0;


/* STRING PARAM */
    for (int k = 2; k < 10 ; ++k) {
        bindInsert[k].buffer_type= MYSQL_TYPE_STRING;
        bindInsert[k].buffer= (char *)str_data[k];
        bindInsert[k].buffer_length= STRING_SIZE;
        bindInsert[k].is_null= 0;
        bindInsert[k].length = &(str_length[k]);
    }

    bindDelete[0].buffer_type= MYSQL_TYPE_LONG;
    bindDelete[0].buffer= (char *)&ID_delete;
    bindDelete[0].is_null= 0;
    bindDelete[0].length= 0;

/* Bind the buffers */
    if (mysql_stmt_bind_param(insertStmt, bindInsert))
    {
        fprintf(stderr, " mysql_stmt_bind_param() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(insertStmt));
        exit(0);
    }
    if (mysql_stmt_bind_param(deleteStmt, bindDelete))
    {
        fprintf(stderr, " mysql_stmt_bind_param() failed\n");
        fprintf(stderr, " %s\n", mysql_stmt_error(deleteStmt));
        exit(0);
    }

    //var time1, time2 time.Time
    string filepath[3] = {"delete.csv", "deleteLog.txt"};
    ofstream finstant, flog, ffinal;
    finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
    flog.open(filepath[1], ios::out | ios::in | ios::trunc);
    //ffinal.open(filepath[2], ios::out | ios::in | ios::trunc);
    finstant<<" ,DA,SA,LL,CBT"<<endl;

    int iniNum = 100000; //0.03 = 30000
    int danodesize = 50;
    int CbtOrder = 50;
    int m = 500;  //for linked list
    int NumOfOutput = 10;
    int operations = 100000;
    int InsertActions = operations * 2 / 10;
    int DeleteActions = operations * 2 / 10;
    int ReorderActions = operations * 2 / 10;
    int SwapActions = operations * 2 / 10;
    int MoveActions = operations * 2 / 10;
    int TotalActions = DeleteActions + InsertActions + ReorderActions+SwapActions + MoveActions; // 550 000
    int ComputeInterval = TotalActions / NumOfOutput; // 10%
    int Interval100 = TotalActions / 100;
    printf("%d, %d, %d, %d, %d\n", InsertActions, DeleteActions, ReorderActions, SwapActions, MoveActions);
    printf("# of operations = %d\n", operations);
    int CurOutputNum = 0;

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
    //random_shuffle(&a[DeleteActions+InsertActions], &a[TotalActions]);
    //random_shuffle(&a[0], &a[MoveActions+SwapActions+ReorderActions]);
    //random_shuffle(&a[0], &a[operations]);
    //int * shortqueryrange = RangeDistributionRandom(, 1, 100);
    int * reorderrange = RangeDistributionRandom(ReorderActions, 1, 100);
    int ir = 0;

    TimeVar time1, time2;

    int NowTotalNum = iniNum;
    int ToInsert = iniNum + 1;
    int * array = new int[iniNum];
    for (int i = 0; i < iniNum; ++i) {
        array[i] = i+1;
    }
    DAbidirectional *da = NewDynamicArray(array, iniNum, danodesize);
    StandardArray *sa = NewStandardArray(array, iniNum);
    CountedBtree * cbt = NewCBTreeForArray(CbtOrder, iniNum);
    LinkedList * ll = NewLinkedListForArray(m, array, iniNum);
    //ll->PrintStructure();
    delete []array;
    int NumSA, NumDA, Numll, NumCBT;
    //da->PrintTree();
    cout<<"da depth = "<<da->Depth() << endl;
    int depth = 1;
    int numUpdate = 0;
    for (int lt = 0; lt < TotalActions; lt++) {
        if (lt % 5000 == 0) {
            cout<<"lt = "<<lt;
            cout<<"da depth = "<<da->Depth() << endl;
            flog<<"lt = "<<lt<<endl;
            flog<<"da depth = "<<da->Depth() << endl;
        }
        if (da->Depth() > depth) {
            depth++;
            flog<<"numUpdate = "<<numUpdate<<" da depth = "<<depth<<endl;
        }
        switch (a[lt]) {

            case 2: //insert
            {
                int pos = RandomInt(1, NowTotalNum);
                sa->Insert(ToInsert, pos);
                da->Insert(ToInsert, pos);
                cbt->Insert(ToInsert, pos);
                ll->Insert(ToInsert, pos);
                ToInsert++;
                NowTotalNum++;

                /* Specify the data values for the first row */
                ID_insert = ToInsert;             /* integer */
                Age_insert = RandomInt(1, 50);
                for (int k = 2; k < 10; ++k) {
                    int thisLen = RandomInt(1, STRING_SIZE);
                    strncpy(str_data[k], RandomString(thisLen), thisLen); /* string  */
                    str_length[k] = thisLen;
                }
                /* Execute the INSERT statement - 1*/
                if (mysql_stmt_execute(insertStmt))
                {
                    fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
                    fprintf(stderr, " %s\n", mysql_stmt_error(insertStmt));
                    exit(0);
                }
                /*
                for (int k = 2; k < 10; ++k) {
                    delete []str_data[k];
                }
                */
                break;
            }
            case 3: //delete
            {
                int pos = RandomInt(1, NowTotalNum);
                int IDtoDelete = sa->Query(pos);
                sa->Delete(pos);
                da->Delete(pos);
                cbt->Delete(pos);
                ll->Delete(pos);
                NowTotalNum--;
                ID_delete = IDtoDelete;
                /* Execute the INSERT statement - 1*/
                if (mysql_stmt_execute(deleteStmt))
                {
                    fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
                    fprintf(stderr, " %s\n", mysql_stmt_error(deleteStmt));
                    exit(0);
                }
                break;
            }

            case 4: //reorder
            {
                int len = reorderrange[ir];
                ir++;
                if (len >= NowTotalNum) {
                    len = NowTotalNum-1;
                }
                int start = RandomInt(1, NowTotalNum - len);
                int end = start + len - 1;
                if (end >= NowTotalNum) {
                    end = NowTotalNum - 1;
                }
                //cout<<"reorder "<<start<<"  "<<end<<endl;
                //cout<<"new order: ";
                int * oldArray = sa->RangeQuery(start, end, &NumSA);
                int * newArray = new int[len];
                for (int j = 0; j < len; ++j) {
                    newArray[j] = oldArray[len-j-1];
                    //cout<<newArray[j]<<" ";
                }
                // cout<<endl;

                sa->Reorder(start, end, newArray);
                da->Reorder(start, end, newArray);
                cbt->Reorder(start, end, newArray);
                ll->Reorder(start, end, newArray);
                delete []newArray;
                delete []oldArray;
                break;
            }
            case 5: //swap
            {
                int a[4] = {};
                for (int & j : a) {
                    j = RandomInt(1, NowTotalNum);
                }
                sort(a, a+4);
                if (a[1] == a[2]) {
                    continue;
                }
                int start1 = a[0];
                int end1 = a[1];
                int start2 = a[2];
                int end2 = a[3];
                sa->Swap(start1, end1, start2, end2);
                da->Swap(start1, end1, start2, end2);
                cbt->Swap(start1, end1, start2, end2);
                ll->Swap(start1, end1, start2, end2);
                break;
            }
            case 6: //move
            {
                int a[3] = {};
                for (int & j : a) {
                    j = RandomInt(1, NowTotalNum);
                }
                sort(a, a+3);
                if (a[1] == a[2]) {
                    continue;
                }
                int start1, end1, start2;

                if (RandomInt(1, 10) % 2 == 1) {
                    start1 = a[0];
                    end1 = a[1];
                    start2 = a[2];
                } else {
                    start1 = a[1];
                    end1 = a[2];
                    start2 = a[0];
                }
                sa->Move(start1, end1, start2);
                da->Move(start1, end1, start2);
                cbt->Move(start1, end1, start2);
                ll->Move(start1, end1, start2);
                break;
            }
        }

        if ( (lt+1 <= 10) || ((lt+1<=100) && ((lt+1)%10 == 0)) || ((lt+1<=1000) && ((lt+1)%100 == 0))
             || ((lt+1<=10000) && ((lt+1)%1000 == 0)) || ((lt+1<=100000) && ((lt+1)%10000 == 0))
             || ((lt+1<=1000000) && ((lt+1)%100000 == 0))) {

            numUpdate ++;

            //if ((lt+1)%ComputeInterval == 0) {
            cout<<"ll length: "<<ll->NumItem<<endl;
            //ll->PrintStructure();
            long long Tsa = 0, Tda = 0, Tll = 0, Tcbt = 0;
            CurOutputNum ++;
            double fl = (lt+1)*1.0/operations ;
            finstant << fl << ",";
            int pos = RandomInt(1, NowTotalNum);
            int IDtoDelete = sa->Query(pos);

            //printf("here is printing, lt = %d, before delete\n", lt);

            TimeVar timea = timeNow();
            ID_delete = IDtoDelete;
            /* Execute the INSERT statement - 1*/
            if (mysql_stmt_execute(deleteStmt))
            {
                fprintf(stderr, " mysql_stmt_execute(), 1 failed\n");
                fprintf(stderr, " %s\n", mysql_stmt_error(deleteStmt));
                exit(0);
            }
            TimeVar timeb = timeNow();
            long long s = duration(timeb-timea);

            time1 = timeNow();
            sa->Delete(pos);
            time2 = timeNow();
            Tsa = duration(time2-time1) + s;

            time1 = timeNow();
            da->Delete(pos);
            time2 = timeNow();
            Tda = duration(time2-time1) + s;

            time1 = timeNow();
            ll->Delete(pos);
            time2 = timeNow();
            Tll = duration(time2 - time1) + s;

            time1 = timeNow();
            cbt->Delete(pos);
            time2 = timeNow();
            Tcbt = duration(time2 - time1) + s;

            finstant <<Tda << ","<< Tsa<<","<<Tll<<","<<Tcbt<<endl;
            cout<<"lt = "<< lt <<" da depth = "<<da->Depth()<<endl;
            flog<<"lt= "<<lt<<" ll length = "<<ll->NumItem<<endl;

            NowTotalNum--;
        }
        numUpdate++;
    } //for lt <= loopTime
    //cout<<"da depth = "<<da->Depth() << endl;
    flog<<"da depth = "<<da->Depth() << endl;
    delete []reorderrange;
    delete []a;
    delete ll;
    flog.close();
    finstant.close();
    if ( mysql_stmt_close(insertStmt) || mysql_stmt_close(deleteStmt) )
    {
        /* mysql_stmt_close() invalidates stmt, so call          */
        /* mysql_error(mysql) rather than mysql_stmt_error(stmt) */
        fprintf(stderr, " failed while closing the statement\n");
        fprintf(stderr, " %s\n", mysql_error(mysql));
        exit(0);
    }
    return 0;
}

