#ifndef INDEXSTRUCTURE_DASTATICBIDIRECTIONAL_H
#define INDEXSTRUCTURE_DASTATICBIDIRECTIONAL_H
#include <cstdio>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>
typedef std::chrono::high_resolution_clock::time_point TimeVar;
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()

class NodeForDAstaticBidirectional {
public:
    NodeForDAstaticBidirectional **Pointers;
    int *Keys;
    NodeForDAstaticBidirectional *Parent;
    bool IsLeaf;
    int NumOfKeys;
    NodeForDAstaticBidirectional *Next;
    NodeForDAstaticBidirectional *Previous;
    explicit NodeForDAstaticBidirectional(int capacity, int numkeys = 0, NodeForDAstaticBidirectional * parent = nullptr);
    ~NodeForDAstaticBidirectional() {
        delete[]Keys;
        delete[]Pointers;
    }
};

class DAstaticBidirectional {
public:
    int NumItems;
    int Capacity;
    int Min; // Min = ceiling(Capacity / 2)
    NodeForDAstaticBidirectional * Root;

    DAstaticBidirectional(int numItems, int capacity) {
        NumItems = numItems;
        Capacity = capacity;
        Min = ceil(Capacity * 1.0 / 2.0);
        Root = new NodeForDAstaticBidirectional(Capacity);
        Root->IsLeaf = true;
    }

    ~DAstaticBidirectional() { delete Root; }

    int *RangeQuery(int, int, int *) const;

    int NumOfItems() const { return NumItems; }

    void PrintTree() const;

    void Insert(int, int);

    void Delete(int);

    int Depth();

    NodeForDAstaticBidirectional *findLeaf(int pos, int *preIndexInLeaf) const;

    void SimplyAdjustAncestorKeysBy(NodeForDAstaticBidirectional *node, int increase) const;

    void AdjustAncestorKeysAccordingToCurNode(NodeForDAstaticBidirectional *n) const;

    NodeForDAstaticBidirectional *CompleteAdjustAncestorNodesAfterDelete(NodeForDAstaticBidirectional *n);

    NodeForDAstaticBidirectional *adjustAfterDeleteKeysInNode(NodeForDAstaticBidirectional *fromLeaf, int numKeysDeleted = -1);

    void insertIntoParent(NodeForDAstaticBidirectional *left, int sumKetsLeft, NodeForDAstaticBidirectional *right, int sumKeysRight, bool needGoUpwards = true);

    void insertIntoNewRoot(NodeForDAstaticBidirectional *left, int sumKeysLeft, NodeForDAstaticBidirectional *right, int sumKeysRight);

    int getLeftIndex(NodeForDAstaticBidirectional *parent, NodeForDAstaticBidirectional *left) const;

    static int NodeSumKeys(NodeForDAstaticBidirectional *n);

    void insertIntoNodeAfterSplitting(NodeForDAstaticBidirectional *n, int leftIndex, int sumKeysRight, NodeForDAstaticBidirectional *right, bool needGoupwards = true);

    int pathToRoot(NodeForDAstaticBidirectional *child) const;

    void deleteEntry(NodeForDAstaticBidirectional *n, int indexInNode);

    static NodeForDAstaticBidirectional *removeEntryFromNode(NodeForDAstaticBidirectional *n, int indexInNode);

    void adjustRoot();

    int getLeftNeighborIndex(NodeForDAstaticBidirectional *n) const;

    NodeForDAstaticBidirectional *coalesceNodes(NodeForDAstaticBidirectional *n, NodeForDAstaticBidirectional *neighbour, int neighbour_index);

    void redistributeNodes(NodeForDAstaticBidirectional *n, NodeForDAstaticBidirectional *neighbour, int neighbour_index);

    void coalesceNodesForDeleteOperation(NodeForDAstaticBidirectional *n, NodeForDAstaticBidirectional *neighbour, int neighbour_index);

    void deleteEntryForDeleteOperatoin(NodeForDAstaticBidirectional *n, int indexInNode);

    void redistributeNodesForDeleteOperation(NodeForDAstaticBidirectional *n, NodeForDAstaticBidirectional *neighbour, int neighbour_index);

    void Reorder(int start, int end, int *neworder) const;

    void deleteEntryForCheckMinArray(NodeForDAstaticBidirectional *n, int indexInNode);

    void coalesceNodesForCheckMinArray(NodeForDAstaticBidirectional *n, NodeForDAstaticBidirectional *neighbour, int neighbour_index);

    void swapLeaf(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                  int startIndexLeaf2, int endIndexLeaf2);

    int getIndexInParent(NodeForDAstaticBidirectional *n) const;

    void deleteMulEntry(NodeForDAstaticBidirectional **entryToDelete, int numEntryToDelete);

    static void movePointerInNodeForward(NodeForDAstaticBidirectional *node, int startIndex, int endIndex, int toIndex);

