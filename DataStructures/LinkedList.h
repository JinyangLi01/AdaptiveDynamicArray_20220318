#ifndef INDEXSTRUCTURE_LINKEDLIST_H
#define INDEXSTRUCTURE_LINKEDLIST_H

#include <iostream>
using namespace std;
/*
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define durationNS(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
*/
class ListMember {
public:
    int rowID;
    ListMember* next;
    ListMember(int ID, ListMember *next_);
    ~ListMember();
};

class LinkedList {
public:
    ListMember* startList;
    int NumItem;
    ListMember** array;
    int m;
    int ArrayLen;
    LinkedList(int m_, int *rowIDs, int num);

    explicit LinkedList(int m);

    ~LinkedList();

    int Query(int pos) const;

    void PrintLL() const;

    void PrintArray() const;

    int *RangeQuery(int start, int end, int*num) const;

    ListMember *FindBlock(int pos) const;

    void Insert(int rowID, int pos);

    void Delete(int pos);

    void NewArray();

    void Reorder(int start, int end, const int *newIDs) const;

    void Move(int start, int end, int des) const;

    void Swap(int start1, int end1, int start2, int end2) const;

    void SwapTimeAna(int start1, int end1, int start2, int end2) const;

    void PrintStructure() const;
};
LinkedList* NewLinkedListForArray(int m_, int * array, int num);
#endif //INDEXSTRUCTURE_LINKEDLIST_H
