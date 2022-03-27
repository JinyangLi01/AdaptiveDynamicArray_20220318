#include "LinkedList.h"
#include <iostream>
#include <list>
#include <chrono>
using namespace std;
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define durationNS(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

LinkedList* NewLinkedListForArray(int m_, int * array, int num) {
    LinkedList * ll = new LinkedList(m_, array, num);
    return ll;
}

ListMember::ListMember(int ID, ListMember * next_) {
    rowID = ID;
    next = next_;
}

ListMember::~ListMember() = default;

LinkedList::LinkedList(int m_) {
    startList = nullptr;
    NumItem = 0;
    array = nullptr;
    m = m_;
    ArrayLen = 0;
}

LinkedList::LinkedList(int m_, int * rowIDs, int num) {
    startList = new ListMember(rowIDs[0], nullptr);
    m = m_;
    NumItem = num;
    ArrayLen = num / m;
    if (num % m > 0) {
        ArrayLen++;
    }
    array = new ListMember*[ArrayLen];
    ListMember * cur = startList;
    int ai = 0;
    for (int j = 0; j < NumItem; ++j) {
        if (j % m == 0) {
            array[ai] = cur;
            ai++;
        }
        if (j == NumItem - 1) {
            break;
        }
        cur->next = new ListMember(rowIDs[j+1], nullptr);
        cur = cur->next;
    }
}

LinkedList::~LinkedList() {
    ListMember * cur = startList;
    for (int i = 0; i < NumItem; ++i) {
        ListMember * n = cur->next;
        delete cur;
        cur = n;
    }
    delete []array;
}

int LinkedList::Query(int pos) const {
    int k = (pos-1)/m;
    ListMember* start = array[k];
    int remain = pos - k * m;
    for (int i = 2; i <= remain; i ++) {
        start = start->next;
    }
    return start->rowID;
}

void LinkedList::PrintLL() const {
    ListMember * b = array[0];
    int sum = 0;
    while (b != nullptr) {
        printf("%d ", b->rowID);
        sum++;
        if (b->next != nullptr) {
            b=b->next;
        } else {
            b=nullptr;
        }
    }
    printf("\n");
    printf("totally %d items in list.\n", sum);
}

void LinkedList::PrintStructure() const {
    printf("array length = %d, array: \n", ArrayLen);
    for (int i = 0; i < ArrayLen; ++i) {
        printf("%d ", array[i]->rowID);
    }
    printf("\n");
    printf("list: \n");
    ListMember * cur = startList;
    int rank = 0;
    while (cur != nullptr) {
        if (rank % m == 0) {
            printf("%d ", cur->rowID);
        }
        rank++;
        cur = cur->next;
    }
    printf("\n");
}

void LinkedList::PrintArray() const {
    printf("%d items, %d arrays\n", NumItem, ArrayLen);
}

int * LinkedList::RangeQuery(int start, int end, int * num) const {
    int llstart = 1, llend = NumItem, realstart = start, realend = end;
    if (realstart < llstart) {
        realstart = llstart;
    }
    if (realend > llend) {
        realend = llend;
    }
    int realnum = realend - realstart + 1;
    *num = realnum;
    if (realnum <= 0) {
        return nullptr;
    }
    int * ans = new int[realnum]; //= make([]int, len)

    int k = (realstart-1)/m;
    ListMember* b = array[k];
    int remain = realstart - k * m;
    for (int i = 2; i <= remain; i ++) {
        b = b->next;
    }
    for (int i = 0; i < realnum; i ++) {
        ans[i] = b->rowID;
        b = b->next;
    }
    return ans;
}

ListMember* LinkedList::FindBlock(int pos) const {
    if (array == nullptr) {
        return nullptr;
    }
    int k = (pos-1)/m;
    ListMember * start = array[k];
    int remain = pos - k * m;
    for (int i = 2; i <= remain; i ++) {
        start = start->next;
    }
    return start;
}

void LinkedList::Insert(int newID, int pos) {
    if (pos == 1) {
        ListMember * newStart = new ListMember(newID, startList);
        startList = newStart;
        NumItem++;
        NewArray();
        return;
    }
    ListMember * pre = FindBlock(pos-1);
    ListMember * newBlock = new ListMember(newID, pre->next);
    pre->next = newBlock;
    NumItem++;
    NewArray();
}

void LinkedList::Delete(int pos) {
    if (pos == 1) {
        ListMember * firstb = startList;
        startList = firstb->next;
        delete firstb;
        NumItem--;
        NewArray();
        return;
    }
    if (pos > NumItem) {
        return;
    }
    ListMember * pre = FindBlock(pos-1);
    ListMember * toDelete = pre->next;
    pre->next = toDelete->next;
    delete toDelete;
    NumItem--;
    NewArray();
}

void LinkedList::NewArray() {
    ListMember * cur = startList;
    ArrayLen = NumItem/m;
    if (NumItem % m > 0) {
        ArrayLen++;
    }
    delete []array;
    array = new ListMember *[ArrayLen];// make([]interface{}, ArrayLen);
    int ai = 0;
    for (int i = 0; i < NumItem; i ++) {
        if (i % m == 0) {
            array[ai] = cur;
            ai++;
        }
        cur = cur->next;
    }
}

void LinkedList::Reorder(int start, int end, const int * newIDs) const {
    ListMember * b = FindBlock(start);
    for (int i = start; i <= end; i ++) {
        b->rowID = newIDs[i-start];
        if (b->next != nullptr) {
            b=b->next;
        }
    }
}


void LinkedList::Swap(int start1, int end1, int start2, int end2) const {
    int tmp;
    int * r0 = RangeQuery(start1, end2, &tmp);
    int len1 = end2-start2+1;
    int len2 = start2-end1-1;
    int len3 = end1-start1+1;

    ListMember * b = FindBlock(start1);
    int s = 0;
    for (; s < len1; s ++) {
        b->rowID = r0[s + len3+len2];
        if (b->next != nullptr) {
            b=b->next;
        }
    }
    for (s = 0; s < len2; s ++) {
        b->rowID = r0[s+len3];
        if (b->next != nullptr) {
            b=b->next;
        }
    }
    for (s = 0; s < len3; s ++) {
        b->rowID = r0[s];
        if (b->next != nullptr) {
            b=b->next;
        }
    }
    delete []r0;
}


void LinkedList::SwapTimeAna(int start1, int end1, int start2, int end2) const {
    printf("num of items = %d, length of array = %d\n", NumItem, ArrayLen);
    int tmp;
    TimeVar time1 = timeNow();
    int * r0 = RangeQuery(start1, end2, &tmp);
    TimeVar time2 = timeNow();
    cout<<"rangequery "<<durationNS(time2-time1);
    int len1 = end2-start2+1;
    int len2 = start2-end1-1;
    int len3 = end1-start1+1;
    TimeVar time3 = timeNow();
    ListMember * b = FindBlock(start1);
    TimeVar time4 = timeNow();
    cout<<" findstart "<<durationNS(time4-time3);

    TimeVar time5 = timeNow();
    int s = 0;
    for (; s < len1; s ++) {
        b->rowID = r0[s + len3+len2];
        if (b->next != nullptr) {
            b=b->next;
        }
    }
    TimeVar time6 = timeNow();
    cout<<" len1 "<<durationNS(time6-time5);

    TimeVar time7 = timeNow();
    for (s = 0; s < len2; s ++) {
        b->rowID = r0[s+len3];
        if (b->next != nullptr) {
            b=b->next;
        }
    }
    TimeVar time8 = timeNow();
    cout<<" len2 "<<durationNS(time8-time7);

    TimeVar time9 = timeNow();
    for (s = 0; s < len3; s ++) {
        b->rowID = r0[s];
        if (b->next != nullptr) {
            b=b->next;
        }
    }
    TimeVar time10 = timeNow();
    cout<<" len3 "<<durationNS(time10-time9)<<endl;

    delete []r0;
}

void LinkedList::Move(int start, int end, int des) const {
    if (des > NumItem) {
        des = NumItem;
    }
    //delta := end-start
    if (des < start) {
        int nstart = des;
        int nend = start-1;
        Swap(nstart, nend, start, end);
        return;
    }  else if (des >= start && des <= end+1) {
        return;
    } else {
        int nstart = end+1;
        int nend = des-1;
        if (nend <= NumItem) {
            Swap(start, end, nstart, nend);
        } else {
            Swap(start, end, nstart, NumItem);
        }
        return;
    }
}
