/*
 * Adaptive Dynamic Array: keeps items in order, support query, update and order manipulation.
 * Order/Rank starts from 1.
 *
 *
 * Six user operations supported by ADA:
 * 1. query.
 *    Point query: int Query(int i). Given rank i, return i-th item.
 *    Eg, [a, b, c, d], Query(1) returns a.
 *    Range query: int* RangeQuery(int start, int end, int * lenOfAns). Given rank [start, end] inclusive, return these item.
 *    Eg, [a, b, c, d, e], RangeQuery(2, 4, &len), returns bcd, and len becomes 3.
 *    The third parameter int * lenOfAns actually serves as a return value. It is need in case [start, end] exceeds the whole sequence.
 * 2. insert
 *    void Insert(int newID, int i). Insert newID to be the i-th one.
 *    Eg, [a, b, c, d], Insert(e, 2), get [a, e, b, c, d].
 * 3. delete
 *    int Delete(int i). Delete the i-th item, return the value of deleted item.
 *    Eg, [a, b, c, d], Delete(3), get [a, b, d].
 * 4. swap
 *    void Swap(int start1, int end1, int start2, int end2).
 *    Swap two ranges [start1, end1] and [start2, end2]. Two ranges can have different lengths.
 *    Eg, [a,b,c,d,e,f,g,h,i], Swap(2, 4, 6, 7), get [a, f, g, e, b, c, d, h, i].
 *    Range 2-4 is bcd, range 6-7 is fg. Swap these two ranges without changing others.
 * 5. move
 *    void Move(int start, int end, int i).
 *    Move range[start, end] before currently the i-th item.
 *    Eg, [a, b, c, d, e, f, g], Move(2, 4, 6), get [a, e, b, c, d, f, g].
 *    Range is abc, currently 6th item is f, so move abc before f.
 * 6. reorder
 *    void Reorder(int start, int end, int *neworder).
 *    Change the order of items in range [start, end] to be neworder.
 *    Eg, [a, b, c, d, e, f], Reorder(2, 4, [dcb]), get [a, d, c, b, e, f].
 *
 *
 * Create and initialize ADA:
 *    DynamicArray * da =new DynamicArray(array, numItems, degree);
 *    array: sequence of items
 *    numItems: number of items in array
 *    degree: fanout of the tree
 *
 *
 *
 * unsolved problem: how to free space of arrays manually
 * because
 * 1. some part of the array is lost track of during array splitting
 * 2. Eg, int * a = new int[10], but the key corresponding to a was changed to 4 afterwards
 * in this case, delete []a will delete 10 int spaces rather than 4
 * which can make anther array deletion run into error
 *
 */


#ifndef INDEXSTRUCTURE_DYNAMICARRAY_H
#define INDEXSTRUCTURE_DYNAMICARRAY_H
#include <cstdio>
#include <iostream>
#include <cmath>
#include <list>
#include <chrono>
#include <string>
#include <queue>

class NodeDA {
public:
    NodeDA **Pointers; // pointers of children
    int *Keys; // keys are number of items in each subtree
    NodeDA *Parent; // pointer to parent
    bool IsLeaf; // whether this is a leaf node
    int NumOfKeys; // number of keys [fanout/2, fanout]
    NodeDA *Next; // like b+tree, all leaf nodes are linked together by this pointer
    explicit NodeDA(int capacity, int numkeys = 0, NodeDA * parent = nullptr, bool isleaf = false); // capacity means fanout
    NodeDA(int capacity, NodeDA * parent, std::string str, bool isleaf = false); // create a node based on string 3 6 0 0, which means four keys are 3,6,0,0
    ~NodeDA();
    int SumOfKeys() const; // return the sum of all keys
};


class DynamicArray {
public:

    /*
     * empty tree, root = nullptr, default capacity = 4
     */
    DynamicArray();

    /*
     * Create a new DynamicArray.
     * array is the initial sequence of items, numItems is the number of items (length of array), and capacity is fanout
     */
    DynamicArray(int * & array, int numItems, int capacity);

