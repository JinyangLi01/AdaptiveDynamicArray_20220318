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
#include <queue>
#include <memory>
#include <fstream>
#include <chrono>
#include <thread>         // std::thread, std::thread::id, std::this_thread::get_id
#include <random>
#include <algorithm>
#include <vector>
#include <string>
#include <mutex>
#include <functional>
#include <string>
#include <atomic>
#include "../../../../DataStructures/DynamicArray.h"
#include "../../../../DataStructures/StandardArray.h"
#include "../../../../DataStructures/CountedBtree.h"
#include "../../../../DataStructures/LinkedList.h"
#include "../../../../DataStructures/tiered-vector.h"
#include <vector>
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


struct ThreadPool {
    struct Task {
        virtual void Run() const = 0;
        virtual ~Task() {};
    };   

    template < typename task_, typename... args_ >
    struct RealTask : public Task {
        RealTask( task_&& task, args_&&... args ) : fun_( std::bind( std::forward<task_>(task), std::forward<args_>(args)... ) ) {}
        void Run() const override {
            fun_();
        }
    private:
        decltype( std::bind(std::declval<task_>(), std::declval<args_>()... ) ) fun_;
    };
    
    template < typename task_, typename... args_ >
    void AddTask( task_&& task, args_&&... args ) {
        auto lock = std::unique_lock<std::mutex>{mtx_};
        using FinalTask = RealTask<task_, args_... >;
        q_.push( std::unique_ptr<Task>( new FinalTask( std::forward<task_>(task), std::forward<args_>(args)... ) ) );
    }
    
    ThreadPool() {
        for( auto & t : pool_ )
            t = std::thread( [=] {
                while ( true ) {
                    std::unique_ptr<Task> task;
                    {
                        auto lock = std::unique_lock<std::mutex>{mtx_};
                        if ( q_.empty() && stop_ ) 
                            break;
                        if ( q_.empty() )
                            continue;
                        task = std::move(q_.front());
                        q_.pop();
                    }
                    if (task)
                        task->Run();
                }
            } );
    }
    ~ThreadPool() {
        stop_ = true;
        for( auto & t : pool_ )
            t.join();
    }
private:
    std::queue<std::unique_ptr<Task>> q_;
    std::thread pool_[8]; 
    std::mutex mtx_;
    volatile bool stop_ {};
};
ThreadPool pool;


class Message {
public:
    std::string operation;
    int data;
    Message(const std::string& op, int dt) : operation(op), data(dt){ }
    // Message(const std::string& operation, int num_op, int pos) : _operation(operation), _num_op(num_op), _pos(pos) { }
    // Message(const Message& inst) : _what(inst._what), _data(inst._data) { }

};
std::vector<Message> messages_vector;
bool wait_message = true;


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


int * VectorRangeQuery(vector<int> &vec, int start, int end) {
    int queryLength = end - start + 1;
    int * qans = new int[queryLength];
    for (int i = start-1; i <= end-1; i++){
        qans[i-start+1] = vec[i];
    }
    return qans;
}


void VectorSwap(vector<int> &vec, int start1, int end1, int start2, int end2)  {
    int len = end2-start1+1;
    int * newarray = new int[len];// make([]int, len)
    int in = 0;
    for (int k = start2-1; k <= end2-1; k ++) {
        newarray[in] = vec[k];
        in++;
    }

    for (int k = end1; k < start2-1; k ++) {
        newarray[in] = vec[k];
        in++;
    }

    for (int k = start1-1; k <= end1-1; k ++) {
        newarray[in] = vec[k];
        in++;
    }
    in = 0;
    for (int k = start1-1; k <= end2-1; k ++) {
        vec[k] = newarray[in];
        in++;
    }
    delete []newarray;
}