    NodeForDAstaticBidirectional *movePointerBefore(NodeForDAstaticBidirectional *fromLeaf, int startIndex, int endIndex, NodeForDAstaticBidirectional *toLeaf, int toIndex);

    NodeForDAstaticBidirectional *movePointerAfter(NodeForDAstaticBidirectional *fromLeaf, int startIndex, int endIndex, NodeForDAstaticBidirectional *toLeaf, int toIndex);

    void insertMulLeavesIntoLeafParentAfterSplitting(NodeForDAstaticBidirectional *toParent, int startIndexInParent, NodeForDAstaticBidirectional **leafToInsert,
                                                     int numLeafToInsert);

    void insertMulIntoParent(NodeForDAstaticBidirectional *toParent, NodeForDAstaticBidirectional **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNewRoot(NodeForDAstaticBidirectional *toParent, NodeForDAstaticBidirectional **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNode(NodeForDAstaticBidirectional *parent, int toIndex, NodeForDAstaticBidirectional **parentNewSibling, int parentNewSiblingNum,
                           int ParentIncrease) const;

    void insertMulIntoNodeAfterSplitting(NodeForDAstaticBidirectional *parent, int toIndex, NodeForDAstaticBidirectional **pointerToInsert, int pointerToInsertNum);

    void redistributeNodesForCheckMinArray(NodeForDAstaticBidirectional *n, NodeForDAstaticBidirectional *neighbour, int neighbour_index);

    void insertLeavesIntoLeafParent(NodeForDAstaticBidirectional *toParent, int startIndexInParent, NodeForDAstaticBidirectional **leafToInsert, int numLeafToInsert);

    void insertMulLeavesIntoLeafParent(NodeForDAstaticBidirectional *toParent, int startIndexInParent, NodeForDAstaticBidirectional **leafToInsert, int numLeafToInsert);

    void swapEndPointer(NodeForDAstaticBidirectional *leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf, NodeForDAstaticBidirectional *rightLeaf,
                        int indexInRightLeaf, int endIndexInRightLeaf);

    void swapWholeLeafAndGoUpwards(NodeForDAstaticBidirectional **wholeLeaf1, NodeForDAstaticBidirectional **bwholeLeaf2, int w1, int w2);

    void exchangeLeaf(NodeForDAstaticBidirectional *LeftLeaf, NodeForDAstaticBidirectional *LeftParent, NodeForDAstaticBidirectional *RightLeaf, NodeForDAstaticBidirectional *RightParent) const;

    void RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAstaticBidirectional *node);

    void adjustAncestorKeysAfterExchangingLeaves(NodeForDAstaticBidirectional **nodeArray, int numNodeForDAstaticBidirectionalInArray);

    static void swapHeadTail12(NodeForDAstaticBidirectional *inter1, NodeForDAstaticBidirectional *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                               int endIndexLeaf2);

    static void swapHeadTail11(NodeForDAstaticBidirectional *inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void swapHeadTail21(NodeForDAstaticBidirectional *inter1, NodeForDAstaticBidirectional *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                               int endIndexLeaf2);

    static void swapHeadTail22(NodeForDAstaticBidirectional *inter1, NodeForDAstaticBidirectional *inter2, NodeForDAstaticBidirectional *inter3, int startIndexLeaf1, int endIndexLeaf1,
                               int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen1Is1(NodeForDAstaticBidirectional **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                             int endIndexLeaf2);

    void swapHeadTailLen2Is1(NodeForDAstaticBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                             int endIndexLeaf2);

    void swapHeadTailLen1Is2(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen2Is2(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAreBoth2(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2,
                                                   int startIndexLeaf1,
                                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAstaticBidirectional *DeleteMidPartOfTwoAdjacentLeaves(NodeForDAstaticBidirectional *leaf1, int startIndexInLeaf1, NodeForDAstaticBidirectional *leaf2, int endIndexInLeaf2);

    void SwapRangesAdjacentRightIs2(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                    int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAre1And2(NodeForDAstaticBidirectional *leftnode, NodeForDAstaticBidirectional *right1, NodeForDAstaticBidirectional *right2, int startIndexLeaf1,
                                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenRightIs1(NodeForDAstaticBidirectional **inter1, int numLeaf1, NodeForDAstaticBidirectional *right, int sartIndexLeaf1, int endIndexLeaf1,
                                        int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacent(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2, int sartIndexLeaf1,
                            int endIndexLeaf1,
                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre2(NodeForDAstaticBidirectional *left1, NodeForDAstaticBidirectional *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional *right1,
                              NodeForDAstaticBidirectional *right2,
                              int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAreBoth1(NodeForDAstaticBidirectional *leftnode, NodeForDAstaticBidirectional *rightnode, int startIndexLeaf1, int endIndexLeaf1,
                                                   int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLeftIs1(NodeForDAstaticBidirectional *left, NodeForDAstaticBidirectional **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                                       int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentLeftIs2(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    static void SwapRangesAdjacentWhenLensAre2And1(NodeForDAstaticBidirectional *left1, NodeForDAstaticBidirectional *left2, NodeForDAstaticBidirectional *right, int startIndexLeaf1,
                                                   int endIndexLeaf1,
                                                   int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre1(NodeForDAstaticBidirectional *leaf1, int startIndex1, int endIndex1, NodeForDAstaticBidirectional *leaf2, int startIndex2, int endIndex2);


    void SwapWhenLensAre1And2(NodeForDAstaticBidirectional *leaf1, int startIndex1, int endIndex1, NodeForDAstaticBidirectional *leaf2, int startIndex2, NodeForDAstaticBidirectional *leaf3,
                              int endIndex3);

    NodeForDAstaticBidirectional *coalesceNodesStopAtParent(NodeForDAstaticBidirectional *n, NodeForDAstaticBidirectional *neighbour, int neighbour_index);

    void SwapForTailHead(NodeForDAstaticBidirectional **inter1, NodeForDAstaticBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                         int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenLenIs1(NodeForDAstaticBidirectional *LongLeaf, NodeForDAstaticBidirectional **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAstaticBidirectional *swapStartPointer(NodeForDAstaticBidirectional *leftLeaf, int endIndexInLeftLeaf, NodeForDAstaticBidirectional *rightLeaf, int endIndexInRightLeaf);

    int RangePosInLeafPointer(int start, int end, NodeForDAstaticBidirectional ***inter, int *startIndexLeaf, int *endIndexLeaf);

    void Move(int start, int end, int des);

    void RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional **inter1, int numLeaf1, NodeForDAstaticBidirectional *toleaf, int toindex);

    void MoveToLeafIsFirstLeaf(NodeForDAstaticBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional *toleaf,
                               int toindex);

    void MoveToLeafIsLastLeaf(NodeForDAstaticBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional *toleaf,
                              int toindex);

    void Move1(NodeForDAstaticBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional *toleaf, int toindex);

    void MoveWhenLenIs1(NodeForDAstaticBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional *toleaf, int toindex);

    void MoveWhenLenIs2(NodeForDAstaticBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional *toleaf, int toindex);

    void CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAstaticBidirectional **leafToDelete, int numLeafToDelete);

    void Swap(int start1, int end1, int start2, int end2);

    static void insertIntoNode(NodeForDAstaticBidirectional *n, int leftIndex, int sumKeysRight, NodeForDAstaticBidirectional *right, bool needGoUpward = true);

    void AdjustAncestorKeyForOneNode(NodeForDAstaticBidirectional *c);

    void insertMulLeavesIntoLeafParentSplitOnce(NodeForDAstaticBidirectional *toParent, int startIndexInParent, NodeForDAstaticBidirectional **leafToInsert,
                                                int numLeafToInsert);

    void insertMulKeysIntoLeafAfterMulSplitting(int *keysToInsert, int numKeysToInsert, NodeForDAstaticBidirectional *toLeaf, int toIndex);

    void SwapWhenLeftLenIs2(NodeForDAstaticBidirectional *left1, NodeForDAstaticBidirectional *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAstaticBidirectional **inter2,
                            int numLeaf2,
                            int startIndexLeaf2, int endIndexLeaf2);

    void ReplaceMidOfTwoAdjacentLeaves(NodeForDAstaticBidirectional *left, NodeForDAstaticBidirectional *right, int startIndexLeft, int endIndexRight, int *keysToInsert,
                                       int numToInsert);

    void CheckAndMergeRedistributeNode(NodeForDAstaticBidirectional *n);

    void replaceKeysInLeaf(NodeForDAstaticBidirectional *leaf, int startIdx, int endIdx, int *keysToInsert, int numToInsert);

    void insertOneIntoLeaf(NodeForDAstaticBidirectional *leaf, int newID, int indexInLeaf) const;

    void insertOneIntoLeafAfterSplitting(NodeForDAstaticBidirectional *leaf, int newID, int indexInLeaf, bool needGoupwards = true);

    bool insertKeysIntoLeaf(int *keysToInsert, int numKeysToInsert, NodeForDAstaticBidirectional *toLeaf, int toIndex);

    void insertMulKeysIntoLeaf(int *keysToInsert, int numKeysToInsert, NodeForDAstaticBidirectional *toLeaf, int toIndex);

    bool
    insertMulKeysIntoLeafAfterOneSplitting(int *keysToInsert, int numKeysToInsert, NodeForDAstaticBidirectional *toLeaf, int toIndex);

};

DAstaticBidirectional * NewDAstaticBidirectional(int *array, int numItems, int degree);


#endif //INDEXSTRUCTURE_DASTATICBIDIRECTIONAL_H