    /*
     * unsolved problem: how to free space of arrays manually
     * because
     * 1. some part of the array is lost track of during array splitting
     * 2. Eg, int * a = new int[10], but the key corresponding to a was changed to 4 afterwards
     * in this case, delete []a will delete 10 int spaces rather than 4
     * which can make anther array deletion run into error
     */
    ~DynamicArray();

    /*
     * For querying a range of items by order.
     * inclusive range with rank [start, end]
     * return an array of items (rowid) in this range
     * lenOfAns is set to be the length of this range. needed in case [start, end] exceeds the actual item sequence.
     * Eg, [a, b, c, d, e], RangeQuery(2, 4, &len), returns bcd, and len becomes 3.
     */
    int* RangeQuery(int start, int end, int * lenOfAns);

    /*
     * For querying only one item by its order.
     * return the item value whose rank is start.
     */
    int Query(int start) const;

    /*
     * For inserting a new item newID to be the pos-th one in the sequence.
     */
    void Insert(int newID, int pos);

    /*
     * For deleting the pos-th item.
     * Return the value of the deleted item.
     */
    int Delete(int pos);

    /*
     * Swap two inclusive ranges [start1, end1] and [start2, end2]. Two ranges can have different lengths.
     * Eg, [a,b,c,d,e,f,g,h,i], Swap(2, 4, 6, 7), get [a, f, g, e, b, c, d, h, i].
     * Range 2-4 is bcd, range 6-7 is fg. Swap these two ranges without changing others.
     */
    void Swap(int start1, int end1, int start2, int end2);

    /*
     * Move range [start, end] before rank des (destination).
     * des should be eigher after end, or before start.
     * Eg, [a, b, c, d, e, f, g], Move(2, 4, 6), get [a, e, b, c, d, f, g].
     * Range [2,4] is abc, currently 6th item is f, so move abc before f.
     */
    void Move(int start, int end, int des);

    /*
     * Reorder range [start, end] according to neworder.
     * Eg, [a, b, c, d, e, f], Reorder(2, 4, [dcb]), get [a, d, c, b, e, f].
     */
    void Reorder(int start, int end, int *neworder);

    /*
     * return the depth of tree in Dynamic Array, not including the arrays (rowid layer)
     * Eg, initially there is only one array and one node (root), so depth = 1.
     */
    int Depth() const;

    /*
     * print the tree, without arrays (rowid layer)
     */
    void PrintTree() const;

    /*
     * print the tree including arrays (rowid layer)
     */
    void PrintDA();

    /*
     * print all items as an array, eg, [a, b, c, d]
     */
    void PrintArray();

    /*
     * return fanout
     */
    int GetFanout() const;

private:
    int NumItems; // total number of items
    int Capacity; // fanout of a node
    int Min; // Min = ceiling(Capacity / 2) by default. A node must be at least half-full
    NodeDA *Root; // root of the tree

    /*
     * Used to locate the pos-th item.
     * Return the pointer to the leaf containing the pos-th item.
     * int *preIndexInLeaf, int *preIndexInArray serve as return values
     * preIndexInLeaf is the index of the array containing current pos-th item in the leaf
     * preIndexInArray is the index of the current pos-th item in the array
     * if pos = 0, preIndexInArray=preIndexInLeaf=-1, return the leaf containing the first item
     */
    NodeDA *findLeaf(int pos, int *preIndexInLeaf, int *preIndexInArray);

    /*
     * increase every key in the path from node to root by increase
     */
    void SimplyAdjustAncestorKeysBy(NodeDA *node, int increase);

    /*
     * increase/decrease keys in the path from node n to root,
     * the difference is calculated by studying the parent of n
     */
    void AdjustAncestorKeysAccordingToCurNode(NodeDA *n);

    /*
     * adjust ancestor nodes after delete
     * including merging and redistribution
     */
    NodeDA *CompleteAdjustAncestorNodesAfterDelete(NodeDA *n);

