#ifndef INDEXSTRUCTURE_DABIDIRECTIONAL_H
#define INDEXSTRUCTURE_DABIDIRECTIONAL_H
#include <cstdio>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>
//typedef std::chrono::high_resolution_clock::time_point TimeVar;
//#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
//#define timeNow() std::chrono::high_resolution_clock::now()

class NodeForDAbid {
public:
    NodeForDAbid **Pointers;
    int *Keys;
    NodeForDAbid *Parent;
    bool IsLeaf;
    int NumOfKeys;
    NodeForDAbid *Next;
    NodeForDAbid *Previous;
    explicit NodeForDAbid(int capacity, int numkeys = 0, NodeForDAbid *parent = nullptr);
    ~NodeForDAbid() {
        delete[]Keys;
        delete[]Pointers;
    }
};

class DAbidirectional {
public:
    int NumItems;
    int Capacity;
    int Min; // Min = ceiling(Capacity / 2)
    NodeForDAbid *Root;

    DAbidirectional(int numItems, int capacity) {
        NumItems = numItems;
        Capacity = capacity;
        Min = ceil(Capacity * 1.0 / 2.0);
        Root = new NodeForDAbid(Capacity);
    }

    ~DAbidirectional() { delete Root; }

    int *RangeQuery(int, int, int *);

    int NumOfItems() { return NumItems; }

    void PrintTree();

    void PrintDA();

    void Insert(int, int);

    void Delete(int);

    int Depth() const;

    NodeForDAbid *findLeaf(int pos, int *preIndexInLeaf, int *preIndexInArray);

    void SimplyAdjustAncestorKeysBy(NodeForDAbid *node, int increase);

    void AdjustAncestorKeysAccordingToCurNodeForDAbid(NodeForDAbid *n);

    NodeForDAbid *CompleteAdjustAncestorNodesAfterDelete(NodeForDAbid *n);

    NodeForDAbid *adjustAfterDeleteKeysInNodeForDAbid(NodeForDAbid *fromLeaf, int numKeysDeleted = -1);

    void insertOneIntoLeaf(NodeForDAbid *leaf, int *righta, int rightlen, int IndexInLeaf);

    void insertIntoParent(NodeForDAbid *left, int sumKetsLeft, NodeForDAbid *right, int sumKeysRight, bool needGoUpwards = true);

    void insertIntoNewRoot(NodeForDAbid *left, int sumKeysLeft, NodeForDAbid *right, int sumKeysRight);

    int getLeftIndex(NodeForDAbid *parent, NodeForDAbid *left);

    int NodeSumKeys(NodeForDAbid *n);

    void
    insertIntoNodeAfterSplitting(NodeForDAbid *n, int leftIndex, int sumKeysRight, NodeForDAbid *right, bool needGoupwards = true);

    int pathToRoot(NodeForDAbid *child);

    void deleteEntry(NodeForDAbid *n, int indexInNode);

    NodeForDAbid *removeEntryFromNodeForDAbid(NodeForDAbid *n, int indexInNode);

    void adjustRoot();

    int getLeftNeighborIndex(NodeForDAbid *n);

    NodeForDAbid *coalesceNodes(NodeForDAbid *n, NodeForDAbid *neighbour, int neighbour_index);

    void redistributeNodes(NodeForDAbid *n, NodeForDAbid *neighbour, int neighbour_index);

    void coalesceNodesForDeleteOperation(NodeForDAbid *n, NodeForDAbid *neighbour, int neighbour_index);

    void deleteEntryForDeleteOperatoin(NodeForDAbid *n, int indexInNode);

    void redistributeNodesForDeleteOperation(NodeForDAbid *n, NodeForDAbid *neighbour, int neighbour_index);

    void Reorder(int start, int end, int *neworder);

    void CutArray(int pos);

    void insertOneIntoLeafForCut(NodeForDAbid *leaf, int *newarray, int length, int indexInLeaf);

    void MergeOrRedistributeArrayAfterDelete(NodeForDAbid *leaf, int IndexInLeaf, int IndexInArray);

    void insertOneIntoLeafForDeletion(NodeForDAbid *leaf, int *newa, int length, int indexInLeaf);

    void MergeOrRedistributeArrayAfterDeleteInMiddle(NodeForDAbid *leaf, int IndexInLeaf, int IndexInArray);

    void CheckMinArrayAfterSwap(int s);

    void MergeRedistributeArrayAfterSwap(NodeForDAbid *leaf, int IndexInLeaf);

    void deleteEntryForCheckMinArray(NodeForDAbid *n, int indexInNode);

    void coalesceNodesForCheckMinArray(NodeForDAbid *n, NodeForDAbid *neighbour, int neighbour_index);

