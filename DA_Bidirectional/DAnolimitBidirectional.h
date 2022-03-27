#ifndef INDEXSTRUCTURE_DANOLIMITBIDIRECTIONAL_H
#define INDEXSTRUCTURE_DANOLIMITBIDIRECTIONAL_H
#include <cstdio>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>
//typedef std::chrono::high_resolution_clock::time_point TimeVar;
//#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
//#define timeNow() std::chrono::high_resolution_clock::now()

class NodeForDAnolimitBidirectional {
public:
    NodeForDAnolimitBidirectional **Pointers;
    int *Keys;
    NodeForDAnolimitBidirectional *Parent;
    bool IsLeaf;
    int NumOfKeys;
    NodeForDAnolimitBidirectional *Next;
    NodeForDAnolimitBidirectional *Previous;
    explicit NodeForDAnolimitBidirectional(int capacity, int numkeys = 0, NodeForDAnolimitBidirectional * parent = nullptr);
    ~NodeForDAnolimitBidirectional()
    {
        delete[]Keys;
        delete[]Pointers;
    }
};

class DAnolimitBidirectional {
public:
    int NumItems;
    int Capacity;
    NodeForDAnolimitBidirectional *Root;
    int Min;
    DAnolimitBidirectional(int numItems, int capacity);
    ~DAnolimitBidirectional(){ delete Root; }
    int *RangeQuery(int, int, int *) const;
    int NumOfItems() { return NumItems; }
    int Depth();
    void PrintTree();
    void PrintDA();
    void Insert(int, int);
    void Delete(int);

    NodeForDAnolimitBidirectional *findLeaf(int pos, int *preIndexInLeaf, int *preIndexInArray) const;

    void SimplyAdjustAncestorKeysBy(NodeForDAnolimitBidirectional *node, int increase) const;

    void AdjustAncestorKeysAccordingToCurNode(NodeForDAnolimitBidirectional *n);

    NodeForDAnolimitBidirectional *CompleteAdjustAncestorNodesAfterDelete(NodeForDAnolimitBidirectional *n);

    NodeForDAnolimitBidirectional *adjustAfterDeleteKeysInNode(NodeForDAnolimitBidirectional *fromLeaf, int numKeysDeleted = -1);

    void insertOneIntoLeaf(NodeForDAnolimitBidirectional *leaf, int *righta, int rightlen, int IndexInLeaf) const;

    void
    insertIntoParent(NodeForDAnolimitBidirectional *left, int sumKetsLeft, NodeForDAnolimitBidirectional *right, int sumKeysRight, bool needGoUpwards = true);

    void insertIntoNewRoot(NodeForDAnolimitBidirectional *left, int sumKeysLeft, NodeForDAnolimitBidirectional *right, int sumKeysRight);

    int getLeftIndex(NodeForDAnolimitBidirectional *parent, NodeForDAnolimitBidirectional *left) const;

    static int NodeSumKeys(NodeForDAnolimitBidirectional *n);

    void insertIntoNodeAfterSplitting(NodeForDAnolimitBidirectional *n, int leftIndex, int sumKeysRight, NodeForDAnolimitBidirectional *right,
                                      bool needGoupwards = true);

    int pathToRoot(NodeForDAnolimitBidirectional *child) const;

    void deleteEntry(NodeForDAnolimitBidirectional *n, int indexInNode);

    static NodeForDAnolimitBidirectional *removeEntryFromNode(NodeForDAnolimitBidirectional *n, int indexInNode);

    void adjustRoot();

    int getLeftNeighborIndex(NodeForDAnolimitBidirectional *n);

    NodeForDAnolimitBidirectional *coalesceNodes(NodeForDAnolimitBidirectional *n, NodeForDAnolimitBidirectional *neighbour, int neighbour_index);

    void redistributeNodes(NodeForDAnolimitBidirectional *n, NodeForDAnolimitBidirectional *neighbour, int neighbour_index);

    void coalesceNodesForDeleteOperation(NodeForDAnolimitBidirectional *n, NodeForDAnolimitBidirectional *neighbour, int neighbour_index);

    void deleteEntryForDeleteOperatoin(NodeForDAnolimitBidirectional *n, int indexInNode);