    /*
     * adjust from node fromLeaf to root
     * at the same time redistribute or merge
     */
    NodeDA *adjustAfterDeleteKeysInNode(NodeDA *fromLeaf, int numKeysDeleted = -1);

    void insertOneIntoLeaf(NodeDA *leaf, int *righta, int rightlen, int IndexInLeaf);

    void insertIntoParent(NodeDA *left, int sumKetsLeft, NodeDA *right, int sumKeysRight, bool needGoUpwards = true);

    void insertIntoNewRoot(NodeDA *left, int sumKeysLeft, NodeDA *right, int sumKeysRight);

    int getLeftIndex(NodeDA *parent, NodeDA *left);

    int NodeSumKeys(NodeDA *n);

    void
    insertIntoNodeAfterSplitting(NodeDA *n, int leftIndex, int sumKeysRight, NodeDA *right, bool needGoupwards = true);

    int pathToRoot(NodeDA *child) const;

    void deleteEntry(NodeDA *n, int indexInNode);

    NodeDA *removeEntryFromNode(NodeDA *n, int indexInNode);

    void adjustRoot();

    int getLeftNeighborIndex(NodeDA *n);

    NodeDA *coalesceNodes(NodeDA *n, NodeDA *neighbour, int neighbour_index);

    void redistributeNodes(NodeDA *n, NodeDA *neighbour, int neighbour_index);

    void coalesceNodesForDeleteOperation(NodeDA *n, NodeDA *neighbour, int neighbour_index);

    void deleteEntryForDeleteOperatoin(NodeDA *n, int indexInNode);

    void redistributeNodesForDeleteOperation(NodeDA *n, NodeDA *neighbour, int neighbour_index);


    void CutArray(int pos);

    void insertOneIntoLeafForCut(NodeDA *leaf, int *newarray, int length, int indexInLeaf);

    void MergeOrRedistributeArrayAfterDelete(NodeDA *leaf, int IndexInLeaf, int IndexInArray);

    void insertOneIntoLeafForDeletion(NodeDA *leaf, int *newa, int length, int indexInLeaf);

    void MergeOrRedistributeArrayAfterDeleteInMiddle(NodeDA *leaf, int IndexInLeaf, int IndexInArray);

    void CheckMinArrayAfterSwap(int s);

    void MergeRedistributeArrayAfterSwap(NodeDA *leaf, int IndexInLeaf);

    void deleteEntryForCheckMinArray(NodeDA *n, int indexInNode);

    void coalesceNodesForCheckMinArray(NodeDA *n, NodeDA *neighbour, int neighbour_index);

    void swapLeaf(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                  int startIndexLeaf2, int endIndexLeaf2);

    int getIndexInParent(NodeDA *n);

    NodeDA * deleteMulEntry(NodeDA **entryToDelete, int numEntryToDelete);

    void movePointerInNodeForward(NodeDA *node, int startIndex, int endIndex, int toIndex);

    //void PutLeavesIntoUpdateSumKeys(NodeDA **leaves, int numLeaf);

    void UpdateSumKeys(NodeDA **nodes, int numNode);

    NodeDA *movePointerBefore(NodeDA *fromLeaf, int startIndex, int endIndex, NodeDA *toLeaf, int toIndex);

    NodeDA *movePointerAfter(NodeDA *fromLeaf, int startIndex, int endIndex, NodeDA *toLeaf, int toIndex);

    void insertMulLeavesIntoLeafParentAfterSplitting(NodeDA *toParent, int startIndexInParent, NodeDA **leafToInsert,
                                                     int numLeafToInsert);