void VectorMove(vector<int> &vec, int NumItems, int start, int end, int des)  {
    if (des > NumItems) {
        des = NumItems;
    } else if (des >= start && des <= end+1) {
        return;
    }

    if (des < start) {
        int len = end-des+1;
        int * newarray = new int[len];
        int in = 0;
        for (int k = start-1; k <= end-1; k ++) {
            newarray[in] = vec[k];
            in++;
        }
        for (int k = des-1; k < start-1; k ++) {
            newarray[in] = vec[k];
            in++;
        }
        in = 0;
        for (int k = des-1; k <= end-1; k ++) {
            vec[k] = newarray[in];
            in++;
        }
        delete []newarray;
        return;
    } else {
        int len = des-start;
        int * newarray = new int[len];
        int in = 0;
        for (int k = end; k < des-1; k ++) {
            newarray[in] = vec[k];
            in++;
        }
        for (int k = start-1; k <= end-1; k ++) {
            newarray[in] = vec[k];
            in++;
        }
        in=0;
        for (int k = start-1; k < des-1; k ++) {
            vec[k] = newarray[in];
            in++;
        }
        delete []newarray;
        return;
    }
}

void SAInsert(int * & array, int ToInsert, int pos, int NumItems, int Length) {
    if (pos > NumItems) {
        pos = NumItems;
    }
    if (NumItems > Length) {
        int oldLen = Length;
        int newLen = Length * 2;
        int * newarray = new int[newLen + 1];
        int newNumItem = oldLen + 1;
        int j = 0;
        for (int i = 0; i < newNumItem; ++i) {
            if (i == pos-1) {
                continue;
            }
            newarray[i] = array[j];
            j++;
        }
        newarray[pos-1] = ToInsert;
        delete []array;
        array = newarray;
        NumItems = newNumItem;
        Length = newLen;
        return;
    }
    int itemToStore = ToInsert;
    for (int i = pos-1; i <= NumItems-1; ++i) {
        int tmp = array[i];
        array[i] = itemToStore;
        itemToStore = tmp;
    }
}

void SADelete(int * & array, int pos, int NumItems) {
    if (pos > NumItems) {
        pos = NumItems;
    }
    for (int i = pos; i < NumItems; i++) {
        array[i-1] = array[i];
    }
}

void SAReorder(int * & array, int start, int end, const int * newID) {
    for (int i = start; i <= end; ++i) {
        array[i-1] = newID[i-start];
    }
}

void SASwap(int * & array, int start1, int end1, int start2, int end2) {
    int len = end2-start1+1;
    int * newarray = new int[len];// make([]int, len)
    int in = 0;
    for (int k = start2-1; k <= end2-1; k ++) {
        newarray[in] = array[k];
        in++;
    }

    for (int k = end1; k < start2-1; k ++) {
        newarray[in] = array[k];
        in++;
    }

    for (int k = start1-1; k <= end1-1; k ++) {
        newarray[in] = array[k];
        in++;
    }
    in = 0;
    for (int k = start1-1; k <= end2-1; k ++) {
        array[k] = newarray[in];
        in++;
    }
    delete []newarray;
}

