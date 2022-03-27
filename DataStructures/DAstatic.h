//
// Created by Jinyang Li on 4/24/20.
//

#ifndef IS_TEST_STREE_H
#define IS_TEST_STREE_H
#include <cstdio>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

class NodeForStree {
public:
    NodeForStree **Pointers;
    int *Keys;
    NodeForStree *Parent;
    bool IsLeaf;
    int NumOfKeys;
    NodeForStree *Next;
    //NodeForStree *Previous;
    explicit NodeForStree(int capacity, int numkeys = 0, NodeForStree * parent = nullptr);
    ~NodeForStree() {
        delete[]Keys;
        delete[]Pointers;
    }
};

class DAstatic {
public:
    int NumItems;
    int Capacity;
    int Min; // Min = ceiling(Capacity / 2)
    NodeForStree * Root;

    DAstatic(int numItems, int capacity) {
        NumItems = numItems;
        Capacity = capacity;
        Min = ceil(Capacity * 1.0 / 2.0);
        Root = new NodeForStree(Capacity);
        Root->IsLeaf = true;
    }

    ~DAstatic();

    int *RangeQuery(int, int, int *) const;

    int NumOfItems() const { return NumItems; }

    void PrintTree() const;

    void Insert(int, int);

    void Delete(int);

    int Depth();

    NodeForStree *findLeaf(int pos, int *preIndexInLeaf) const;

    void SimplyAdjustAncestorKeysBy(NodeForStree *node, int increase) const;

    void AdjustAncestorKeysAccordingToCurNode(NodeForStree *n) const;

    NodeForStree *CompleteAdjustAncestorNodesAfterDelete(NodeForStree *n);

    NodeForStree *adjustAfterDeleteKeysInNode(NodeForStree *fromLeaf, int numKeysDeleted = -1);

    void insertIntoParent(NodeForStree *left, int sumKetsLeft, NodeForStree *right, int sumKeysRight, bool needGoUpwards = true);

    void insertIntoNewRoot(NodeForStree *left, int sumKeysLeft, NodeForStree *right, int sumKeysRight);

    int getLeftIndex(NodeForStree *parent, NodeForStree *left) const;

    static int NodeSumKeys(NodeForStree *n);

    void insertIntoNodeAfterSplitting(NodeForStree *n, int leftIndex, int sumKeysRight, NodeForStree *right, bool needGoupwards = true);

    int pathToRoot(NodeForStree *child) const;

    void deleteEntry(NodeForStree *n, int indexInNode);

    static NodeForStree *removeEntryFromNode(NodeForStree *n, int indexInNode);

    void adjustRoot();

    int getLeftNeighborIndex(NodeForStree *n) const;

    NodeForStree *coalesceNodes(NodeForStree *n, NodeForStree *neighbour, int neighbour_index);

    void redistributeNodes(NodeForStree *n, NodeForStree *neighbour, int neighbour_index);

    void coalesceNodesForDeleteOperation(NodeForStree *n, NodeForStree *neighbour, int neighbour_index);

    void deleteEntryForDeleteOperatoin(NodeForStree *n, int indexInNode);

    void redistributeNodesForDeleteOperation(NodeForStree *n, NodeForStree *neighbour, int neighbour_index);

    void Reorder(int start, int end, int *neworder) const;

    void deleteEntryForCheckMinArray(NodeForStree *n, int indexInNode);

    void coalesceNodesForCheckMinArray(NodeForStree *n, NodeForStree *neighbour, int neighbour_index);

    void swapLeaf(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                  int startIndexLeaf2, int endIndexLeaf2);

    int getIndexInParent(NodeForStree *n) const;

    NodeForStree * deleteMulEntry(NodeForStree **entryToDelete, int numEntryToDelete);

    static void movePointerInNodeForward(NodeForStree *node, int startIndex, int endIndex, int toIndex);

    NodeForStree *movePointerBefore(NodeForStree *fromLeaf, int startIndex, int endIndex, NodeForStree *toLeaf, int toIndex);

    NodeForStree *movePointerAfter(NodeForStree *fromLeaf, int startIndex, int endIndex, NodeForStree *toLeaf, int toIndex);

    void insertMulLeavesIntoLeafParentAfterSplitting(NodeForStree *toParent, int startIndexInParent, NodeForStree **leafToInsert,
                                                     int numLeafToInsert);