    void insertMulIntoParent(NodeDA *toParent, NodeDA **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNewRoot(NodeDA *toParent, NodeDA **parentNewSibling, int parentNewSiblingNum);

    void insertMulIntoNode(NodeDA *parent, int toIndex, NodeDA **parentNewSibling, int parentNewSiblingNum,
                           int ParentIncrease);

    void insertMulIntoNodeAfterSplitting(NodeDA *parent, int toIndex, NodeDA **pointerToInsert, int pointerToInsertNum);

    void redistributeNodesForCheckMinArray(NodeDA *n, NodeDA *neighbour, int neighbour_index);

    void
    insertLeavesIntoLeafParent(NodeDA *toParent, int startIndexInParent, NodeDA **leafToInsert, int numLeafToInsert);

    void
    insertMulLeavesIntoLeafParent(NodeDA *toParent, int startIndexInParent, NodeDA **leafToInsert, int numLeafToInsert);

    void swapEndPointer(NodeDA *leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf, NodeDA *rightLeaf,
                        int indexInRightLeaf, int endIndexInRightLeaf);

    void swapWholeLeafAndGoUpwards(NodeDA **wholeLeaf1, NodeDA **bwholeLeaf2, int w1, int w2);

    void exchangeLeaf(NodeDA *LeftLeaf, NodeDA *LeftParent, NodeDA *RightLeaf, NodeDA *RightParent);

    void RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeDA *node);

    void adjustAncestorKeysAfterExchangingLeaves(NodeDA **nodeArray, int numNodeInArray);

