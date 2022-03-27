#ifndef INDEXSTRUCTURE_COUNTEDBTREE_H
#define INDEXSTRUCTURE_COUNTEDBTREE_H

class NodeForCBT {
public:
    NodeForCBT ** Pointers;
    int * Keys;
    int * KeysNumInChild;
    NodeForCBT * Parent  ;
    bool IsLeaf  ;
    int NumOfKeys ;
    explicit NodeForCBT(int capacity, int numkeys = 0, NodeForCBT *parent = nullptr, bool isLeaf = false);
    ~NodeForCBT(){delete []Keys; delete []Pointers; delete []KeysNumInChild;}
};

class CountedBtree {
public:
    int NumKeys{};
    int Capacity{};
    NodeForCBT *Root{};
    CountedBtree() = default;
    explicit CountedBtree(int capacity) {
        NumKeys = 0;
        Capacity = capacity;
        Root = nullptr;
    }
    ~CountedBtree();

    void PrintTree() const;

    void Insert(int rowID, int pos);

    NodeForCBT *findLeaf(int pos, int *IndexToInsertInLeaf) const;

    static NodeForCBT *FindLastKeyInLeaf(NodeForCBT *n, int *indexInLeaf);

    int getLeftIndex(NodeForCBT *parent, NodeForCBT *left) const;

    void insertIntoLeaf(NodeForCBT *leaf, int key, int indexInLeaf) const;

    void insertIntoNodeForCBT(NodeForCBT *n, int left_index, int sumKeysRight, NodeForCBT *right, int KeyToLift) const;

    void insertIntoLeafAfterSplitting(NodeForCBT *leaf, int key, int indexInLeaf);

    void insertIntoNodeForCBTAfterSplitting(NodeForCBT *parent, int left_index, int sumKeysRight, NodeForCBT *right, int newKey);

    void insertIntoParent(NodeForCBT *left, int sumKeysLeft, NodeForCBT *right, int sumKeysRight, int KeyToLift);

    void insertIntoNewRoot(NodeForCBT *left, int sumKeysLeft, NodeForCBT *right, int sumKeysRight, int newkey);

    void startNewTree(int rowID);

    static int getLeftNeighborIndex(NodeForCBT *n);

    static int getRightNeighborIndex(NodeForCBT *n);

    // void adjustRoot();

    void coalesceNodeForCBTs(NodeForCBT *n, NodeForCBT *neighbour, int neighbour_index, int flag);

    void coalesceLeafForCBTs(NodeForCBT *n, NodeForCBT *neighbour, int neighbour_index, int flag);

    static int getIndexInParent(NodeForCBT *n);

    static int ComputeNumItems(NodeForCBT *n);

    void Replace(int pos, int newKey) const;

    void Reorder(int start, int end, int *newIDs) const;

    void Swap(int start1, int end1, int start2, int end2);

    NodeForCBT *FindNodeForCBT(int pos, int *index) const;

    void Move(int start, int end, int des);

    NodeForCBT *removeEntryFromNodeForCBT(NodeForCBT *n, int indexInNodeForCBT) const;

    void redistributeNodeForCBTs(NodeForCBT *n, NodeForCBT *neighbour, int neighbour_index, int flag) const;

    void redistributeLeafForCBTs(NodeForCBT *n, NodeForCBT *neighbour, int neighbour_index, int flag) const;

    int Query(int pos) const;

    void Exchange(int a, int b) const;

    void Delete(int rank);

    void DeleteFromNodeForCBT(NodeForCBT *n, int idx);

    void DeleteFromLeafForCBT(NodeForCBT *n, int idx);

    void adjustRootInDeletion();

    int *RangeQuery(int start, int end, int *lenOfAns) const;
};



CountedBtree* NewCBTreeForArray(int order, int num);
#endif //INDEXSTRUCTURE_COUNTEDBTREE_H