int * SARangeQuery(int * & array, int start, int end) {
    int queryLength = end - start + 1;
    int * qans = new int[queryLength];
    for (int i = start-1; i <= end-1; i++){
        qans[i-start+1] = array[i];
    }
    return qans;
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
const int iniNum = 225675; // movie dataset 220k awards
const int danodesize = 50;
const int m = 5000;
const int CbtOrder=danodesize;
int TinsertIdx = 0, TdeleteIdx = 0;
long long TinsertADA=0, TdeleteADA=0, TinsertSA = 0, TdeleteSA=0, TinsertCBT=0, TdeleteCBT=0,
        TinsertLL=0, TdeleteLL=0, TinsertTV=0, TdeleteTV=0, TinsertVEC=0, TdeleteVEC=0;
long long TinsertDB = 0, TdeleteDB = 0;
long long TreorderADA=0, TreorderSA=0, TreorderCBT=0, TreorderLL=0, TreorderTV=0, TreorderVEC=0;
long long TswapADA=0, TswapSA=0, TswapCBT=0, TswapLL=0, TswapTV=0, TswapVEC=0;
int NowTotalNum = iniNum;
const int prewindow = 30;
int pos = 10000; // middle line of the window currently

// vector<thread> threads;
int threadIdx = 1;

DynamicArray *da;
// StandardArray *sa;
int * standard_array = new int[iniNum];
int length_SA = iniNum;
CountedBtree * cbt;
LinkedList * ll;
Seq::Tiered<int, LayerItr<LayerEnd, Layer<50, Layer<50, Layer<50>>>>> tv;
vector<int> vec;
string table_name = "awards1";

class MysqlRow {
public:
    int id;
    string director_name, ceremony, year, category, outcome, original_language;
};


auto * rows0 = new MysqlRow[5*prewindow];
auto * rows1 = new MysqlRow[5*prewindow];

// time query db once
long long Thisdb = 0;
// TqueryDB: query time for db in asyn threads, only for DB!!!
// TqueryADA, TquerySA, TqueryCBT, TqueryTV: query time for syn main thread
long long TqueryDB = 0, TqueryADA = 0, TquerySA = 0, TqueryCBT=0, TqueryLL=0, TqueryTV=0, TqueryVEC = 0;



void insertToDB(int ToInsert, int insertindex, int threadIdx) {
    cout << " Here is inserttodb" << endl;
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
    // cout << "prepare insert stmt" << endl;
    sql::PreparedStatement* insertStmt = sqlconn->pstmtInsert;
    TimeVar time1 = timeNow();
    insertStmt->setInt(1, ToInsert);
    for (int k = 2; k <= 7; ++k) {
        int thisLen = RandomInt(1, STRING_SIZE);
        insertStmt->setString(k, RandomString(thisLen));
    }
    insertStmt->execute();
    TimeVar time2 = timeNow();
    long long t = duration(time2 - time1);
    if (insertindex >= 0) {
        TinsertDB += t;
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
        TdeleteDB += t;
    }
    (*(SqlConAvaliable[i])).clear();
    //printf("thread %d finishes\n", threadIdx);
}


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
    // cout << "Here is FetchSqlRes1" << endl;
    int i = 0;
    while (res1->next()) {
        // You can use either numeric offsets...
        // cout << "in which loop, i = " << i << endl;
        rows[i].id = res1->getInt(1);
        //cout<< res1->getInt(1) << endl;
        rows[i].director_name = res1->getString(2);
        //cout<< rows[i].director_name << endl;
        rows[i].ceremony = res1->getString(3);
        rows[i].year = res1->getString(4);
        rows[i].category = res1->getString(5);
        rows[i].outcome = res1->getString(6);
        rows[i].original_language = res1->getString(7);
        //cout<< rows[i].original_language << endl;
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
    // cout<<"Here is QueryDB1"<<endl;
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

void DoQueries(int num_queries) {
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
        IDs = SARangeQuery(standard_array, bufferstart, bufferend);
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


        timea = timeNow();
        IDs = VectorRangeQuery(vec, bufferstart, bufferend);
        timeb = timeNow();
        TqueryVEC += duration(timeb - timea);

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
                // thread t(QueryDB0, rows0, IDs, num, TqueryIdx, threadIdx);
                // t.detach();
                
                pool.AddTask( QueryDB0, rows0, IDs, num, TqueryIdx, threadIdx );    
                
                // messages_vector.push_back(Message("query", 20, pos));
                threadIdx++;
            } else {
                // thread t(QueryDB1, rows1, IDs, num, TqueryIdx, threadIdx);
                // t.detach();
                pool.AddTask(QueryDB1, rows1, IDs, num, TqueryIdx, threadIdx);
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
                TqueryVEC += duration(time2 - time1);
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
                TqueryVEC += duration(time2 - time1);
            }
        }
    }
}

