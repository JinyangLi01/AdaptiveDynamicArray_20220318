#ifndef INDEXSTRUCTURE_DANOLIMIT_H
#define INDEXSTRUCTURE_DANOLIMIT_H
#include <cstdio>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>
//typedef std::chrono::high_resolution_clock::time_point TimeVar;
//#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
//#define timeNow() std::chrono::high_resolution_clock::now()

class NodeForDAnolimit {
public:
    NodeForDAnolimit **Pointers;
    int *Keys;
    NodeForDAnolimit *Parent;
    bool IsLeaf;
    int NumOfKeys;
    NodeForDAnolimit *Next;
    //NodeForDAnolimit *Previous;
    explicit NodeForDAnolimit(int capacity, int numkeys = 0, NodeForDAnolimit * parent = nullptr);
    ~NodeForDAnolimit()
    {
        delete[]Keys;
        delete[]Pointers;
    }
};

class DAnolimit {
public:
    int NumItems;
    int Capacity;
    NodeForDAnolimit *Root;
    int Min;
    DAnolimit(int numItems, int capacity);
    ~DAnolimit();
    int *RangeQuery(int, int, int *) const;
    int NumOfItems() { return NumItems; }
    int Depth();
    void PrintTree();
    void PrintDA();
    void Insert(int, int);
    void Delete(int);

    NodeForDAnolimit *findLeaf(int pos, int *preIndexInLeaf, int *preIndexInArray) const;

    void SimplyAdjustAncestorKeysBy(NodeForDAnolimit *node, int increase) const;

    void AdjustAncestorKeysAccordingToCurNode(NodeForDAnolimit *n);

    NodeForDAnolimit *CompleteAdjustAncestorNodesAfterDelete(NodeForDAnolimit *n);

    NodeForDAnolimit *adjustAfterDeleteKeysInNode(NodeForDAnolimit *fromLeaf, int numKeysDeleted = -1);

    void insertOneIntoLeaf(NodeForDAnolimit *leaf, int *righta, int rightlen, int IndexInLeaf) const;

    void
    insertIntoParent(NodeForDAnolimit *left, int sumKetsLeft, NodeForDAnolimit *right, int sumKeysRight, bool needGoUpwards = true);

    void insertIntoNewRoot(NodeForDAnolimit *left, int sumKeysLeft, NodeForDAnolimit *right, int sumKeysRight);

    int getLeftIndex(NodeForDAnolimit *parent, NodeForDAnolimit *left) const;

    static int NodeSumKeys(NodeForDAnolimit *n);

    void insertIntoNodeAfterSplitting(NodeForDAnolimit *n, int leftIndex, int sumKeysRight, NodeForDAnolimit *right,
                                      bool needGoupwards = true);

    int pathToRoot(NodeForDAnolimit *child) const;

    void deleteEntry(NodeForDAnolimit *n, int indexInNode);

    static NodeForDAnolimit *removeEntryFromNode(NodeForDAnolimit *n, int indexInNode);

    void adjustRoot();

    int getLeftNeighborIndex(NodeForDAnolimit *n);

    NodeForDAnolimit *coalesceNodes(NodeForDAnolimit *n, NodeForDAnolimit *neighbour, int neighbour_index);

    void redistributeNodes(NodeForDAnolimit *n, NodeForDAnolimit *neighbour, int neighbour_index);

    void coalesceNodesForDeleteOperation(NodeForDAnolimit *n, NodeForDAnolimit *neighbour, int neighbour_index);

    void deleteEntryForDeleteOperatoin(NodeForDAnolimit *n, int indexInNode);

    void redistributeNodesForDeleteOperation(NodeForDAnolimit *n, NodeForDAnolimit *neighbour, int neighbour_index);

    void Reorder(int start, int end, int *neworder) const;

    void CutArray(int pos);

    static void insertOneIntoLeafForCut(NodeForDAnolimit *leaf, int *newarray, int length, int indexInLeaf);

    void MergeOrRedistributeArrayAfterDelete(NodeForDAnolimit *leaf, int IndexInLeaf, int IndexInArray);

    void insertOneIntoLeafForDeletion(NodeForDAnolimit *leaf, int *newa, int length, int indexInLeaf) const;

    void MergeOrRedistributeArrayAfterDeleteInMiddle(NodeForDAnolimit *leaf, int IndexInLeaf, int IndexInArray);

    //void CheckMinArrayAfterSwap(int s);

    //void MergeRedistributeArrayAfterSwap(NodeForDAnolimit *leaf, int IndexInLeaf);

    void deleteEntryForCheckMinArray(NodeForDAnolimit *n, int indexInNode);

    void coalesceNodesForCheckMinArray(NodeForDAnolimit *n, NodeForDAnolimit *neighbour, int neighbour_index);

    void
    swapLeaf(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
             int startIndexLeaf2, int endIndexLeaf2);

    int getIndexInParent(NodeForDAnolimit *n) const;

    NodeForDAnolimit * deleteMulEntry(NodeForDAnolimit **entryToDelete, int numEntryToDelete);