    void redistributeNodesForDeleteOperation(NodeForDAnolimitBidirectional *n, NodeForDAnolimitBidirectional *neighbour, int neighbour_index);

    void Reorder(int start, int end, int *neworder) const;

    void CutArray(int pos);

    static void insertOneIntoLeafForCut(NodeForDAnolimitBidirectional *leaf, int *newarray, int length, int indexInLeaf);

    void MergeOrRedistributeArrayAfterDelete(NodeForDAnolimitBidirectional *leaf, int IndexInLeaf, int IndexInArray);

    void insertOneIntoLeafForDeletion(NodeForDAnolimitBidirectional *leaf, int *newa, int length, int indexInLeaf) const;

    void MergeOrRedistributeArrayAfterDeleteInMiddle(NodeForDAnolimitBidirectional *leaf, int IndexInLeaf, int IndexInArray);

    void CheckMinArrayAfterSwap(int s);

    void MergeRedistributeArrayAfterSwap(NodeForDAnolimitBidirectional *leaf, int IndexInLeaf);

    void deleteEntryForCheckMinArray(NodeForDAnolimitBidirectional *n, int indexInNode);

    void coalesceNodesForCheckMinArray(NodeForDAnolimitBidirectional *n, NodeForDAnolimitBidirectional *neighbour, int neighbour_index);

    void
    swapLeaf(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
             int startIndexLeaf2, int endIndexLeaf2);

    int getIndexInParent(NodeForDAnolimitBidirectional *n) const;

    void deleteMulEntry(NodeForDAnolimitBidirectional **entryToDelete, int numEntryToDelete);

    static void movePointerInNodeForward(NodeForDAnolimitBidirectional *node, int startIndex, int endIndex, int toIndex);

    //void PutLeavesIntoUpdateSumKeys(NodeForDAnolimitBidirectional **leaves, int numLeaf);

    void UpdateSumKeys(NodeForDAnolimitBidirectional **nodes, int numNode);

    NodeForDAnolimitBidirectional *movePointerBefore(NodeForDAnolimitBidirectional *fromLeaf, int startIndex, int endIndex, NodeForDAnolimitBidirectional *toLeaf, int toIndex);

    NodeForDAnolimitBidirectional *movePointerAfter(NodeForDAnolimitBidirectional *fromLeaf, int startIndex, int endIndex, NodeForDAnolimitBidirectional *toLeaf, int toIndex);

    void
    insertMulLeavesIntoLeafParentAfterSplitting(NodeForDAnolimitBidirectional *toParent, int startIndexInParent, NodeForDAnolimitBidirectional **leafToInsert,
                                                int numLeafToInsert);