// insert into pos
void DoInsert(int num_insert, int ToInsert) {
    cout << "Here is DoInsert, num_insert=" << num_insert  << endl;
    int winstart = pos - 14, winend = pos + 15;
    int insert_pos = pos;
    TimeVar time1, time2, time3;
    
    for (int i = 0; i < num_insert; i ++) {
        time1 = timeNow();
        da->Insert(ToInsert, insert_pos);
        time2 = timeNow();
        cout << "da insert time = " << duration(time2-time1);
        int queryResda = da->Query(insert_pos);
        // printf("inserted = %d, query da after inserting: %d\n", ToInsert, queryResda);


        TinsertADA += duration(time2-time1);
        time1 = timeNow();
        SAInsert(standard_array, ToInsert, pos, iniNum, length_SA);
        time2 = timeNow();
        TinsertSA += duration(time2-time1);
        time1 = timeNow();
        cbt->Insert(ToInsert, insert_pos);
        time2 = timeNow();
        TinsertCBT += duration(time2-time1);
        time1 = timeNow();
        ll->Insert(ToInsert, insert_pos);
        time2 = timeNow();
        TinsertLL += duration(time2-time1);


        time1 = timeNow();
        tv.insert(insert_pos, ToInsert);
        time2 = timeNow();
        cout << " tv insert time = " << duration(time2-time1);
        cout << " insert pos = "<<insert_pos;
        TinsertTV += duration(time2-time1);

        time1 = timeNow();
        vec.insert(vec.begin() + pos - 1, ToInsert);
        time2 = timeNow();
        TinsertVEC += duration(time2-time1);

        int queryRes = tv.Query(insert_pos);
        printf(" inserted = %d, query tv: %d\n", ToInsert, queryRes);

        time2 = timeNow();
        // thread t(insertToDB, ToInsert, TinsertIdx, threadIdx);
        // t.detach();
        // pool.AddTask(insertToDB, ToInsert, TinsertIdx, threadIdx);

        messages_vector.push_back(Message("insert", ToInsert));
        time3 = timeNow();
        cout << " time for messages_vector: " << duration(time3 - time2) << endl;
        TinsertADA += duration(time3 - time2);
        TinsertSA += duration(time3 - time2);
        TinsertCBT += duration(time3 - time2);
        TinsertLL += duration(time3 - time2);
        TinsertTV += duration(time3 - time2);
        TinsertVEC += duration(time3 - time2);

        ToInsert++;
        NowTotalNum++;
        threadIdx++;
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        insert_pos;
    }
}