    void swapLeaf(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                  int startIndexLeaf2, int endIndexLeaf2);

    int getIndexInParent(NodeForDAbid *n);

    void deleteMulEntry(NodeForDAbid **entryToDelete, int numEntryToDelete);

    void movePointerInNodeForward(NodeForDAbid *node, int startIndex, int endIndex, int toIndex);

    //void PutLeavesIntoUpdateSumKeys(NodeForDAbid **leaves, int numLeaf);

    void UpdateSumKeys(NodeForDAbid **nodes, int numNode);

    NodeForDAbid *movePointerBefore(NodeForDAbid *fromLeaf, int startIndex, int endIndex, NodeForDAbid *toLeaf, int toIndex);

    NodeForDAbid *movePointerAfter(NodeForDAbid *fromLeaf, int startIndex, int endIndex, NodeForDAbid *toLeaf, int toIndex);

    void insertMulLeavesIntoLeafParentAfterSplitting(NodeForDAbid *toParent, int startIndexInParent, NodeForDAbid **leafToInsert,
                                                     int numLeafToInsert);

    void insertMulIntoParent(NodeForDAbid *toParent, NodeForDAbid **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNewRoot(NodeForDAbid *toParent, NodeForDAbid **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNodeForDAbid(NodeForDAbid *parent, int toIndex, NodeForDAbid **parentNewSibling, int parentNewSiblingNum,
                           int ParentIncrease);

    void insertMulIntoNodeAfterSplitting(NodeForDAbid *parent, int toIndex, NodeForDAbid **pointerToInsert, int pointerToInsertNum);

    void redistributeNodesForCheckMinArray(NodeForDAbid *n, NodeForDAbid *neighbour, int neighbour_index);

    void
    insertLeavesIntoLeafParent(NodeForDAbid *toParent, int startIndexInParent, NodeForDAbid **leafToInsert, int numLeafToInsert);

    void
    insertMulLeavesIntoLeafParent(NodeForDAbid *toParent, int startIndexInParent, NodeForDAbid **leafToInsert, int numLeafToInsert);

    void swapEndPointer(NodeForDAbid *leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf, NodeForDAbid *rightLeaf,
                        int indexInRightLeaf, int endIndexInRightLeaf);

    void swapWholeLeafAndGoUpwards(NodeForDAbid **wholeLeaf1, NodeForDAbid **bwholeLeaf2, int w1, int w2);

    void exchangeLeaf(NodeForDAbid *LeftLeaf, NodeForDAbid *LeftParent, NodeForDAbid *RightLeaf, NodeForDAbid *RightParent);

    void RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAbid *node);

    void adjustAncestorKeysAfterExchangingLeaves(NodeForDAbid **nodeArray, int numNodeInArray);

    void swapHeadTail12(NodeForDAbid *inter1, NodeForDAbid *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void
    swapHeadTail11(NodeForDAbid *inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTail21(NodeForDAbid *inter1, NodeForDAbid *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail22(NodeForDAbid *inter1, NodeForDAbid *inter2, NodeForDAbid *inter3, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    void
    swapHeadTailLen1Is1(NodeForDAbid **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void
    swapHeadTailLen2Is1(NodeForDAbid **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTailLen1Is2(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen2Is2(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAreBoth2(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2,
                                            int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAbid *DeleteMidPartOfTwoAdjacentLeaves(NodeForDAbid *leaf1, int startIndexInLeaf1, NodeForDAbid *leaf2, int endIndexInLeaf2);

    void SwapRangesAdjacentRightIs2(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                    int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SWapRangesAdjacentWhenLensAre1And2(NodeForDAbid *leftnode, NodeForDAbid *right1, NodeForDAbid *right2, int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapRangesAdjacentWhenRightIs1(NodeForDAbid **inter1, int numLeaf1, NodeForDAbid *right, int sartIndexLeaf1, int endIndexLeaf1,
                                   int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacent(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2, int sartIndexLeaf1,
                            int endIndexLeaf1,
                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre2(NodeForDAbid *left1, NodeForDAbid *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid *right1,
                              NodeForDAbid *right2,
                              int startIndexLeaf2, int endIndexLeaf2);

    void SWapRangesAdjacentWhenLensAreBoth1(NodeForDAbid *leftnode, NodeForDAbid *rightnode, int startIndexLeaf1, int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapRangesAdjacentWhenLeftIs1(NodeForDAbid *left, NodeForDAbid **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                                  int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentLeftIs2(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAre2And1(NodeForDAbid *left1, NodeForDAbid *left2, NodeForDAbid *right, int startIndexLeaf1,
                                            int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapWhenBothLensAre1(NodeForDAbid *leaf1, int startIndex1, int endIndex1, NodeForDAbid *leaf2, int startIndex2, int endIndex2);


    void
    SwapWhenLensAre1And2(NodeForDAbid *leaf1, int startIndex1, int endIndex1, NodeForDAbid *leaf2, int startIndex2, NodeForDAbid *leaf3,
                         int endIndex3);

    NodeForDAbid *coalesceNodesStopAtParent(NodeForDAbid *n, NodeForDAbid *neighbour, int neighbour_index);

    void SwapForTailHead(NodeForDAbid **inter1, NodeForDAbid **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                         int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenLenIs1(NodeForDAbid *LongLeaf, NodeForDAbid **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAbid *swapStartPointer(NodeForDAbid *leftLeaf, int endIndexInLeftLeaf, NodeForDAbid *rightLeaf, int endIndexInRightLeaf);

    int RangePosInLeafPointer(int start, int end, NodeForDAbid ***inter, int *startIndexLeaf, int *endIndexLeaf);

    void Move(int start, int end, int des);

    void RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid **inter1, int numLeaf1, NodeForDAbid *toleaf, int toindex);

    void MoveToLeafIsFirstLeaf(NodeForDAbid **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid *toleaf,
                               int toindex);

    void MoveToLeafIsLastLeaf(NodeForDAbid **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid *toleaf,
                              int toindex);

    void Move1(NodeForDAbid **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid *toleaf, int toindex);

    void
    MoveWhenLenIs1(NodeForDAbid **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid *toleaf, int toindex);

    void
    MoveWhenLenIs2(NodeForDAbid **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid *toleaf, int toindex);

    NodeForDAbid *CutForDesInMove(int des, int *toIndex);

    NodeForDAbid *insertOneIntoLeafAfterSplittingForMove(NodeForDAbid *leaf, int *newID, int length, int indexInLeaf, int *toIndex);

    void CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAbid **leafToDelete, int numLeafToDelete);

   // void Swap(int start1, int end1, int start2, int end2, int *CutArrayTime, int *FindLeafTime, int *swapLeafTime,
     //         int *CheckMinTime);

    void Swap(int start1, int end1, int start2, int end2);

    void insertIntoNodeForDAbid(NodeForDAbid *n, int leftIndex, int sumKeysRight, NodeForDAbid *right, bool needGoUpward = true);

    void insertOneIntoLeafAfterSplitting(NodeForDAbid *leaf, int *newPointer, int length, int indexInLeaf,
                                         bool needGoupwards = true);

    void AdjustAncestorKeyForOneNodeForDAbid(NodeForDAbid *c);

    void insertMulLeavesIntoLeafParentSplitOnce(NodeForDAbid *toParent, int startIndexInParent, NodeForDAbid **leafToInsert,
                                                int numLeafToInsert);

    bool
    insertKeysIntoLeaf(int *keysToInsert, NodeForDAbid **pointersToInsert, int numKeysToInsert, NodeForDAbid *toLeaf, int toIndex);

    bool insertMulKeysIntoLeafAfterOneSplitting(int *keysToInsert, NodeForDAbid **pointersToInsert, int numKeysToInsert,
                                                NodeForDAbid *toLeaf,
                                                int toIndex);

    void insertMulKeysIntoLeaf(int *keysToInsert, NodeForDAbid **pointersToInsert, int numKeysToInsert, NodeForDAbid *toLeaf,
                               int toIndex);

    void insertMulKeysIntoLeafAfterMulSplitting(int *keysToInsert, NodeForDAbid **pointersToInsert, int numKeysToInsert,
                                                NodeForDAbid *toLeaf, int toIndex);

    void SwapWhenLeftLenIs2(NodeForDAbid *left1, NodeForDAbid *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAbid **inter2,
                            int numLeaf2,
                            int startIndexLeaf2, int endIndexLeaf2);

    void
    ReplaceMidOfTwoAdjacentLeaves(NodeForDAbid *left, NodeForDAbid *right, int startIndexLeft, int endIndexRight, int *keysToInsert,
                                  NodeForDAbid **pointersToInsert, int numToInsert);

    void CheckAndMergeRedistributeNodeForDAbid(NodeForDAbid *n);

    void replaceKeysInLeaf(NodeForDAbid *leaf, int startIdx, int endIdx, int *keysToInsert, NodeForDAbid **pointersToInsert,
                           int numToInsert);
};

DAbidirectional * NewDAbidirectional(int *array, int numItems, int degree);

#endif //INDEXSTRUCTURE_DABIDIRECTIONAL_H