    void insertMulIntoParent(NodeForStree *toParent, NodeForStree **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNewRoot(NodeForStree *toParent, NodeForStree **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNode(NodeForStree *parent, int toIndex, NodeForStree **parentNewSibling, int parentNewSiblingNum,
                           int ParentIncrease) const;

    void insertMulIntoNodeAfterSplitting(NodeForStree *parent, int toIndex, NodeForStree **pointerToInsert, int pointerToInsertNum);

    void redistributeNodesForCheckMinArray(NodeForStree *n, NodeForStree *neighbour, int neighbour_index);

    void insertLeavesIntoLeafParent(NodeForStree *toParent, int startIndexInParent, NodeForStree **leafToInsert, int numLeafToInsert);

    void insertMulLeavesIntoLeafParent(NodeForStree *toParent, int startIndexInParent, NodeForStree **leafToInsert, int numLeafToInsert);

    void swapEndPointer(NodeForStree *leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf, NodeForStree *rightLeaf,
                        int indexInRightLeaf, int endIndexInRightLeaf);

    void swapWholeLeafAndGoUpwards(NodeForStree **wholeLeaf1, NodeForStree **bwholeLeaf2, int w1, int w2);

    void exchangeLeaf(NodeForStree *LeftLeaf, NodeForStree *LeftParent, NodeForStree *RightLeaf, NodeForStree *RightParent) const;

    void RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForStree *node);

    void adjustAncestorKeysAfterExchangingLeaves(NodeForStree **nodeArray, int numNodeForStreeInArray);

    static void swapHeadTail12(NodeForStree *inter1, NodeForStree *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    static void swapHeadTail11(NodeForStree *inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void swapHeadTail21(NodeForStree *inter1, NodeForStree *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    static void swapHeadTail22(NodeForStree *inter1, NodeForStree *inter2, NodeForStree *inter3, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen1Is1(NodeForStree **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTailLen2Is1(NodeForStree **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTailLen1Is2(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen2Is2(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAreBoth2(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2,
                                            int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    NodeForStree *DeleteMidPartOfTwoAdjacentLeaves(NodeForStree *leaf1, int startIndexInLeaf1, NodeForStree *leaf2, int endIndexInLeaf2);

    void SwapRangesAdjacentRightIs2(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                    int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAre1And2(NodeForStree *leftnode, NodeForStree *right1, NodeForStree *right2, int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenRightIs1(NodeForStree **inter1, int numLeaf1, NodeForStree *right, int sartIndexLeaf1, int endIndexLeaf1,
                                   int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacent(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2, int sartIndexLeaf1,
                            int endIndexLeaf1,
                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre2(NodeForStree *left1, NodeForStree *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForStree *right1,
                              NodeForStree *right2,
                              int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAreBoth1(NodeForStree *leftnode, NodeForStree *rightnode, int startIndexLeaf1, int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLeftIs1(NodeForStree *left, NodeForStree **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                                  int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentLeftIs2(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAre2And1(NodeForStree *left1, NodeForStree *left2, NodeForStree *right, int startIndexLeaf1,
                                            int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre1(NodeForStree *leaf1, int startIndex1, int endIndex1, NodeForStree *leaf2, int startIndex2, int endIndex2);


    void SwapWhenLensAre1And2(NodeForStree *leaf1, int startIndex1, int endIndex1, NodeForStree *leaf2, int startIndex2, NodeForStree *leaf3,
                         int endIndex3);

    NodeForStree *coalesceNodesStopAtParent(NodeForStree *n, NodeForStree *neighbour, int neighbour_index);

    void SwapForTailHead(NodeForStree **inter1, NodeForStree **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                         int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenLenIs1(NodeForStree *LongLeaf, NodeForStree **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    NodeForStree *swapStartPointer(NodeForStree *leftLeaf, int endIndexInLeftLeaf, NodeForStree *rightLeaf, int endIndexInRightLeaf);

    int RangePosInLeafPointer(int start, int end, NodeForStree ***inter, int *startIndexLeaf, int *endIndexLeaf);

    void Move(int start, int end, int des);

    void RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeForStree **inter1, int numLeaf1, NodeForStree *toleaf, int toindex);

    void MoveToLeafIsFirstLeaf(NodeForStree **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree *toleaf,
                               int toindex);

    void MoveToLeafIsLastLeaf(NodeForStree **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree *toleaf,
                              int toindex);

    void Move1(NodeForStree **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree *toleaf, int toindex);

    void MoveWhenLenIs1(NodeForStree **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree *toleaf, int toindex);

    void MoveWhenLenIs2(NodeForStree **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree *toleaf, int toindex);

    void Swap(int start1, int end1, int start2, int end2);

    static void insertIntoNode(NodeForStree *n, int leftIndex, int sumKeysRight, NodeForStree *right, bool needGoUpward = true);

    void AdjustAncestorKeyForOneNode(NodeForStree *c);

    void insertMulLeavesIntoLeafParentSplitOnce(NodeForStree *toParent, int startIndexInParent, NodeForStree **leafToInsert,
                                                int numLeafToInsert);

    void insertMulKeysIntoLeafAfterMulSplitting(int *keysToInsert, int numKeysToInsert, NodeForStree *toLeaf, int toIndex);

    void SwapWhenLeftLenIs2(NodeForStree *left1, NodeForStree *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForStree **inter2,
                            int numLeaf2,
                            int startIndexLeaf2, int endIndexLeaf2);

    void ReplaceMidOfTwoAdjacentLeaves(NodeForStree *left, NodeForStree *right, int startIndexLeft, int endIndexRight, int *keysToInsert,
                               int numToInsert);

    void CheckAndMergeRedistributeNode(NodeForStree *n);

    void replaceKeysInLeaf(NodeForStree *leaf, int startIdx, int endIdx, int *keysToInsert, int numToInsert);

    void insertOneIntoLeaf(NodeForStree *leaf, int newID, int indexInLeaf) const;

    void insertOneIntoLeafAfterSplitting(NodeForStree *leaf, int newID, int indexInLeaf, bool needGoupwards = true);

    bool insertKeysIntoLeaf(int *keysToInsert, int numKeysToInsert, NodeForStree *toLeaf, int toIndex);

    void insertMulKeysIntoLeaf(int *keysToInsert, int numKeysToInsert, NodeForStree *toLeaf, int toIndex);

    bool insertMulKeysIntoLeafAfterOneSplitting(int *keysToInsert, int numKeysToInsert, NodeForStree *toLeaf, int toIndex);

    void CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForStree *preLeaf, NodeForStree **leafToDelete,
                                                               int numLeafToDelete);

    NodeForStree *findPrevious(NodeForStree *node);

    int Query(int pos) const;
};

DAstatic * NewStree(int *array, int numItems, int degree);

#endif //IS_TEST_STREE_H