// delete from winstart
void DoDelete(int num_delete) {
    int winstart = pos - 14, winend = pos + 15;
    int delete_pos = winstart;
    TimeVar time1, time2, time3;

    for (int i = 0; i < num_delete; i ++) {
        time1 = timeNow();
        int delete_id = da->Delete(delete_pos);
        time2 = timeNow();
        cout << "da delete time = " << duration(time2-time1);
        TdeleteADA += duration(time2-time1);
        time1 = timeNow();
        SADelete(standard_array, pos, iniNum);
        time2 = timeNow();
        TdeleteSA += duration(time2-time1);
        time1 = timeNow();
        cbt->Delete(delete_pos);
        time2 = timeNow();
        TdeleteCBT += duration(time2-time1);
        time1 = timeNow();
        ll->Delete(delete_pos);
        time2 = timeNow();
        TdeleteLL += duration(time2-time1);
        time1 = timeNow();
        tv.remove(delete_pos);
        time2 = timeNow();
        cout << "tv delete time = " << duration(time2-time1);
        TdeleteTV += duration(time2-time1);

        time1 = timeNow();
        vec.erase(vec.begin() + pos - 1);
        time2 = timeNow();
        TdeleteVEC += duration(time2-time1);


        time2 = timeNow();
        // thread t(deleteFromDB, delete_id, 1, threadIdx);
        // t.detach();
        //pool.AddTask(deleteFromDB, delete_id, 1, threadIdx);
        
        messages_vector.push_back(Message("delete", delete_id));
        time3 = timeNow();

        cout << " time for messages_vector: " << duration(time3 - time2) << endl;

        TdeleteADA += duration(time3 - time2);
        TdeleteSA += duration(time3 - time2);
        TdeleteCBT += duration(time3 - time2);
        TdeleteLL += duration(time3 - time2);
        TdeleteTV += duration(time3 - time2);
        TdeleteVEC += duration(time3 - time2);

        NowTotalNum--;
        threadIdx++;
        
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void receive_message() {
    //cout << "here is receive_message()" << endl;
    while(wait_message) {
        if(messages_vector.size() > 0) {
            //cout << "Here is messages_vector.size() > 0" << endl;
            Message m = messages_vector.back();
            messages_vector.pop_back();
            int data = m.data;
            if (m.operation == "insert") {
                insertToDB(data, TinsertIdx, threadIdx);
            } else  if (m.operation == "delete") {
                deleteFromDB(data, 1, threadIdx);
            }
            
        }
    }
}



int main() {

    try {
        
        numSqlCons.operator=(100);
        int ToInsert = 300150;

        /* Create a connection */
        driver = get_driver_instance();

        string queryString = "SELECT * FROM " + table_name + " WHERE id in (?";
        for (int l = 1; l < 5*prewindow; ++l) {
            queryString += ", ?";
        }
        queryString += ")";

        string insertString = "INSERT INTO " + table_name + " (id, director_name, ceremony, year, category, outcome, original_language) values (?, ?, ?, ?, ?, ?, ?) ";
        string deleteString  = " DELETE FROM " + table_name + " WHERE id = ?";
        for (int l = 0; l < numSqlCons; ++l) {
            auto * sqlconn = new SQLCONN();
            sqlconn->sqlconnection = driver->connect("localhost", "root", "LjyDq1998");
            /* Connect to the MySQL test database */
            sqlconn->sqlconnection->setSchema("ADA1");
            sqlconn->pstmtQuery = sqlconn->sqlconnection->prepareStatement(queryString);
            sqlconn->pstmtInsert = sqlconn->sqlconnection->prepareStatement(insertString);
            sqlconn->pstmtDelete = sqlconn->sqlconnection->prepareStatement(deleteString);
            SqlConVector.push_back(sqlconn);
            auto * ai = new atomic_flag; (*ai).clear();
            SqlConAvaliable.push_back(ai);
        }
        printf("size of SqlConVector = %lu\n", SqlConVector.size());

        //var time1, time2 time.Time
        string filepath[3] = {"m_result_" + table_name + ".csv", "m_result_" + table_name + "Log.txt"};
        ofstream finstant, flog, ffinal;
        finstant.open(filepath[0], ios::out | ios::in | ios::trunc);
        flog.open(filepath[1], ios::out | ios::in | ios::trunc);
        finstant << "table" + table_name + " , message.cpp, tiered <500>, swap(pos, +600, +1000, +2500)" << endl;


        int InsertActions = 5;
        int DeleteActions = 5;
        int ReorderActions = 1;
        int SwapActions = 1;
        int QueryActions = 88;

        /*
         * 1: query, 2: insert, 3: delete, 4: reorder, 5: swap
         */
        printf("30 queries, 5 insert, 5 delete, 30 query, 1 reorder, 1 swap, 28 queries\n");
        printf("# of operations = %d\n", operations);


        TimeVar time1, time2, time3;

        NowTotalNum = iniNum;
        
        int *array = new int[iniNum];
        for (int i = 0; i < iniNum; ++i) {
            array[i] = i + 1;
        }

        da = NewDynamicArray(array, iniNum, danodesize);
        for (int i = 0; i < iniNum; i ++) {
            standard_array[i] = array[i];
        }
        // sa = NewStandardArray(array, iniNum);
        cbt = NewCBTreeForArray(CbtOrder, iniNum);
        ll = NewLinkedListForArray(m, array, iniNum);
        tv.initialize(array, iniNum);

        printf("tv after initialization, query 10th: %d\n", tv.Query(10));
        vec.reserve(iniNum);
        for (int i = 0; i < iniNum; ++i) {
            vec.push_back(array[i]);
        }

        thread worker(receive_message);
        worker.detach();

        delete[]array;
        array = nullptr;
        int depth = 1, NumDA;

        // 30 queries:
        cout << "30 queries" << endl;

        available0.clear();
        available1.clear();

        pos = RandomInt(1, NowTotalNum);
        int winstart = pos - 14, winend = pos + 15;
        int bufferstart = winstart - 2 * prewindow;
        int bufferend = winend + 2 * prewindow;
        int num = 0;


        int *IDs = da->RangeQuery(bufferstart, bufferend, &num);

        thread th(QueryDB1, rows1, IDs, num, -1, threadIdx);
        th.join();
        threadIdx++;

        DoQueries(30);


        //5 insert 
        cout << InsertActions << " insert" << endl;
        DoInsert(InsertActions, ToInsert);
        printf("query tv finally: %d, %d\n", tv.Query(pos), tv.Query(pos+2));

        // 5 delete
        cout << DeleteActions << " delete" << endl;
        DoDelete(DeleteActions);

        cout << "10 queries" << endl;
        //30 queries
        DoQueries(30);

        //1 reorder
        winstart = pos - 14, winend = pos + 15;
        int start = winstart, end = winend;
        if (end >= NowTotalNum) {
            end = NowTotalNum - 1;
        }
        int *oldArray = da->RangeQuery(start, end, &NumDA);
        int len = end-start+1;
        int *newArray = new int[len];
        for (int j = 0; j < len; ++j) {
            newArray[j] = oldArray[len - j - 1];
        }
        time1 = timeNow();
        da->Reorder(start, end, newArray);
        time2 = timeNow();
        TreorderADA += duration(time2 - time1); 
        time1 = timeNow();
        SAReorder(standard_array, start, end, newArray);
        time2 = timeNow();
        TreorderSA += duration(time2 - time1);
        time1 = timeNow();
        cbt->Reorder(start, end, newArray);
        time2 = timeNow();
        TreorderCBT += duration(time2 - time1);
        time1 = timeNow();
        ll->Reorder(start, end, newArray);
        time2 = timeNow();
        TreorderLL += duration(time2 - time1);
        time1 = timeNow();
        tv.Reorder(start, end, newArray);
        time2 = timeNow();
        TreorderTV += duration(time2 - time1);
        time1 = timeNow();
        for (int i = start-1; i <= end-1; ++i) {
            vec[i] = newArray[i-start+1];
        }
        time2 = timeNow();
        TreorderVEC += duration(time2 - time1);


        delete[]newArray;
        delete[]oldArray;

        //1 swap
        winstart = pos - 14, winend = pos + 15;
        int start1 = winstart;
        int end1 = winstart + 200;
        int start2 = winstart + 300;
        int end2 = winstart + 480;
        time1 = timeNow();
        da->Swap(start1, end1, start2, end2);
        time2 = timeNow();
        TswapADA += duration(time2 - time1);
        time1 = timeNow();
        SASwap(standard_array, start1, end1, start2, end2);
        time2 = timeNow();
        TswapSA += duration(time2 - time1);
        time1 = timeNow();
        cbt->Swap(start1, end1, start2, end2);
        time2 = timeNow();
        TswapCBT += duration(time2 - time1);
        time1 = timeNow();
        ll->Swap(start1, end1, start2, end2);
        time2 = timeNow();
        TswapLL += duration(time2 - time1);
        time1 = timeNow();
        tv.Swap(start1, end1, start2, end2);
        time2 = timeNow();
        TswapTV += duration(time2 - time1);
        time1 = timeNow();
        VectorSwap(vec, start1, end1, start2, end2);
        time2 = timeNow();
        TswapVEC += duration(time2 - time1);


        //28 queries
        DoQueries(28);


        // printf("threads size = %lu\n\n", threads.size());

        finstant << "insert, ADA, SA, CBT, LL, TV, VEC" << endl;
        finstant << "asyn," << TinsertADA/InsertActions << "," << TinsertSA/InsertActions << "," << TinsertCBT/InsertActions
         << "," << TinsertLL/InsertActions << "," << TinsertTV/InsertActions<< "," << TinsertVEC/InsertActions << endl;
        finstant << "syn," << (TinsertADA+TinsertDB)/InsertActions << "," << (TinsertSA+TinsertDB)/InsertActions << "," 
        << (TinsertCBT+TinsertDB)/InsertActions << "," << (TinsertLL+TinsertDB)/InsertActions << "," << (TinsertTV+TinsertDB)/InsertActions
        << "," << (TinsertVEC+TinsertDB)/InsertActions << endl;

        finstant << "delete, ADA, SA, CBT, LL, TV, VEC" << endl;
        finstant << "asyn," << TdeleteADA/DeleteActions << "," << TdeleteSA/DeleteActions << "," << TdeleteCBT/DeleteActions
         << "," << TdeleteLL/DeleteActions << "," << TdeleteTV/DeleteActions << "," << TdeleteVEC/DeleteActions<< endl;
        finstant << "syn," << (TdeleteADA+TdeleteDB)/InsertActions << "," << (TdeleteSA+TdeleteDB)/InsertActions << "," 
        << (TdeleteCBT+TdeleteDB)/InsertActions << "," << (TdeleteLL+TdeleteDB)/InsertActions << "," << (TdeleteTV+TdeleteDB)/InsertActions
        << "," << (TdeleteVEC+TdeleteDB)/InsertActions << endl;

        finstant << "reorder, ADA, SA, CBT, LL, TV, VEC" << endl;
        finstant << "no framework needed," << TreorderADA/ReorderActions << "," << TreorderSA/ReorderActions << "," << TreorderCBT/ReorderActions
         << "," << TreorderLL/ReorderActions << "," << TreorderTV/ReorderActions << "," << TreorderVEC/ReorderActions << endl;

        finstant << "swap, ADA, SA, CBT, LL, TV, VEC" << endl;
        finstant << "no framework needed," << TswapADA/SwapActions << "," << TswapSA/SwapActions << "," << TswapCBT/SwapActions
         << "," << TswapLL/SwapActions << "," << TswapTV/SwapActions << "," << TswapVEC/SwapActions << endl;


        finstant << "query, ADA, SA, CBT, LL, TV, VEC" << endl;
        finstant << "prefetching," << TqueryADA/QueryActions << "," << TquerySA/QueryActions << "," << TqueryCBT/QueryActions
         << "," << TqueryLL/QueryActions << "," << TqueryTV/QueryActions << "," << TqueryVEC/QueryActions<< endl;
        finstant << "non-prefetching," << (TqueryADA+TqueryDB)/QueryActions << "," << (TquerySA+TqueryDB)/QueryActions << "," << (TqueryCBT+TqueryDB)/QueryActions
         << "," << (TqueryLL+TqueryDB)/QueryActions << "," << (TqueryTV+TqueryDB)/QueryActions << "," << (TqueryVEC+TqueryDB)/QueryActions << endl;


        finstant<<endl<<endl;


        
        int joinNum = 1;
        vector<thread>::iterator it;
        // for (it = threads.begin(); it != threads.end(); ++it) {
        //     //printf("before joining th %d\n", joinNum);
        //     it->join();
        //     //printf("after joining th %d\n", joinNum);
        //     joinNum++;
        // }

        flog.close();
        finstant.close();

        cout << "pos = " << pos << endl;
    } catch (sql::SQLException &e) {
        cout << "# ERR: SQLException in " << __FILE__;
        cout << "(" << __FUNCTION__ << ") on line " << __LINE__ << endl;
        cout << "# ERR: " << e.what();
        cout << " (MySQL error code: " << e.getErrorCode();
        cout << ", SQLState: " << e.getSQLState() << " )" << endl;
    }
    return 0;
}