    static void movePointerInNodeForward(NodeForDAnolimit *node, int startIndex, int endIndex, int toIndex);

    //void PutLeavesIntoUpdateSumKeys(NodeForDAnolimit **leaves, int numLeaf);

    void UpdateSumKeys(NodeForDAnolimit **nodes, int numNode);

    NodeForDAnolimit *movePointerBefore(NodeForDAnolimit *fromLeaf, int startIndex, int endIndex, NodeForDAnolimit *toLeaf, int toIndex);

    NodeForDAnolimit *movePointerAfter(NodeForDAnolimit *fromLeaf, int startIndex, int endIndex, NodeForDAnolimit *toLeaf, int toIndex);

    void
    insertMulLeavesIntoLeafParentAfterSplitting(NodeForDAnolimit *toParent, int startIndexInParent, NodeForDAnolimit **leafToInsert,
                                                int numLeafToInsert);

    void insertMulIntoParent(NodeForDAnolimit *toParent, NodeForDAnolimit **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNewRoot(NodeForDAnolimit *toParent, NodeForDAnolimit **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNode(NodeForDAnolimit *parent, int toIndex, NodeForDAnolimit **parentNewSibling, int parentNewSiblingNum,
                           int ParentIncrease) const;

    void
    insertMulIntoNodeAfterSplitting(NodeForDAnolimit *parent, int toIndex, NodeForDAnolimit **pointerToInsert, int pointerToInsertNum);

    void redistributeNodesForCheckMinArray(NodeForDAnolimit *n, NodeForDAnolimit *neighbour, int neighbour_index);

    void insertLeavesIntoLeafParent(NodeForDAnolimit *toParent, int startIndexInParent, NodeForDAnolimit **leafToInsert,
                                    int numLeafToInsert);

    void insertMulLeavesIntoLeafParent(NodeForDAnolimit *toParent, int startIndexInParent, NodeForDAnolimit **leafToInsert,
                                       int numLeafToInsert);

    void swapEndPointer(NodeForDAnolimit *leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf, NodeForDAnolimit *rightLeaf,
                        int indexInRightLeaf, int endIndexInRightLeaf);

    void swapWholeLeafAndGoUpwards(NodeForDAnolimit **wholeLeaf1, NodeForDAnolimit **bwholeLeaf2, int w1, int w2);

    void exchangeLeaf(NodeForDAnolimit *LeftLeaf, NodeForDAnolimit *LeftParent, NodeForDAnolimit *RightLeaf, NodeForDAnolimit *RightParent) const;

    void RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAnolimit *node);

    void adjustAncestorKeysAfterExchangingLeaves(NodeForDAnolimit **nodeArray, int numNodeInArray);

    void swapHeadTail12(NodeForDAnolimit *inter1, NodeForDAnolimit *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail11(NodeForDAnolimit *inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail21(NodeForDAnolimit *inter1, NodeForDAnolimit *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail22(NodeForDAnolimit *inter1, NodeForDAnolimit *inter2, NodeForDAnolimit *inter3, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen1Is1(NodeForDAnolimit **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                             int startIndexLeaf2,
                             int endIndexLeaf2);

    void swapHeadTailLen2Is1(NodeForDAnolimit **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1,
                             int startIndexLeaf2,
                             int endIndexLeaf2);

    void swapHeadTailLen1Is2(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen2Is2(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAreBoth2(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2,
                                            int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAnolimit *
    DeleteMidPartOfTwoAdjacentLeaves(NodeForDAnolimit *leaf1, int startIndexInLeaf1, NodeForDAnolimit *leaf2, int endIndexInLeaf2);

    void
    SwapRangesAdjacentRightIs2(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                               int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SWapRangesAdjacentWhenLensAre1And2(NodeForDAnolimit *leftnode, NodeForDAnolimit *right1, NodeForDAnolimit *right2, int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenRightIs1(NodeForDAnolimit **inter1, int numLeaf1, NodeForDAnolimit *right, int sartIndexLeaf1,
                                        int endIndexLeaf1,
                                        int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacent(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2, int sartIndexLeaf1,
                            int endIndexLeaf1,
                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre2(NodeForDAnolimit *left1, NodeForDAnolimit *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit *right1,
                              NodeForDAnolimit *right2,
                              int startIndexLeaf2, int endIndexLeaf2);

    void
    SWapRangesAdjacentWhenLensAreBoth1(NodeForDAnolimit *leftnode, NodeForDAnolimit *rightnode, int startIndexLeaf1, int endIndexLeaf1,
                                       int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLeftIs1(NodeForDAnolimit *left, NodeForDAnolimit **inter2, int numLeaf2, int startIndexLeaf1,
                                       int endIndexLeaf1,
                                       int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapRangesAdjacentLeftIs2(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                              int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAre2And1(NodeForDAnolimit *left1, NodeForDAnolimit *left2, NodeForDAnolimit *right, int startIndexLeaf1,
                                            int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre1(NodeForDAnolimit *leaf1, int startIndex1, int endIndex1, NodeForDAnolimit *leaf2, int startIndex2,
                              int endIndex2);


    void
    SwapWhenLensAre1And2(NodeForDAnolimit *leaf1, int startIndex1, int endIndex1, NodeForDAnolimit *leaf2, int startIndex2, NodeForDAnolimit *leaf3,
                         int endIndex3);

    NodeForDAnolimit *coalesceNodesStopAtParent(NodeForDAnolimit *n, NodeForDAnolimit *neighbour, int neighbour_index);

    void SwapForTailHead(NodeForDAnolimit **inter1, NodeForDAnolimit **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                         int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenLenIs1(NodeForDAnolimit *LongLeaf, NodeForDAnolimit **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    NodeForDAnolimit *swapStartPointer(NodeForDAnolimit *leftLeaf, int endIndexInLeftLeaf, NodeForDAnolimit *rightLeaf, int endIndexInRightLeaf);

    int RangePosInLeafPointer(int start, int end, NodeForDAnolimit ***inter, int *startIndexLeaf, int *endIndexLeaf);

    void Move(int start, int end, int des);

    void
    RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit **inter1, int numLeaf1, NodeForDAnolimit *toleaf, int toindex);

    void
    MoveToLeafIsFirstLeaf(NodeForDAnolimit **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit *toleaf,
                          int toindex);

    void MoveToLeafIsLastLeaf(NodeForDAnolimit **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit *toleaf,
                              int toindex);

    void Move1(NodeForDAnolimit **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit *toleaf, int toindex);

    void MoveWhenLenIs1(NodeForDAnolimit **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit *toleaf,
                        int toindex);

    void MoveWhenLenIs2(NodeForDAnolimit **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit *toleaf,
                        int toindex);

    NodeForDAnolimit *CutForDesInMove(int des, int *toIndex);

    NodeForDAnolimit *
    insertOneIntoLeafAfterSplittingForMove(NodeForDAnolimit *leaf, int *newID, int length, int indexInLeaf, int *toIndex);

    //void
   // Swap(int start1, int end1, int start2, int end2, int *CutArrayTime, int *FindLeafTime, int *swapLeafTime,
     //    int *CheckMinTime);

    void Swap(int start1, int end1, int start2, int end2);

    static void insertIntoNode(NodeForDAnolimit *n, int leftIndex, int sumKeysRight, NodeForDAnolimit *right, bool needGoUpward = true);

    void insertOneIntoLeafAfterSplitting(NodeForDAnolimit *leaf, int *newPointer, int length, int indexInLeaf,
                                         bool needGoupwards = true);

    void AdjustAncestorKeyForOneNode(NodeForDAnolimit *c);

    void insertMulLeavesIntoLeafParentSplitOnce(NodeForDAnolimit *toParent, int startIndexInParent, NodeForDAnolimit **leafToInsert,
                                                int numLeafToInsert);

    bool insertKeysIntoLeaf(int *keysToInsert, NodeForDAnolimit **pointersToInsert, int numKeysToInsert, NodeForDAnolimit *toLeaf,
                            int toIndex);

    bool insertMulKeysIntoLeafAfterOneSplitting(const int *keysToInsert, NodeForDAnolimit **pointersToInsert, int numKeysToInsert,
                                                NodeForDAnolimit *toLeaf,
                                                int toIndex);

    void insertMulKeysIntoLeaf(const int *keysToInsert, NodeForDAnolimit **pointersToInsert, int numKeysToInsert, NodeForDAnolimit *toLeaf,
                               int toIndex);

    void insertMulKeysIntoLeafAfterMulSplitting(const int *keysToInsert, NodeForDAnolimit **pointersToInsert, int numKeysToInsert,
                                                NodeForDAnolimit *toLeaf, int toIndex);

    void SwapWhenLeftLenIs2(NodeForDAnolimit *left1, NodeForDAnolimit *left2, int startIndexLeaf1, int endIndexLeaf1, NodeForDAnolimit **inter2,
                            int numLeaf2,
                            int startIndexLeaf2, int endIndexLeaf2);

    void ReplaceMidOfTwoAdjacentLeaves(NodeForDAnolimit *left, NodeForDAnolimit *right, int startIndexLeft, int endIndexRight,
                                       int *keysToInsert,
                                       NodeForDAnolimit **pointersToInsert, int numToInsert);

    void CheckAndMergeRedistributeNode(NodeForDAnolimit *n);

    void replaceKeysInLeaf(NodeForDAnolimit *leaf, int startIdx, int endIdx, int *keysToInsert, NodeForDAnolimit **pointersToInsert,
                           int numToInsert);

    void PrintArray() const;

    NodeForDAnolimit *findPrevious(NodeForDAnolimit *node);

    void CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForDAnolimit *preLeaf, NodeForDAnolimit **leafToDelete,
                                                          int numLeafToDelete);

    int Query(int pos);
};

DAnolimit * NewDAnolimit(int * array, int numItems, int degree );
#endif //INDEXSTRUCTURE_DANOLIMIT_H