    void insertMulIntoParent(NodeForDAnolimitBidirectional *toParent, NodeForDAnolimitBidirectional **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNewRoot(NodeForDAnolimitBidirectional *toParent, NodeForDAnolimitBidirectional **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNode(NodeForDAnolimitBidirectional *parent, int toIndex, NodeForDAnolimitBidirectional **parentNewSibling, int parentNewSiblingNum,
                           int ParentIncrease) const;

    void
    insertMulIntoNodeAfterSplitting(NodeForDAnolimitBidirectional *parent, int toIndex, NodeForDAnolimitBidirectional **pointerToInsert, int pointerToInsertNum);

    void redistributeNodesForCheckMinArray(NodeForDAnolimitBidirectional *n, NodeForDAnolimitBidirectional *neighbour, int neighbour_index);

    void insertLeavesIntoLeafParent(NodeForDAnolimitBidirectional *toParent, int startIndexInParent, NodeForDAnolimitBidirectional **leafToInsert,
                                    int numLeafToInsert);

    void insertMulLeavesIntoLeafParent(NodeForDAnolimitBidirectional *toParent, int startIndexInParent, NodeForDAnolimitBidirectional **leafToInsert,
                                       int numLeafToInsert);

    void swapEndPointer(NodeForDAnolimitBidirectional *leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf, NodeForDAnolimitBidirectional *rightLeaf,
                        int indexInRightLeaf, int endIndexInRightLeaf);

    void swapWholeLeafAndGoUpwards(NodeForDAnolimitBidirectional **wholeLeaf1, NodeForDAnolimitBidirectional **bwholeLeaf2, int w1, int w2);

    void exchangeLeaf(NodeForDAnolimitBidirectional *LeftLeaf, NodeForDAnolimitBidirectional *LeftParent, NodeForDAnolimitBidirectional *RightLeaf, NodeForDAnolimitBidirectional *RightParent) const;

    void RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAnolimitBidirectional *node);

    void adjustAncestorKeysAfterExchangingLeaves(NodeForDAnolimitBidirectional **nodeArray, int numNodeInArray);

    void swapHeadTail12(NodeForDAnolimitBidirectional *inter1, NodeForDAnolimitBidirectional *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail11(NodeForDAnolimitBidirectional *inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail21(NodeForDAnolimitBidirectional *inter1, NodeForDAnolimitBidirectional *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail22(NodeForDAnolimitBidirectional *inter1, NodeForDAnolimitBidirectional *inter2, NodeForDAnolimitBidirectional *inter3, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen1Is1(NodeForDAnolimitBidirectional **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                             int startIndexLeaf2,
                             int endIndexLeaf2);

    void swapHeadTailLen2Is1(NodeForDAnolimitBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1,
                             int startIndexLeaf2,
                             int endIndexLeaf2);

    void swapHeadTailLen1Is2(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen2Is2(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAreBoth2(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2,
                                            int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAnolimitBidirectional *
    DeleteMidPartOfTwoAdjacentLeaves(NodeForDAnolimitBidirectional *leaf1, int startIndexInLeaf1, NodeForDAnolimitBidirectional *leaf2, int endIndexInLeaf2);

    void
    SwapRangesAdjacentRightIs2(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                               int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SWapRangesAdjacentWhenLensAre1And2(NodeForDAnolimitBidirectional *leftnode, NodeForDAnolimitBidirectional *right1, NodeForDAnolimitBidirectional *right2, int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenRightIs1(NodeForDAnolimitBidirectional **inter1, int numLeaf1, NodeForDAnolimitBidirectional *right, int sartIndexLeaf1,
                                        int endIndexLeaf1,
                                        int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacent(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2, int sartIndexLeaf1,
                            int endIndexLeaf1,
                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre2(NodeForDAnolimitBidirectional *left1, NodeForDAnolimitBidirectional *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional *right1,
                              NodeForDAnolimitBidirectional *right2,
                              int startIndexLeaf2, int endIndexLeaf2);

    void
    SWapRangesAdjacentWhenLensAreBoth1(NodeForDAnolimitBidirectional *leftnode, NodeForDAnolimitBidirectional *rightnode, int startIndexLeaf1, int endIndexLeaf1,
                                       int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLeftIs1(NodeForDAnolimitBidirectional *left, NodeForDAnolimitBidirectional **inter2, int numLeaf2, int startIndexLeaf1,
                                       int endIndexLeaf1,
                                       int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapRangesAdjacentLeftIs2(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                              int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAre2And1(NodeForDAnolimitBidirectional *left1, NodeForDAnolimitBidirectional *left2, NodeForDAnolimitBidirectional *right, int startIndexLeaf1,
                                            int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre1(NodeForDAnolimitBidirectional *leaf1, int startIndex1, int endIndex1, NodeForDAnolimitBidirectional *leaf2, int startIndex2,
                              int endIndex2);


    void
    SwapWhenLensAre1And2(NodeForDAnolimitBidirectional *leaf1, int startIndex1, int endIndex1, NodeForDAnolimitBidirectional *leaf2, int startIndex2, NodeForDAnolimitBidirectional *leaf3,
                         int endIndex3);

    NodeForDAnolimitBidirectional *coalesceNodesStopAtParent(NodeForDAnolimitBidirectional *n, NodeForDAnolimitBidirectional *neighbour, int neighbour_index);

    void SwapForTailHead(NodeForDAnolimitBidirectional **inter1, NodeForDAnolimitBidirectional **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                         int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenLenIs1(NodeForDAnolimitBidirectional *LongLeaf, NodeForDAnolimitBidirectional **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAnolimitBidirectional *swapStartPointer(NodeForDAnolimitBidirectional *leftLeaf, int endIndexInLeftLeaf, NodeForDAnolimitBidirectional *rightLeaf, int endIndexInRightLeaf);

    int RangePosInLeafPointer(int start, int end, NodeForDAnolimitBidirectional ***inter, int *startIndexLeaf, int *endIndexLeaf);

    void Move(int start, int end, int des);

    void
    RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional **inter1, int numLeaf1, NodeForDAnolimitBidirectional *toleaf, int toindex);

    void
    MoveToLeafIsFirstLeaf(NodeForDAnolimitBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional *toleaf,
                          int toindex);

    void MoveToLeafIsLastLeaf(NodeForDAnolimitBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional *toleaf,
                              int toindex);

    void Move1(NodeForDAnolimitBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional *toleaf, int toindex);

    void MoveWhenLenIs1(NodeForDAnolimitBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional *toleaf,
                        int toindex);

    void MoveWhenLenIs2(NodeForDAnolimitBidirectional **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional *toleaf,
                        int toindex);

    NodeForDAnolimitBidirectional *CutForDesInMove(int des, int *toIndex);

    NodeForDAnolimitBidirectional *
    insertOneIntoLeafAfterSplittingForMove(NodeForDAnolimitBidirectional *leaf, int *newID, int length, int indexInLeaf, int *toIndex);

    void CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAnolimitBidirectional **leafToDelete, int numLeafToDelete);

    //void
    // Swap(int start1, int end1, int start2, int end2, int *CutArrayTime, int *FindLeafTime, int *swapLeafTime,
    //    int *CheckMinTime);

    void Swap(int start1, int end1, int start2, int end2);

    static void insertIntoNode(NodeForDAnolimitBidirectional *n, int leftIndex, int sumKeysRight, NodeForDAnolimitBidirectional *right, bool needGoUpward = true);

    void insertOneIntoLeafAfterSplitting(NodeForDAnolimitBidirectional *leaf, int *newPointer, int length, int indexInLeaf,
                                         bool needGoupwards = true);

    void AdjustAncestorKeyForOneNode(NodeForDAnolimitBidirectional *c);

    void insertMulLeavesIntoLeafParentSplitOnce(NodeForDAnolimitBidirectional *toParent, int startIndexInParent, NodeForDAnolimitBidirectional **leafToInsert,
                                                int numLeafToInsert);

    bool insertKeysIntoLeaf(int *keysToInsert, NodeForDAnolimitBidirectional **pointersToInsert, int numKeysToInsert, NodeForDAnolimitBidirectional *toLeaf,
                            int toIndex);

    bool insertMulKeysIntoLeafAfterOneSplitting(const int *keysToInsert, NodeForDAnolimitBidirectional **pointersToInsert, int numKeysToInsert,
                                                NodeForDAnolimitBidirectional *toLeaf,
                                                int toIndex);

    void insertMulKeysIntoLeaf(const int *keysToInsert, NodeForDAnolimitBidirectional **pointersToInsert, int numKeysToInsert, NodeForDAnolimitBidirectional *toLeaf,
                               int toIndex);

    void insertMulKeysIntoLeafAfterMulSplitting(const int *keysToInsert, NodeForDAnolimitBidirectional **pointersToInsert, int numKeysToInsert,
                                                NodeForDAnolimitBidirectional *toLeaf, int toIndex);

    void SwapWhenLeftLenIs2(NodeForDAnolimitBidirectional *left1, NodeForDAnolimitBidirectional *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimitBidirectional **inter2,
                            int numLeaf2,
                            int startIndexLeaf2, int endIndexLeaf2);

    void ReplaceMidOfTwoAdjacentLeaves(NodeForDAnolimitBidirectional *left, NodeForDAnolimitBidirectional *right, int startIndexLeft, int endIndexRight,
                                       int *keysToInsert,
                                       NodeForDAnolimitBidirectional **pointersToInsert, int numToInsert);

    void CheckAndMergeRedistributeNode(NodeForDAnolimitBidirectional *n);

    void replaceKeysInLeaf(NodeForDAnolimitBidirectional *leaf, int startIdx, int endIdx, int *keysToInsert, NodeForDAnolimitBidirectional **pointersToInsert,
                           int numToInsert);

    void PrintArray() const;
};

DAnolimitBidirectional * NewDAnolimitBidirectional(int * array, int numItems, int degree );


#endif //INDEXSTRUCTURE_DANOLIMITBIDIRECTIONAL_H