    void swapHeadTail12(NodeDA *inter1, NodeDA *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void
    swapHeadTail11(NodeDA *inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTail21(NodeDA *inter1, NodeDA *inter2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTail22(NodeDA *inter1, NodeDA *inter2, NodeDA *inter3, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    void
    swapHeadTailLen1Is1(NodeDA **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void
    swapHeadTailLen2Is1(NodeDA **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                        int endIndexLeaf2);

    void swapHeadTailLen1Is2(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void swapHeadTailLen2Is2(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                             int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAreBoth2(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2,
                                            int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    NodeDA *DeleteMidPartOfTwoAdjacentLeaves(NodeDA *leaf1, int startIndexInLeaf1, NodeDA *leaf2, int endIndexInLeaf2);

    void SwapRangesAdjacentRightIs2(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                    int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SWapRangesAdjacentWhenLensAre1And2(NodeDA *leftnode, NodeDA *right1, NodeDA *right2, int startIndexLeaf1,
                                            int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapRangesAdjacentWhenRightIs1(NodeDA **inter1, int numLeaf1, NodeDA *right, int sartIndexLeaf1, int endIndexLeaf1,
                                   int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacent(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2, int sartIndexLeaf1,
                            int endIndexLeaf1,
                            int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenBothLensAre2(NodeDA *left1, NodeDA *left2, int startIndexLeaf1, int endIndexLeaf1, NodeDA *right1,
                              NodeDA *right2,
                              int startIndexLeaf2, int endIndexLeaf2);

    void SWapRangesAdjacentWhenLensAreBoth1(NodeDA *leftnode, NodeDA *rightnode, int startIndexLeaf1, int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapRangesAdjacentWhenLeftIs1(NodeDA *left, NodeDA **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                                  int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentLeftIs2(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapRangesAdjacentWhenLensAre2And1(NodeDA *left1, NodeDA *left2, NodeDA *right, int startIndexLeaf1,
                                            int endIndexLeaf1,
                                            int startIndexLeaf2, int endIndexLeaf2);

    void
    SwapWhenBothLensAre1(NodeDA *leaf1, int startIndex1, int endIndex1, NodeDA *leaf2, int startIndex2, int endIndex2);


    void
    SwapWhenLensAre1And2(NodeDA *leaf1, int startIndex1, int endIndex1, NodeDA *leaf2, int startIndex2, NodeDA *leaf3,
                         int endIndex3);

    NodeDA *coalesceNodesStopAtParent(NodeDA *n, NodeDA *neighbour, int neighbour_index);

    void SwapForTailHead(NodeDA **inter1, NodeDA **inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                         int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2);

    void SwapWhenLenIs1(NodeDA *LongLeaf, NodeDA **inter2, int numLeaf2, int startIndexLeaf1, int endIndexLeaf1,
                        int startIndexLeaf2, int endIndexLeaf2);

    NodeDA *swapStartPointer(NodeDA *leftLeaf, int endIndexInLeftLeaf, NodeDA *rightLeaf, int endIndexInRightLeaf);

    int RangePosInLeafPointer(int start, int end, NodeDA ***inter, int *startIndexLeaf, int *endIndexLeaf);


    void RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeDA **inter1, int numLeaf1, NodeDA *toleaf, int toindex);

    void MoveToLeafIsFirstLeaf(NodeDA **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA *toleaf,
                               int toindex);

    void MoveToLeafIsLastLeaf(NodeDA **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA *toleaf,
                              int toindex);

    void Move1(NodeDA **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA *toleaf, int toindex);

    void
    MoveWhenLenIs1(NodeDA **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA *toleaf, int toindex);

    void
    MoveWhenLenIs2(NodeDA **inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA *toleaf, int toindex);

    NodeDA *CutForDesInMove(int des, int *toIndex);

    NodeDA *insertOneIntoLeafAfterSplittingForMove(NodeDA *leaf, int *newID, int length, int indexInLeaf, int *toIndex);

    void insertIntoNode(NodeDA *n, int leftIndex, int sumKeysRight, NodeDA *right, bool needGoUpward = true);

    void insertOneIntoLeafAfterSplitting(NodeDA *leaf, int *newPointer, int length, int indexInLeaf,
                                         bool needGoupwards = true);

    void AdjustAncestorKeyForOneNode(NodeDA *c);

    void insertMulLeavesIntoLeafParentSplitOnce(NodeDA *toParent, int startIndexInParent, NodeDA **leafToInsert,
                                                int numLeafToInsert);

    bool
    insertKeysIntoLeaf(int *keysToInsert, NodeDA **pointersToInsert, int numKeysToInsert, NodeDA *toLeaf, int toIndex);

    bool insertMulKeysIntoLeafAfterOneSplitting(int *keysToInsert, NodeDA **pointersToInsert, int numKeysToInsert,
                                                NodeDA *toLeaf,
                                                int toIndex);

    void insertMulKeysIntoLeaf(int *keysToInsert, NodeDA **pointersToInsert, int numKeysToInsert, NodeDA *toLeaf,
                               int toIndex);

    void insertMulKeysIntoLeafAfterMulSplitting(int *keysToInsert, NodeDA **pointersToInsert, int numKeysToInsert,
                                                NodeDA *toLeaf, int toIndex);

    void SwapWhenLeftLenIs2(NodeDA *left1, NodeDA *left2, int startIndexLeaf1, int endIndexLeaf1, NodeDA **inter2,
                            int numLeaf2,
                            int startIndexLeaf2, int endIndexLeaf2);

    void
    ReplaceMidOfTwoAdjacentLeaves(NodeDA *left, NodeDA *right, int startIndexLeft, int endIndexRight, int *keysToInsert,
                                  NodeDA **pointersToInsert, int numToInsert);

    void CheckAndMergeRedistributeNode(NodeDA *n);

    void replaceKeysInLeaf(NodeDA *leaf, int startIdx, int endIdx, int *keysToInsert, NodeDA **pointersToInsert,
                           int numToInsert);

    NodeDA *findPrevious(NodeDA *node);

    void CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeDA *preLeaf, NodeDA **leafToDelete,
                                                               int numLeafToDelete);

};

/*
 * Initialize a new ADA, given array, num of items, and fanout (degree)
 * Has exactly the same functionality as DynamicArray::DynamicArray
 * Keeps both this function and DynamicArray::DynamicArray because I used NewDynamicArray in experiments
 */
DynamicArray *NewDynamicArray(int * & array, int numItems, int degree);


#endif //INDEXSTRUCTURE_DYNAMICARRAY_H
