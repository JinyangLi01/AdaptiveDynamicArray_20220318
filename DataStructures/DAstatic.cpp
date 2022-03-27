#include "DAstatic.h"

NodeForStree::NodeForStree(int capacity, int numkeys, NodeForStree * parent) {
    NumOfKeys = numkeys;
    Parent = parent;
    Pointers = new NodeForStree*[capacity];
    Keys = new int[capacity];
    Next = nullptr;
    //Previous = nullptr;
    IsLeaf = false;
}

DAstatic * NewStree(int * array, int numItems, int degree ){
    DAstatic * da =new DAstatic(0, degree);
    if (array == nullptr) {
        da->Root = nullptr;
        return da;
    }
    for (int i = 0; i < numItems; ++i) {
        da->Insert(array[i], i+1);
    }
    return da;
}

DAstatic::~DAstatic() {
    std::list<NodeForStree *> toDelete;
    std::list<NodeForStree *> toFindChild;
    toFindChild.push_back(Root);
    while (!toFindChild.empty()) {
        NodeForStree * ele = toFindChild.front();
        if (ele != nullptr) {
            toFindChild.pop_front();
            if (ele->IsLeaf) {
                for (int i = 0; i < ele->NumOfKeys; ++i) {
                    int * s = reinterpret_cast<int *>(ele->Pointers[i]);
                    delete []s;
                }
                delete ele;
            } else {
                for (int i = 0; i < ele->NumOfKeys; ++i) {
                    toFindChild.push_back(ele->Pointers[i]);
                }
                toDelete.push_back(ele);
            }
        }
    }
    while (!toDelete.empty()) {
        NodeForStree * ele = toDelete.front();
        toDelete.pop_front();
        delete ele;
    }
}

void DAstatic::PrintTree() const {
    printf("\n");
    std::list<NodeForStree *> lst;
    int i = 0, rank = 0, newRank = 0;
    if (Root == nullptr) {
        printf("Empty Tree\n");
        return;
    }
    lst.push_back(Root);
    while (!lst.empty()) {
        NodeForStree * ele = lst.front();
        if (ele != nullptr) {
            lst.pop_front();
            if (ele->Parent != nullptr) {
                rank = newRank;
                newRank = pathToRoot(ele);
                if (newRank != rank) {
                    printf("\n");
                }
            }
            for (i = 0; i < ele->NumOfKeys; ++i) {
                printf("%d ", ele->Keys[i]);
            }
            for (; i < Capacity; i++) {
                printf("0 ");
            }
            if (!ele->IsLeaf) {
                for (i = 0; i < ele->NumOfKeys; ++i) {
                    if (ele->Keys[i] != 0) {
                        NodeForStree * c = ele->Pointers[i];
                        lst.push_back(c);
                    }
                }
            }
            printf(" | ");
        }
    }
    printf("\n");
}

int DAstatic::pathToRoot(NodeForStree * child) const {
    int length = 0;
    NodeForStree * c = child;
    while (c != Root) {
        c = c->Parent;
        length ++;
    }
    return length;
}

//make a leaf with capacity
NodeForStree * makeLeafForStree(int capacity) {
    NodeForStree * n = new NodeForStree(capacity);
    n->IsLeaf = true;
    return n;
}

int DAstatic::Depth() {
    if (Root == nullptr) {
        return 0;
    }
    int ans = 0;
    NodeForStree * c = Root;
    while (!c->IsLeaf) {
        ans ++;
        c = c->Pointers[0];
    }
    ans++;
    return ans;
}

//okstree
void DAstatic::Insert(int newID, int pos) {
    NumItems ++;
    if (pos > NumItems + 1) {
        pos = NumItems + 1;
    }
    if (Root == nullptr) {
        Root = makeLeafForStree(Capacity);
        Root->Keys[0] = newID;
        Root->NumOfKeys = 1;
        return;
    }
    NodeForStree * leaf;
    int preIndexInLeaf;
    leaf = findLeaf(pos-1, &preIndexInLeaf);
    if (leaf->NumOfKeys < Capacity) {
        insertOneIntoLeaf(leaf, newID, preIndexInLeaf+1);
    } else {
        insertOneIntoLeafAfterSplitting(leaf, newID, preIndexInLeaf+1);
    }
}

void DAstatic::insertMulKeysIntoLeaf(int * keysToInsert, int numKeysToInsert, NodeForStree* toLeaf, int toIndex) {
    int newNumKeys = numKeysToInsert + toLeaf->NumOfKeys;
    int startInsertedIdx = toIndex, endInsertedIdx = startInsertedIdx + numKeysToInsert - 1;
    for (int l = newNumKeys - 1; l > endInsertedIdx; --l) {
        toLeaf->Keys[l] = toLeaf->Keys[l-numKeysToInsert];
    }
    for (int i = startInsertedIdx; i <= endInsertedIdx; ++i) {
        toLeaf->Keys[i] = keysToInsert[i-startInsertedIdx];
    }
    toLeaf->NumOfKeys += numKeysToInsert;
    AdjustAncestorKeysAccordingToCurNode(toLeaf);
}

//only split once
//note that in some cases, we inserting into beginning/end of a node, mostly happening in swap/move
//when insert into the beginning, we need to make pointer toLeaf still point to the second leaf
//return true if toIndex == 0
bool DAstatic::insertMulKeysIntoLeafAfterOneSplitting(int * keysToInsert, int numKeysToInsert, NodeForStree* toLeaf, int toIndex) {
    int numGoRight = toLeaf->NumOfKeys - toIndex;
    int * keysGoRight = new int[numGoRight];
    for (int i = 0; i < numGoRight; ++i) {
        keysGoRight[i] = toLeaf->Keys[i+toIndex];
    }
    int totalNum = toLeaf->NumOfKeys + numKeysToInsert;
    int leftNum = totalNum/2, rightNum = totalNum - leftNum;
    int toInsertIdx = 0, goRightIdx = 0;
    int leftidx = toIndex, rightidx = 0;
    for (; leftidx < leftNum; ++leftidx) {
        toLeaf->Keys[leftidx] = keysToInsert[toInsertIdx];
        toInsertIdx++;
        if (toInsertIdx == numKeysToInsert) {
            leftidx++;
            break;
        }
    }
    for (; leftidx < leftNum; ++leftidx) {
        toLeaf->Keys[leftidx] = keysGoRight[goRightIdx];
        goRightIdx++;
    }
    NodeForStree * newLeaf = makeLeafForStree(Capacity);
    for (int k = leftNum; k < toIndex; ++k) {
        newLeaf->Pointers[rightidx] = toLeaf->Pointers[k];
        newLeaf->Keys[rightidx] = toLeaf->Keys[k];
        rightidx++;
    }
    for (; toInsertIdx < numKeysToInsert; ++toInsertIdx) {
        newLeaf->Keys[rightidx] = keysToInsert[toInsertIdx];
        rightidx++;
    }
    for (; goRightIdx < numGoRight; ++goRightIdx) {
        newLeaf->Keys[rightidx] = keysGoRight[goRightIdx];
        rightidx++;
    }
    toLeaf->NumOfKeys = leftNum;
    newLeaf->NumOfKeys = rightNum;
    newLeaf->Parent = toLeaf->Parent;
    newLeaf->Next = toLeaf->Next;
    /*
    if (newLeaf->Next != nullptr) {
        newLeaf->Next->Previous = newLeaf;
    }
     */
    toLeaf->Next = newLeaf;
    //newLeaf->Previous = toLeaf;
    insertIntoParent(toLeaf, toLeaf->NumOfKeys, newLeaf, newLeaf->NumOfKeys);
    if (toIndex == 0) {
        return true;
    }
    return false;
}

//okstree
void DAstatic::insertOneIntoLeaf(NodeForStree * leaf, int newID, int indexInLeaf) const {
    int keyToStore = newID;
    leaf->NumOfKeys ++;
    int tmpKey;
    int i = indexInLeaf;
    for (; i < leaf->NumOfKeys - 1; i++) {
        tmpKey = leaf->Keys[i];
        leaf->Keys[i] = keyToStore;
        keyToStore = tmpKey;
    }
    leaf->Keys[i] = keyToStore;

    if (leaf == Root) {
        return;
    }
    NodeForStree * c = leaf;
    NodeForStree * parent = c->Parent;
    while (parent != nullptr) {
        for (int j = 0; j < parent->NumOfKeys; ++j) {
            if (parent->Pointers[j] == c) {
                parent->Keys[j] ++;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

//okstree
void DAstatic::insertOneIntoLeafAfterSplitting(NodeForStree * leaf, int newID, int indexInLeaf, bool needGoupwards) {
    NodeForStree * newLeaf;
    int split=0, i=0;
    newLeaf = makeLeafForStree(Capacity);
    split = Min;
    leaf->NumOfKeys = split;
    newLeaf->NumOfKeys = Capacity + 1 - split;
    if (indexInLeaf < split) {// new key is in leaf (old leaf)
        int j = Capacity - 1;
        for (i = newLeaf->NumOfKeys - 1; i >= 0 ; --i) {
            newLeaf->Keys[i] = leaf->Keys[j];
            j--;
        }
        for (i = split - 1; i > indexInLeaf ; --i) {
            leaf->Keys[i] = leaf->Keys[i-1];
        }
        leaf->Keys[i] = newID;
        i--;
    } else {// new key is in new_leaf
        indexInLeaf -= split;
        for (i = 0; i < newLeaf->NumOfKeys ; ++i) {
            if (i == indexInLeaf) {
                newLeaf->Keys[i] = newID;
                break;
            } else {
                newLeaf->Keys[i] = leaf->Keys[i + split];
            }
        }
        i++;
        for (; i < newLeaf->NumOfKeys; i++) {
            newLeaf->Keys[i] = leaf->Keys[i-1+split];
        }
    }
    newLeaf->Parent = leaf->Parent;
    newLeaf->Next = leaf->Next;
    /*
    if (newLeaf->Next != nullptr) {
        newLeaf->Next->Previous = newLeaf;
    }
    newLeaf->Previous = leaf;
     */
    leaf->Next = newLeaf;
    insertIntoParent(leaf, leaf->NumOfKeys, newLeaf, newLeaf->NumOfKeys, needGoupwards);
}

//insert right after left
void DAstatic::insertIntoParent(NodeForStree * left, int sumKeysLeft, NodeForStree* right, int sumKeysRight, bool needGoupwards) {
    NodeForStree * parent = left->Parent;
    if (parent == nullptr) {
        insertIntoNewRoot(left, sumKeysLeft, right, sumKeysRight);
        return;
    }
    int leftIndex = getLeftIndex(parent, left);
    parent->Keys[leftIndex] = sumKeysLeft;
    if (parent->NumOfKeys < Capacity) {
        insertIntoNode(parent, leftIndex, sumKeysRight, right, needGoupwards);
        return;
    }
    insertIntoNodeAfterSplitting(parent, leftIndex, sumKeysRight, right, needGoupwards);
}

// insert right after leftIndex, modify right key and go upwards if needed
void DAstatic::insertIntoNode(NodeForStree * n, int leftIndex, int sumKeysRight, NodeForStree * right, bool needGoUpward) {
    n->NumOfKeys ++;
    int i = 0;
    int keyToStore = sumKeysRight;
    NodeForStree * pointerToStore = right;
    right->Parent = n;
    i = leftIndex + 1;
    for (; i < n->NumOfKeys - 1; i++) {
        int tmpKey = n->Keys[i];
        NodeForStree * tmpPointer = n->Pointers[i];
        n->Keys[i] = keyToStore;
        n->Pointers[i] = pointerToStore;
        keyToStore = tmpKey;
        pointerToStore = tmpPointer;
    }
    n->Keys[i] = keyToStore;
    n->Pointers[i] = pointerToStore;
    if (!needGoUpward) {
        return;
    }
    int delta = 0;
    NodeForStree * c = n;
    NodeForStree * parent = c->Parent;
    if (parent == nullptr) {
        return;
    }
    for (int y = 0; y < parent->NumOfKeys; ++y) {
        if (parent->Pointers[y] == c) {
            int x = NodeSumKeys(c);
            delta = x - parent->Keys[y];
            parent->Keys[y] = x;
            break;
        }
    }
    c = c->Parent;
    parent = c->Parent;
    while(parent != nullptr) {
        for (i = 0; i < parent->NumOfKeys; ++i) {
            if (parent->Pointers[i] == c) {
                parent->Keys[i] += delta;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

void DAstatic::insertIntoNodeAfterSplitting(NodeForStree * parent, int leftIndex, int sumKeysRight, NodeForStree * right, bool needGoUpwards) {
    int i, j, leftSumKeys = 0, rightSumKeys = 0;
    NodeForStree * newNode;
    int * tempKeys;
    NodeForStree** tempPointers;
    tempPointers = new NodeForStree*[Capacity + 1];
    tempKeys = new int[Capacity+1];
    j = 0;
    for (i = 0; i < parent->NumOfKeys; ++i) {
        if (j == leftIndex + 1) {
            j ++;
        }
        tempPointers[j] = parent->Pointers[i];
        tempKeys[j] = parent->Keys[i];
        j++;
    }
    tempPointers[leftIndex + 1] =right;
    tempKeys[leftIndex + 1] = sumKeysRight;
    newNode = new NodeForStree(Capacity);
    for ( i = 0; i < Min; ++i) {
        leftSumKeys += tempKeys[i];
        parent->Pointers[i] = tempPointers[i];
        if (tempPointers[i] != nullptr) {
            tempPointers[i]->Parent = parent;
        }
        parent->Keys[i] = tempKeys[i];
    }
    parent->NumOfKeys = Min;
    j = 0;
    for (; i < Capacity + 1; i++) {
        rightSumKeys += tempKeys[i];
        newNode->Pointers[j] = tempPointers[i];
        if (tempPointers[i] != nullptr) {
            tempPointers[i]->Parent = newNode;
        }
        newNode->Keys[j] = tempKeys[i];
        j ++;
    }
    newNode->NumOfKeys = j;
    newNode->Parent = parent->Parent;
    insertIntoParent(parent, leftSumKeys, newNode, rightSumKeys, needGoUpwards);
}

int DAstatic::NodeSumKeys(NodeForStree * n) {
    if (n->IsLeaf) {
        return n->NumOfKeys;
    }
    int ans = 0;
    for (int i = 0; i < n->NumOfKeys; ++i) {
        ans += n->Keys[i];
    }
    return ans;
}

int DAstatic::getLeftIndex(NodeForStree * parent, NodeForStree * left) const {
    int leftIndex = 0;
    while(leftIndex < Capacity && parent->Pointers[leftIndex] != left) {
        leftIndex += 1;
    }
    return leftIndex;
}

void DAstatic::insertIntoNewRoot(NodeForStree * left, int sumKeysLeft, NodeForStree* right, int sumKeysRight) {
    Root = new NodeForStree(Capacity);
    Root->Keys[0] = sumKeysLeft;
    Root->Pointers[0] = left;
    Root->Keys[1] = sumKeysRight;
    Root->Pointers[1] = right;
    Root->Parent = nullptr;
    left->Parent = Root;
    right->Parent = Root;
    Root->NumOfKeys = 2;
}

//return leaf, preIndexInLeaf
NodeForStree * DAstatic::findLeaf(int pos, int * preIndexInLeaf) const {
    if (pos > NumItems) {
        pos = NumItems;
    }
    bool flag = false;
    NodeForStree * c = Root;
    int p = pos;
    if (pos == 0) {
        p = 1;
        flag = true;
    }
    if (c == nullptr) {
        return c;
    }
    int i;
    while (!c->IsLeaf) {
        for (i = 0; i < c->NumOfKeys; ++i) {
            if (p <= c->Keys[i]) {
                c = c->Pointers[i];
                break;
            }
            p -= c->Keys[i];
        }
    }
    if (flag) {
        *preIndexInLeaf = -1;
        return c;
    }
    *preIndexInLeaf = p-1;
    return c;
}

int DAstatic::Query(int pos) const {
    if (pos > NumItems) {
        pos = NumItems;
    }
    int idxInLeaf;
    NodeForStree * cur = findLeaf(pos, &idxInLeaf);
    return cur->Keys[idxInLeaf];
}


//return length of ans, ans is answer array
int* DAstatic::RangeQuery(int start, int end, int * lenOfAns) const {
    int dastart = 1;
    int daend = NumItems;
    int realstart = start;
    int realend = end;
    if (realstart < dastart) {
        realstart = dastart;
    }
    if (realend > daend) {
        realend = daend;
    }
    int realnum = realend - realstart + 1;
    *lenOfAns = realnum;
    if (realnum <= 0) {
        return nullptr;
    }
    int *ans = new int[realnum];
    int idxInLeaf;
    NodeForStree * cur = findLeaf(realstart, &idxInLeaf);
    for (int i = 0; i < realnum; i++) {
        ans[i] = cur->Keys[idxInLeaf];
        idxInLeaf++;
        if (idxInLeaf == cur->NumOfKeys) {
            cur = cur->Next;
            idxInLeaf = 0;
        }
    }
    return ans;
}

//okstree
void DAstatic::Delete(int pos) {
    if (pos > NumItems) {
        pos = NumItems;
    }
    int IndexInLeaf;
    NodeForStree * leaf = findLeaf(pos, &IndexInLeaf);
    deleteEntryForDeleteOperatoin(leaf, IndexInLeaf);
    NumItems--;
}


//delete an entry and adjust ancestors
void DAstatic::deleteEntry(NodeForStree * n, int indexInNode) {
    int neighbour_index = 0, capacity = 0;
    NodeForStree * neighbour;
    int numKeysDeleted = n->Keys[indexInNode];
    n = removeEntryFromNode(n, indexInNode);
    n->NumOfKeys--;
    if (n == Root) {
        adjustRoot();
        return;
    }
    if (n->NumOfKeys >= Min) {
        AdjustAncestorKeysAccordingToCurNode(n);
        return;
    }

    neighbour_index = getLeftNeighborIndex(n);
    if (neighbour_index == -1) {
        neighbour = n->Parent->Pointers[1];
    } else {
        neighbour = n->Parent->Pointers[neighbour_index];
    }
    if (neighbour->NumOfKeys + n->NumOfKeys <= Capacity) {
        coalesceNodes(n, neighbour, neighbour_index);
        return;
    } else {
        redistributeNodes(n, neighbour, neighbour_index);
        return;
    }
}


//merge into the right node
NodeForStree * DAstatic::coalesceNodes(NodeForStree * n, NodeForStree * neighbour, int neighbour_index) {
    int i, j;
    NodeForStree * tmp;
    int n_index = neighbour_index + 1;
    if (neighbour_index == -1) {
        tmp = n;
        n = neighbour;
        neighbour = tmp;
        n_index = 1;
        neighbour_index = 0;
    }
    i = neighbour->NumOfKeys;
    int newParentKey = NodeSumKeys(neighbour) + NodeSumKeys(n);
    for (j = 0; j < n->NumOfKeys; j++) {
        neighbour->Keys[i] = n->Keys[j];
        neighbour->Pointers[i] = n->Pointers[j];
        if (neighbour->Pointers[i] != nullptr) {
            if (!neighbour->IsLeaf){
                tmp = neighbour->Pointers[i];
                tmp->Parent = neighbour;
            }
        }
        i += 1;
    }
    neighbour->NumOfKeys = neighbour->NumOfKeys + n->NumOfKeys;
    neighbour->Parent->Keys[neighbour_index] = newParentKey;
    n->NumOfKeys = 0;
    if (n->IsLeaf) {
        neighbour->Next = n->Next;
        /*
        if (neighbour->Next != nullptr) {
            neighbour->Next->Previous = neighbour;
        }
         */
    }
    deleteEntry(neighbour->Parent, n_index);
    return neighbour;
}

int DAstatic::getLeftNeighborIndex(NodeForStree * n) const {
    int i = 0, nei = -1;
    NodeForStree * parent = n->Parent;
    for (i = 0; i < Capacity; ++i) {
        if (parent->Pointers[i] == n) {
            return nei;
        }
        if (parent->Pointers[i] != nullptr) {
            nei = i;
        }
    }
    return i;
}


void DAstatic::redistributeNodes(NodeForStree * n, NodeForStree * neighbour, int neighbour_index) {
    int i, leftNumKeys, rightNumKeys, leftIndex, leftSumKeys = 0, rightSumKeys = 0;
    if (neighbour_index != -1) { // left neighbour
        int numGoRight = neighbour->NumOfKeys - Min;
        leftIndex = neighbour_index;
        //int indexToGoRight = Min;
        leftNumKeys = Min;
        rightNumKeys = n->NumOfKeys + numGoRight;
        for (int j = rightNumKeys - 1; j >= numGoRight ; --j) {
            n->Pointers[j] = n->Pointers[j - numGoRight];
            n->Keys[j] = n->Keys[j - numGoRight];
        }
        i = neighbour->NumOfKeys - 1;
        for (int j = numGoRight - 1; j >= 0 ; --j) {
            n->Pointers[j] = neighbour->Pointers[i];
            n->Keys[j] = neighbour->Keys[i];
            if (n->Pointers[j] != nullptr) {
                if (!n->IsLeaf) {
                    NodeForStree * s = n->Pointers[j];
                    s->Parent = n;
                }
            }
            i--;
        }
        n->NumOfKeys = rightNumKeys;
        neighbour->NumOfKeys = leftNumKeys;
        leftSumKeys = NodeSumKeys(neighbour);
        rightSumKeys = NodeSumKeys(n);
    } else { // right neighbour
        leftIndex = 0;
        int j = 0;
        for (i = neighbour->NumOfKeys - 1; i >= 0; i--) {
            j++;
            if (j == Min) {
                break;
            }
        }
        int indexToGoLeft = i - 1;
        rightNumKeys = Min;
        int tmp_j = n->NumOfKeys;
        for (i = 0; i <= indexToGoLeft; i++) {
            n->Pointers[tmp_j] = neighbour->Pointers[i];
            n->Keys[tmp_j] = neighbour->Keys[i];
            if (n->Pointers[tmp_j] != nullptr) {
                if (!n->IsLeaf) {
                    NodeForStree * s = n->Pointers[tmp_j];
                    s->Parent = n;
                }
            }
            tmp_j++;
        }
        leftNumKeys = tmp_j;
        int k = indexToGoLeft + 1;
        for (i = 0; i < Min; i++) {
            neighbour->Keys[i] = neighbour->Keys[k];
            neighbour->Pointers[i] = neighbour->Pointers[k];
            neighbour->Keys[k] = 0;
            neighbour->Pointers[k] = nullptr;
            k++;
        }
        neighbour->NumOfKeys = rightNumKeys;
        n->NumOfKeys = leftNumKeys;
        leftSumKeys = NodeSumKeys(n);
        rightSumKeys = NodeSumKeys(neighbour);
    }

    NodeForStree * c = n->Parent;
    if (n->IsLeaf && n->Pointers[0] == nullptr) {
        c->Keys[leftIndex] = leftNumKeys;
        c->Keys[leftIndex+1] = rightNumKeys;
    } else {
        c->Keys[leftIndex] = leftSumKeys ;
        c->Keys[leftIndex+1] = rightSumKeys;
    }
    AdjustAncestorKeysAccordingToCurNode(c);
}

void DAstatic::adjustRoot() {
    NodeForStree * newRoot = nullptr;
    if (Root->NumOfKeys > 1) {
        return;
    }
    if (!Root->IsLeaf) {
        for (int i = 0; i < Capacity; i++) { // don't need this loop
            if (Root->Pointers[i] != nullptr) {
                newRoot = Root->Pointers[i];
                break;
            }
        }
        newRoot->Parent = nullptr;
        Root = newRoot;
    }
}

//okstree
// only remove an entry from node, doing nothing else
NodeForStree * DAstatic::removeEntryFromNode(NodeForStree * n, int indexInNode) {
    n->Keys[indexInNode] = 0;
    if (n->Pointers[indexInNode] != nullptr) {
        if (!n->IsLeaf) {
            NodeForStree * deleted = n->Pointers[indexInNode];
            deleted->Parent = nullptr;
            n->Pointers[indexInNode] = nullptr;
            //delete deleted;
        }
    }
    int j = indexInNode;
    for (int i = j + 1; i < n->NumOfKeys; i++) {
        n->Keys[j] = n->Keys[i];
        n->Pointers[j] = n->Pointers[i];
        j++;
    }
    return n;
}

//okstree
void DAstatic::Reorder(int start, int end, int* neworder) const {
    if (end > NumItems) {
        printf("Error: index exceeds limit!\n");
        return;
    }
    int length = end-start+1, newIdx = 0;
    int indexInLeaf;
    NodeForStree * cur = findLeaf(start, &indexInLeaf);
    for (int j = 0; j < length; ++j) {
        cur->Keys[indexInLeaf] = neworder[newIdx];
        indexInLeaf++;
        newIdx++;
        if (indexInLeaf == cur->NumOfKeys) {
            cur = cur->Next;
            indexInLeaf = 0;
        }
    }
}

void DAstatic::coalesceNodesForDeleteOperation(NodeForStree * n, NodeForStree * neighbour, int neighbour_index) {
    int i, j, neighbour_insertion_index;
    NodeForStree * tmp;
    int n_index = neighbour_index + 1;
    if (neighbour_index == -1) {
        tmp = n;
        n = neighbour;
        neighbour = tmp;
        n_index = 1;
        neighbour_index = 0;
    }
    neighbour_insertion_index = neighbour->NumOfKeys;
    i = neighbour_insertion_index;
    int newParentKey = NodeSumKeys(neighbour) + NodeSumKeys(n);
    for (j = 0; j < n->NumOfKeys; j++) {
        neighbour->Keys[i] = n->Keys[j];
        neighbour->Pointers[i] = n->Pointers[j];
        if (!neighbour->IsLeaf) {
            tmp = neighbour->Pointers[i];
            if (tmp != nullptr) {
                tmp->Parent = neighbour;
            }
        }
        i += 1;
    }
    neighbour->NumOfKeys = neighbour->NumOfKeys + n->NumOfKeys;
    neighbour->Parent->Keys[neighbour_index] = newParentKey;
    if (neighbour->IsLeaf) {
        neighbour->Next = n->Next;
        /*
        if (neighbour->Next != nullptr) {
            neighbour->Next->Previous = neighbour;
        }
         */
    }
    n->NumOfKeys = 0;
    deleteEntryForDeleteOperatoin(n->Parent, n_index);
}

//okstree
void DAstatic::deleteEntryForDeleteOperatoin(NodeForStree * n, int indexInNode) {
    int neighbour_index;
    NodeForStree * neighbour;
    n = removeEntryFromNode(n, indexInNode);
    n->NumOfKeys--;
    if (n == Root) {
        adjustRoot();
        return;
    }
    int nNumKeys = n->NumOfKeys;
    if (nNumKeys >= Min) {
        NodeForStree * tmp = n;
        while (tmp != Root) {
            for (int i = 0; i < Capacity; i++) {
                if (tmp->Parent->Pointers[i] == tmp) {
                    tmp->Parent->Keys[i] --;
                    tmp = tmp->Parent;
                    break;
                }
            }
        }
        return;
    }
    neighbour_index = getLeftNeighborIndex(n);
    if (neighbour_index == -1) {
        neighbour = n->Parent->Pointers[1];
    } else {
        neighbour = n->Parent->Pointers[neighbour_index];
    }
    if (neighbour->NumOfKeys + nNumKeys <= Capacity) {
        coalesceNodesForDeleteOperation(n, neighbour, neighbour_index);
        return;
    } else {
        redistributeNodesForDeleteOperation(n, neighbour, neighbour_index);
        return;
    }
}

void DAstatic::redistributeNodesForDeleteOperation(NodeForStree * n, NodeForStree * neighbour, int neighbour_index) {
    int i, leftNumKeys = 0, rightNumKeys = 0, leftIndex, leftSumKeys = 0, rightSumKeys = 0;
    if (neighbour_index != -1) {
        leftIndex = neighbour_index;
        int indexToGoRight = Min;
        leftNumKeys = Min;
        int temp_keys[2*Capacity];
        NodeForStree * temp_pointers[2*Capacity];
        int tmp_j = 0;
        for (i = indexToGoRight; i < neighbour->NumOfKeys; i++) {
            temp_pointers[tmp_j] = neighbour->Pointers[i];
            temp_keys[tmp_j] = neighbour->Keys[i];
            tmp_j++;
        }
        for (i = 0; i < n->NumOfKeys; i++) {
            temp_keys[tmp_j] = n->Keys[i];
            temp_pointers[tmp_j] = n->Pointers[i];
            tmp_j++;
        }
        rightNumKeys = tmp_j;
        for (i = 0; i < Capacity; i++) {
            if (i == tmp_j) {
                break;
            }
            n->Pointers[i] = temp_pointers[i];
            n->Keys[i] = temp_keys[i];
            if (!n->IsLeaf) {
                if (n->Pointers[i] != nullptr) {
                    NodeForStree * s = n->Pointers[i];
                    s->Parent = n;
                }
            }
        }
        neighbour->NumOfKeys = leftNumKeys;
        n->NumOfKeys = rightNumKeys;
        leftSumKeys = NodeSumKeys(neighbour);
        rightSumKeys = NodeSumKeys(n);
    } else {
        leftIndex = 0;
        int j = 0;
        for (i = neighbour->NumOfKeys - 1; i >= 0; i--) {
            j++;
            if (j == Min) {
                break;
            }
        }
        int indexToGoLeft = i - 1;
        rightNumKeys = Min;
        int tmp_j = n->NumOfKeys;
        for (i = 0; i <= indexToGoLeft; i++) {
            n->Pointers[tmp_j] = neighbour->Pointers[i];
            n->Keys[tmp_j] = neighbour->Keys[i];
            if (!n->IsLeaf) {
                if (n->Pointers[tmp_j] != nullptr) {
                    NodeForStree * s = n->Pointers[tmp_j];
                    s->Parent = n;
                }
            }
            tmp_j++;
        }
        leftNumKeys = tmp_j;
        int k = indexToGoLeft + 1;
        for (i = 0; i < Min; i++) {
            neighbour->Keys[i] = neighbour->Keys[k];
            neighbour->Pointers[i] = neighbour->Pointers[k];
            k++;
        }
        neighbour->NumOfKeys = rightNumKeys;
        n->NumOfKeys = leftNumKeys;
        leftSumKeys = NodeSumKeys(n);
        rightSumKeys = NodeSumKeys(neighbour);
    }
    NodeForStree * c = n->Parent;
    c->Keys[leftIndex] = leftSumKeys;
    c->Keys[leftIndex+1] = rightSumKeys;
    c = n->Parent;
    NodeForStree * parent = c->Parent;
    while (parent != nullptr) {
        for (i = 0; i < parent->NumOfKeys; i++) {
            if (parent->Pointers[i] == c) {
                parent->Keys[i] -= 1;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}


//=============================================swap=========================================================

void DAstatic::Swap(int start1, int end1, int start2, int end2) {

    int numLeaf1, numLeaf2, startIndexLeaf1, startIndexLeaf2, endIndexLeaf1, endIndexLeaf2;
    NodeForStree ** inter1 = nullptr;
    NodeForStree ** inter2 = nullptr;
    numLeaf1 = RangePosInLeafPointer(start1, end1, &inter1, &startIndexLeaf1, &endIndexLeaf1);
    numLeaf2 = RangePosInLeafPointer(start2, end2, &inter2, &startIndexLeaf2, &endIndexLeaf2);

    swapLeaf(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
}

int DAstatic::RangePosInLeafPointer(int start, int end, NodeForStree *** inter, int * startIndexLeaf, int * endIndexLeaf){
    int numLeaf = 0;
    int numLeafMost = (end-start+1)/Min + 2;
    *inter = new NodeForStree* [numLeafMost];
    NodeForStree * leaf = findLeaf(start, startIndexLeaf);
    *((*inter)) = leaf;
    int length = end - start + 1;
    if (length <= leaf->NumOfKeys - *startIndexLeaf) {
        *endIndexLeaf = *startIndexLeaf + length - 1;
        return 1;
    }
    length -= leaf->NumOfKeys - *startIndexLeaf;
    int ansIdx = 1;
    NodeForStree * cur = leaf->Next;
    while (true) {
        *((*inter) + ansIdx) = cur;
        if (length <= cur->NumOfKeys) {
            *endIndexLeaf = length - 1;
            return ansIdx+1;
        } else {
            length -= cur->NumOfKeys;
            ansIdx++;
            cur = cur->Next;
        }
    }
    return  0;
}


void DAstatic::deleteEntryForCheckMinArray(NodeForStree * n, int indexInNode) {
    int min_keys, neighbour_index, capacity;
    NodeForStree * neighbour;
    n = removeEntryFromNode(n, indexInNode);
    n->NumOfKeys--;
    if (n == Root) {
        adjustRoot();
        return;
    }
    min_keys = Min;
    int nNumKeys = n->NumOfKeys;
    if (nNumKeys >= min_keys) {
        return;
    }

    neighbour_index = getLeftNeighborIndex(n);

    if (neighbour_index == -1) {
        neighbour = n->Parent->Pointers[1];
    } else {
        neighbour = n->Parent->Pointers[neighbour_index];
    }

    capacity = Capacity;
    if (neighbour->NumOfKeys+nNumKeys <= capacity) {
        coalesceNodesForCheckMinArray(n, neighbour, neighbour_index);
        return;
    } else {
        redistributeNodesForCheckMinArray(n, neighbour, neighbour_index);
        return;
    }
}

void DAstatic::redistributeNodesForCheckMinArray(NodeForStree * n, NodeForStree * neighbour, int neighbour_index) {
    int i, leftNumKeys = 0, rightNumKeys = 0, leftIndex, leftSumKeys = 0, rightSumKeys = 0;

    int min_keys = Min;
    if (neighbour_index != -1) { // neighbor (more keys) is on the left
        leftIndex = neighbour_index;
        for (i = 0; i < min_keys; i++) {
            leftSumKeys += neighbour->Keys[i];
        }
        int indexToGoRight = min_keys;
        leftNumKeys = min_keys;

        int * temp_keys = new int[Capacity*2];
        NodeForStree ** temp_pointers = new NodeForStree*[Capacity*2];
        int tmp_j = 0;
        for (i = indexToGoRight; i < neighbour->NumOfKeys; i++ ){
            temp_pointers[tmp_j] = neighbour->Pointers[i];
            temp_keys[tmp_j] = neighbour->Keys[i];
            rightSumKeys += neighbour->Keys[i];
            tmp_j++;
        }
        rightSumKeys += NodeSumKeys(n);
        for (i = 0; i < n->NumOfKeys; i++) {
            temp_keys[tmp_j] = n->Keys[i];
            temp_pointers[tmp_j] = n->Pointers[i];
            tmp_j++;
        }
        rightNumKeys = tmp_j;
        for (i = 0; i < Capacity; i++) {
            if (i == tmp_j) {
                break;
            }
            n->Pointers[i] = temp_pointers[i];
            n->Keys[i] = temp_keys[i];
            if (!n->IsLeaf) {
                if (n->Pointers[i] != nullptr) {
                    NodeForStree * s = n->Pointers[i];
                    s->Parent = n;
                }
            }
        }
        neighbour->NumOfKeys = leftNumKeys;
        n->NumOfKeys = rightNumKeys;
    } else { // neighbor is on the right (more keys) (neighbor_index == -1)
        leftIndex = 0;
        int j = 0;
        for (i = neighbour->NumOfKeys - 1; i >= 0; i--) {
            rightSumKeys += neighbour->Keys[i];
            j++;
            if (j == min_keys) {
                break;
            }
        }
        int indexToGoLeft = i - 1;
        rightNumKeys = min_keys;

        int tmp_j = n->NumOfKeys;
        leftSumKeys = NodeSumKeys(n);
        for (i = 0; i <= indexToGoLeft; i++) {
            n->Pointers[tmp_j] = neighbour->Pointers[i];
            n->Keys[tmp_j] = neighbour->Keys[i];
            leftSumKeys += n->Keys[tmp_j];
            if (!n->IsLeaf) {
                if (n->Pointers[tmp_j] != nullptr) {
                    NodeForStree * s = n->Pointers[tmp_j];
                    s->Parent = n;
                }
            }
            tmp_j++;
        }
        leftNumKeys = tmp_j;
        int k = indexToGoLeft + 1;
        for (i = 0; i < min_keys; i++) {
            neighbour->Keys[i] = neighbour->Keys[k];
            neighbour->Pointers[i] = neighbour->Pointers[k];
            k++;
        }
        neighbour->NumOfKeys = rightNumKeys;
        n->NumOfKeys = leftNumKeys;
    }
    NodeForStree * c = n->Parent;
    c->Keys[leftIndex] = leftSumKeys;
    c->Keys[leftIndex+1] = rightSumKeys;
    return;
}

void DAstatic::coalesceNodesForCheckMinArray(NodeForStree * n, NodeForStree * neighbour, int neighbour_index) {
    int i, j, neighbour_insertion_index ;
    NodeForStree *  tmp ;
    int n_index = neighbour_index + 1;
    //set neighbor to be the node on the left
    if (neighbour_index == -1) {
        tmp = n;
        n = neighbour;
        neighbour = tmp;
        n_index = 1;
        neighbour_index = 0;
    }
    neighbour_insertion_index = neighbour->NumOfKeys;
    i = neighbour_insertion_index;
    int newParentKey = NodeSumKeys(neighbour) + NodeSumKeys(n);

    for (j = 0; j < n->NumOfKeys; j++) {
        neighbour->Keys[i] = n->Keys[j];
        neighbour->Pointers[i] = n->Pointers[j];
        if (!neighbour->IsLeaf) {
            tmp = neighbour->Pointers[i];
            if (tmp != nullptr) {
                tmp->Parent = neighbour;
            }
        }
        i += 1;
    }
    neighbour->NumOfKeys = neighbour->NumOfKeys + n->NumOfKeys;
    neighbour->Parent->Keys[neighbour_index] = newParentKey;
    if (neighbour->IsLeaf){
        neighbour->Next = n->Next;
        /*
        if (neighbour->Next != nullptr) {
            NodeForStree * tmp = neighbour->Next;
            tmp->Previous = neighbour;
        }
         */
    }
    n->NumOfKeys = 0;
    deleteEntryForCheckMinArray(n->Parent, n_index);
}

int DAstatic::getIndexInParent(NodeForStree * n) const {
    NodeForStree * parent = n->Parent;
    for (int i = 0; i < Capacity; i++) {
        if (parent->Pointers[i] == n) {
            return i;
        }
    }
    return -1;
}


//--------------------------------------------------------- ***
void DAstatic::swapLeaf(NodeForStree ** inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
                        int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    if (inter1[numLeaf1-1] == inter2[0]) { //head = tail
        if (numLeaf1 == 1 && numLeaf2 == 1) {
            swapHeadTail11(inter1[0], startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        } else if (numLeaf1 == 1 && numLeaf2 == 2) {
            swapHeadTail12(inter1[0], inter2[1], startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        } else if (numLeaf1 == 2 && numLeaf2 == 1) {
            swapHeadTail21(inter1[0], inter1[1], startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        } else if (numLeaf1 == 2 && numLeaf2 == 2) {
            swapHeadTail22(inter1[0], inter1[1], inter2[1], startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        } else if (numLeaf1 == 1) {
            swapHeadTailLen1Is1(inter2, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        } else if (numLeaf2 == 1) {
            swapHeadTailLen2Is1(inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        } else if (numLeaf1 == 2) {
            swapHeadTailLen1Is2(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        } else if (numLeaf2 == 2) {
            swapHeadTailLen2Is2(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
            return;
        }
        SwapForTailHead(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
        return;
    }

    if (inter1[numLeaf1-1]->Next == inter2[0]) {
        SwapRangesAdjacent(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
        return;
    }

    if (numLeaf2 == 1 && numLeaf1 == 1) {
        SwapWhenBothLensAre1(inter1[0], startIndexLeaf1, endIndexLeaf1, inter2[0], startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf1 == 1 && numLeaf2 == 2) {
        SwapWhenLensAre1And2(inter1[0], startIndexLeaf1, endIndexLeaf1, inter2[0], startIndexLeaf2, inter2[1], endIndexLeaf2);
        return;
    } else if (numLeaf1 == 2 && numLeaf2 == 1) {
        SwapWhenLensAre1And2(inter2[0], startIndexLeaf2, endIndexLeaf2, inter1[0], startIndexLeaf1, inter1[1], endIndexLeaf1);
        return;
    } else if (numLeaf1 == 1) {
        SwapWhenLenIs1(inter1[0], inter2, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf2 == 1) {
        SwapWhenLenIs1(inter2[0], inter1, numLeaf1, startIndexLeaf2, endIndexLeaf2, startIndexLeaf1, endIndexLeaf1);
        return;
    } else if (numLeaf2 == 2 && numLeaf1 == 2) {
        SwapWhenBothLensAre2(inter1[0], inter1[1], startIndexLeaf1, endIndexLeaf1, inter2[0], inter2[1], startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf1 == 2) {
        SwapWhenLeftLenIs2(inter1[0], inter1[1], startIndexLeaf1, endIndexLeaf1, inter2, numLeaf2, startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf2 == 2) {
        SwapWhenLeftLenIs2(inter2[0], inter2[1], startIndexLeaf2, endIndexLeaf2, inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1);
        return;
    }
    NodeForStree **node1 = inter1;
    NodeForStree **node2 = inter2;
    //TimeVar time1 = timeNow();
    swapWholeLeafAndGoUpwards(node1, node2, numLeaf1, numLeaf2);
    //TimeVar time2 = timeNow();
    //printf("time to swapWholeLeafAndGoUpwards = %d\n", duration(time2-time1));

    //move pointers in starting leaf
    NodeForStree * lastl1 = node1[numLeaf1-1];
    NodeForStree * lastl2 = node2[numLeaf2-1];
    if (startIndexLeaf1 > 0 && startIndexLeaf2 > 0) {
        swapStartPointer(node1[0], startIndexLeaf1-1, node2[0], startIndexLeaf2-1);
    } else if (startIndexLeaf1 > 0 && startIndexLeaf2 == 0) {
        movePointerBefore(node1[0], 0, startIndexLeaf1 - 1, node2[0], 0);
    } else if (startIndexLeaf1 == 0 && startIndexLeaf2 > 0) {
        movePointerBefore(node2[0], 0, startIndexLeaf2 - 1, node1[0], 0);
    }

    //move pointers in end leaf
    if (endIndexLeaf1 < lastl1->NumOfKeys-1 && endIndexLeaf2 < lastl2->NumOfKeys-1) {
        swapEndPointer(lastl1, endIndexLeaf1+1, lastl1->NumOfKeys-1, lastl2,
                       endIndexLeaf2+1, lastl2->NumOfKeys-1);
        return; // okokok
    } else if (endIndexLeaf1 == lastl1->NumOfKeys-1 && endIndexLeaf2 < lastl2->NumOfKeys-1) {
        movePointerAfter(lastl2, endIndexLeaf2 + 1, lastl2->NumOfKeys - 1, lastl1, endIndexLeaf1 + 1);
        return;
    } else { //endIndexLeaf1 < lastl1->NumOfKeys-1 && endIndexLeaf2 == lastl2->NumOfKeys-1
        movePointerAfter(lastl1, endIndexLeaf1 + 1, lastl1->NumOfKeys - 1, lastl2, endIndexLeaf2 + 1);
        return;
    }
}


void DAstatic::swapEndPointer(NodeForStree * leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf,
                              NodeForStree * rightLeaf, int indexInRightLeaf , int endIndexInRightLeaf) {
    int len1 = endIndexInLeftLeaf + 1 - indexInLeftLeaf;
    int len2 = endIndexInRightLeaf + 1 - indexInRightLeaf;

    int j = indexInRightLeaf;
    int i = indexInLeftLeaf;

    int exchangeNum = len1;
    if (exchangeNum > len2) {
        exchangeNum = len2;
    }
    for (int k = 0; k < exchangeNum; k++) {
        int tmpKey = leftLeaf->Keys[i];
        leftLeaf->Keys[i] = rightLeaf->Keys[j];
        rightLeaf->Keys[j] = tmpKey;
        j++;
        i++;
    }

    if (exchangeNum < len1) {
        // move pointers in leftLeaf index starting from i ending xx to rightLeaf index starting from j
        movePointerAfter(leftLeaf, i, endIndexInLeftLeaf, rightLeaf, j);
    } else if (exchangeNum < len2) {
        movePointerAfter(rightLeaf, j, endIndexInRightLeaf, leftLeaf, i);
    } else {
        AdjustAncestorKeysAccordingToCurNode(leftLeaf);
        AdjustAncestorKeysAccordingToCurNode(rightLeaf);
    }
}

//call this function after deleting a range of leaves
void DAstatic::CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForStree * preLeaf, NodeForStree ** leafToDelete, int numLeafToDelete) {
    NodeForStree * leftLeafParent = leafToDelete[0]->Parent;
    NodeForStree * rightLeafParent = leafToDelete[numLeafToDelete-1]->Parent;
    if (leftLeafParent->NumOfKeys != 0) {
        RedistributeMergeRecomputeKeysInAncestorsLeftPart(leftLeafParent);
    } else {
        if (preLeaf != nullptr) {
            leftLeafParent = preLeaf->Parent;
            RedistributeMergeRecomputeKeysInAncestorsLeftPart(leftLeafParent);
        }
    }

    rightLeafParent = leafToDelete[numLeafToDelete-1]->Parent;
    if (rightLeafParent->NumOfKeys != 0) {
        if (rightLeafParent != leftLeafParent) {
            RedistributeMergeRecomputeKeysInAncestorsLeftPart(rightLeafParent);
        }
    } else {
        if (leafToDelete[numLeafToDelete-1]->Next != nullptr) {
            rightLeafParent = leafToDelete[numLeafToDelete-1]->Next->Parent;
            RedistributeMergeRecomputeKeysInAncestorsLeftPart(rightLeafParent);
        }
    }
}


void DAstatic::swapWholeLeafAndGoUpwards(NodeForStree **wholeLeaf1, NodeForStree **wholeLeaf2, int w1, int w2) {
    int remain, exchangeNum;
    if (w1 == w2) {
        exchangeNum = w1;
        remain = 0;
    } else if (w1<w2) {
        exchangeNum = w1;
        remain = 1;
    } else {
        exchangeNum = w2;
        remain = -1 ;// left leaves more than right leaves
    }

    //---------------------------start exchange corresponding leaves------------------------------------------
    NodeForStree *  LeftEdgeLeaf = nullptr, * RightEdgeLeaf = nullptr;
    NodeForStree ** LeftParentArray = new NodeForStree* [exchangeNum];
    NodeForStree ** RightParentArray = new NodeForStree* [exchangeNum];
    int lp = 0, rp = 0;
    NodeForStree * lastLeftGrandParent = nullptr;
    NodeForStree * lastRightGrandParent = nullptr;

    // adjust next, previous pointers
    NodeForStree * LeftStartLeaf = wholeLeaf1[0];
    NodeForStree * RightStartLeaf = wholeLeaf2[0];
    NodeForStree * LeftEndLeaf = wholeLeaf1[exchangeNum-1];
    NodeForStree * RightEndLeaf = wholeLeaf2[exchangeNum-1];
    if (LeftEndLeaf->Next == RightStartLeaf) {
        LeftEndLeaf->Next = RightEndLeaf->Next;
        /*
        if (LeftEndLeaf->Next != nullptr) {
            LeftEndLeaf->Next->Previous = LeftEndLeaf;
        }
        RightStartLeaf->Previous = LeftStartLeaf->Previous;
        */
        NodeForStree * preLeftStar = findPrevious(LeftStartLeaf);
        if (preLeftStar != nullptr) {
            preLeftStar->Next = RightStartLeaf;
        }
        //LeftStartLeaf->Previous = RightEndLeaf;
        RightEndLeaf->Next = LeftStartLeaf;
    } else {
        // need to adjust next and previous pointers of the first two exchanging leaves
        LeftEdgeLeaf = wholeLeaf1[0];
        RightEdgeLeaf = wholeLeaf2[0];
        /*
        if (LeftEdgeLeaf->Next == RightEdgeLeaf) {
            NodeForStree * LeftPre = LeftEdgeLeaf->Previous;
            RightEdgeLeaf->Previous = LeftPre;
            if (LeftPre != nullptr) {
                LeftPre->Next = RightEdgeLeaf;
            }
            RightEdgeLeaf->Next = LeftEdgeLeaf;
            LeftEdgeLeaf->Previous = RightEdgeLeaf;
        } else {
         */
        NodeForStree * LeftPre = findPrevious(LeftEdgeLeaf);
        NodeForStree * RightPre = findPrevious(RightEdgeLeaf);
        //RightEdgeLeaf->Previous = LeftPre;
        if (LeftPre != nullptr) {
            LeftPre->Next = RightEdgeLeaf;
        }
        //LeftEdgeLeaf->Previous = RightPre;
        if (RightPre != nullptr) {
            RightPre->Next = LeftEdgeLeaf;
        }
        //}
        LeftEdgeLeaf = wholeLeaf1[exchangeNum-1];
        RightEdgeLeaf = wholeLeaf2[exchangeNum-1];
        //need to adjust next previous pointers of the last two exchanging leaves
        NodeForStree * LeftNext = LeftEdgeLeaf->Next;
        NodeForStree * RightNext = RightEdgeLeaf->Next;
        RightEdgeLeaf->Next = LeftNext;
        /*
        if (LeftNext != nullptr) {
            LeftNext->Previous = RightEdgeLeaf;
        }
         */
        LeftEdgeLeaf->Next = RightNext;
        /*
        if (RightNext != nullptr) {
            RightNext->Previous = LeftEdgeLeaf;
        }
         */
    }
    bool NeedAdjustAncestor = false;
    for (int i = 0; i < exchangeNum; i++) {
        NodeForStree * LeftLeaf = wholeLeaf1[i];
        NodeForStree * RightLeaf = wholeLeaf2[i];
        NodeForStree * LeftParent = LeftLeaf->Parent;
        NodeForStree * RightParent = RightLeaf->Parent;

        exchangeLeaf(LeftLeaf, LeftParent, RightLeaf, RightParent);
        LeftEdgeLeaf = RightLeaf;
        RightEdgeLeaf = LeftLeaf;
        if (LeftParent != nullptr && LeftParent->Parent != nullptr) {
            NeedAdjustAncestor = true;
            if (LeftParent->Parent != lastLeftGrandParent) {
                LeftParentArray[lp] = LeftParent->Parent;
                lastLeftGrandParent = LeftParent->Parent;
                lp++;
            }
            if (RightParent->Parent != lastRightGrandParent) {
                RightParentArray[rp] = RightParent->Parent;
                lastRightGrandParent = RightParent->Parent;
                rp++;
            }
        }
    }
    if (NeedAdjustAncestor) {
        adjustAncestorKeysAfterExchangingLeaves(LeftParentArray, lp);
        adjustAncestorKeysAfterExchangingLeaves(RightParentArray, rp);
    }
    //---------------------------end exchange corresponding leaves------------------------------------------

    //delete leaves
    if (remain != 0) {
        //delete leaves without merging/redistribution/adjusting ancestor's keys
        NodeForStree **  leafToDelete;
        int numLeafToDelete ;
        NodeForStree * pre = nullptr;
        if (remain == -1) { // left leaves more
            numLeafToDelete = w1 - w2;
            leafToDelete = &wholeLeaf1[exchangeNum];
            pre = wholeLeaf2[exchangeNum-1];
        } else { // remain == 1, right leaves more
            numLeafToDelete = w2 - w1;
            leafToDelete = &wholeLeaf2[exchangeNum];
            pre = wholeLeaf1[exchangeNum-1];
        }
        if (pre != nullptr) {
            pre->Next = leafToDelete[numLeafToDelete-1]->Next;
        }
        /*
        if (leafToDelete[0]->Previous != nullptr) {
            leafToDelete[0]->Previous->Next = leafToDelete[numLeafToDelete-1]->Next;
        }
        if (leafToDelete[numLeafToDelete-1]->Next != nullptr) {
            leafToDelete[numLeafToDelete-1]->Next->Previous = leafToDelete[0]->Previous;
        }
         */
        //only delete entries, no redistribution or merging
        NodeForStree * preLeaf = deleteMulEntry(leafToDelete, numLeafToDelete);
        // adjust keys in ancestors
        CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, leafToDelete, numLeafToDelete);

        // insert leaves
        if (remain == -1) { // left leaves more
            NodeForStree * toParent = RightEdgeLeaf->Parent;
            int startIndexInParent = getIndexInParent(RightEdgeLeaf) + 1;
            insertLeavesIntoLeafParent(toParent, startIndexInParent, leafToDelete, numLeafToDelete);
        } else if (remain == 1) { // right leaves more
            NodeForStree *  toParent = LeftEdgeLeaf->Parent;
            int startIndexInParent = getIndexInParent(LeftEdgeLeaf) + 1;
            insertLeavesIntoLeafParent(toParent, startIndexInParent, leafToDelete, numLeafToDelete);
        }
    }
}


void DAstatic::exchangeLeaf(NodeForStree * LeftLeaf, NodeForStree * LeftParent, NodeForStree * RightLeaf, NodeForStree * RightParent) const{
    int index1 = -1, index2 = -1, key1 = -1, key2 = -1;
    for (int i = 0; i < Capacity; i++) {
        if (LeftParent->Pointers[i] == LeftLeaf) {
            index1 = i;
            key1 = LeftParent->Keys[i];
            break;
        }
    }
    for (int i = 0; i < Capacity; i++) {
        if (RightParent->Pointers[i] == RightLeaf) {
            index2 = i;
            key2 = RightParent->Keys[i];
            break;
        }
    }
    LeftParent->Pointers[index1] = RightLeaf;
    LeftParent->Keys[index1] = key2;
    RightParent->Pointers[index2] = LeftLeaf;
    RightParent->Keys[index2] = key1;
    RightLeaf->Parent = LeftParent;
    LeftLeaf->Parent = RightParent;
}


NodeForStree * DAstatic::movePointerAfter(NodeForStree * fromLeaf, int startIndex, int endIndex, NodeForStree * toLeaf, int toIndex) {
    int * keysToInsert = new int[Capacity];
    for (int i = startIndex; i <= endIndex; i++) {
        keysToInsert[i-startIndex] = fromLeaf->Keys[i];
    }
    int numKeysToInsert = endIndex - startIndex + 1;
    //insert
    insertKeysIntoLeaf(keysToInsert, numKeysToInsert, toLeaf, toIndex);

    //delete
    int q = startIndex;
    int p = endIndex + 1;
    while (p < fromLeaf->NumOfKeys) {
        fromLeaf->Keys[q] = fromLeaf->Keys[p];
        p++;
        q++;
    }
    fromLeaf->NumOfKeys -= numKeysToInsert;
    return adjustAfterDeleteKeysInNode(fromLeaf);
}

void DAstatic::insertLeavesIntoLeafParent(NodeForStree * toParent, int startIndexInParent, NodeForStree ** leafToInsert, int numLeafToInsert){
    if (toParent->NumOfKeys + numLeafToInsert <= Capacity) {
        insertMulLeavesIntoLeafParent(toParent, startIndexInParent, leafToInsert, numLeafToInsert);
    } else {
        insertMulLeavesIntoLeafParentAfterSplitting(toParent, startIndexInParent, leafToInsert, numLeafToInsert);
    }
}


// no need to split
void DAstatic::insertMulLeavesIntoLeafParent(NodeForStree * toParent, int startIndexInParent, NodeForStree ** leafToInsert, int numLeafToInsert){
    NodeForStree * last = leafToInsert[numLeafToInsert-1];
    if (toParent->NumOfKeys == startIndexInParent){
        last->Next = toParent->Pointers[startIndexInParent-1]->Next;
        //leafToInsert[0]->Previous = toParent->Pointers[startIndexInParent-1];
        /*
        if (last->Next != nullptr){
            last->Next->Previous = last;
        }
         */
        NodeForStree * pre = toParent->Pointers[startIndexInParent-1];
        if (pre != nullptr){
            pre->Next = leafToInsert[0];
        }

    } else {
        last->Next = toParent->Pointers[startIndexInParent];
        /*
        leafToInsert[0]->Previous = toParent->Pointers[startIndexInParent]->Previous;
        if (last->Next != nullptr){
            last->Next->Previous = last;
        }
         */
        NodeForStree * pre = findPrevious(toParent->Pointers[startIndexInParent]);
        if (pre != nullptr){
            pre->Next = leafToInsert[0];
        }
    }

    //move existing pointers afterwards
    for (int j = toParent->NumOfKeys - 1; j >= startIndexInParent; j--){
        toParent->Keys[j+numLeafToInsert] = toParent->Keys[j]; //bug !! j+numLeafToInsert exceed range
        toParent->Pointers[j+numLeafToInsert] = toParent->Pointers[j];
    }
    // insert new leaves
    int j = 0;
    int numKeysIncreased = 0;

    int NumOfLeafToInsert = numLeafToInsert;
    for (int i = startIndexInParent; i < Capacity; i++){
        toParent->Keys[i] = NodeSumKeys(leafToInsert[j]);
        numKeysIncreased += toParent->Keys[i];
        toParent->Pointers[i] = leafToInsert[j];
        leafToInsert[j]->Parent = toParent;
        j++;
        if (j == NumOfLeafToInsert){
            break;
        }
    }
    toParent->NumOfKeys += NumOfLeafToInsert;

    //go upwards, modify keys in ancestors
    if (toParent == Root){
        return;
    }
    AdjustAncestorKeyForOneNode(toParent);
}


// adjust keys in ancestors of node c
void DAstatic::AdjustAncestorKeyForOneNode(NodeForStree * c) {
    NodeForStree * parent = c->Parent;
    int delta = -1;
    for (int i = 0; i < parent->NumOfKeys; i++){
        if (parent->Pointers[i] == c){
            int x = NodeSumKeys(c);
            delta = x - parent->Keys[i];
            parent->Keys[i] = x;
            break;
        }
    }
    c = c->Parent;
    parent = c->Parent;
    while (parent != nullptr){
        for (int i = 0; i < parent->NumOfKeys; i++){
            if (parent->Pointers[i] == c){
                parent->Keys[i] += delta;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

//numLeafToInsert < Capacity
//toParent->NumOfKeys+numLeaf > Capacity
//insert into toParent
// need to adjust next, previous pointers before calling this function
void DAstatic::insertMulLeavesIntoLeafParentSplitOnce(NodeForStree * toParent, int startIndexInParent,
                                                      NodeForStree ** leafToInsert, int numLeafToInsert) {
    int allNum = toParent->NumOfKeys + numLeafToInsert;
    int leftNum = allNum / 2;
    int rightNum = allNum - leftNum;
    int leftSum = 0, rightSum = 0;
    NodeForStree ** allLeaf = new NodeForStree*[Capacity + numLeafToInsert];
    int * allKeys = new int[Capacity + numLeafToInsert];
    int j = 0;
    for (int i = 0; i < startIndexInParent; i++) {
        allLeaf[j] = toParent->Pointers[i];
        allKeys[j] = toParent->Keys[i];
        j++;
    }
    for (int i = 0; i < numLeafToInsert; i++) {
        allLeaf[j] = leafToInsert[i];
        allKeys[j] = NodeSumKeys(allLeaf[j]);
        j++;
    }
    for (int i = startIndexInParent; i < toParent->NumOfKeys; i++) {
        allLeaf[j] = toParent->Pointers[i];
        allKeys[j] = toParent->Keys[i];
        j++;
    }
    int s = 0;
    for (int k = 0; k < leftNum; ++k) {
        toParent->Pointers[k] = allLeaf[s];
        toParent->Pointers[k]->Parent = toParent;
        toParent->Keys[k] = allKeys[s];
        leftSum += toParent->Keys[k];
        s++;
    }
    toParent->NumOfKeys = leftNum;
    NodeForStree * newn = new NodeForStree(Capacity);
    for (int l = 0; l < rightNum; ++l) {
        newn->Pointers[l] = allLeaf[s];
        newn->Pointers[l]->Parent = newn;
        newn->Keys[l] = allKeys[s];
        rightSum += newn->Keys[l];
        s++;
    }
    newn->NumOfKeys = rightNum;
    insertIntoParent(toParent, leftSum, newn, rightSum);
}


//may split more than once
void DAstatic::insertMulLeavesIntoLeafParentAfterSplitting(NodeForStree * toParent, int startIndexInParent,
                                                           NodeForStree ** leafToInsert, int numLeafToInsert) {
    //next, previous
    NodeForStree * last = leafToInsert[numLeafToInsert-1];
    if (toParent->NumOfKeys == startIndexInParent) {
        last->Next = toParent->Pointers[startIndexInParent-1]->Next;
        /*
        leafToInsert[0]->Previous = toParent->Pointers[startIndexInParent-1];
        if (last->Next != nullptr) {
            last->Next->Previous = last;
        }
         */
        NodeForStree * pre = toParent->Pointers[startIndexInParent-1];
        if (pre != nullptr) {
            pre->Next = leafToInsert[0];
        }

    } else {
        last->Next = toParent->Pointers[startIndexInParent];
        /*
        leafToInsert[0]->Previous = toParent->Pointers[startIndexInParent]->Previous;
        if (last->Next != nullptr) {
            last->Next->Previous = last;
        }
        */
        NodeForStree * pre = toParent->Pointers[startIndexInParent-1];
        if (pre != nullptr) {
            pre->Next = leafToInsert[0];
        }
    }

    if (numLeafToInsert <= Capacity) { // new leaves can be put into one node
        insertMulLeavesIntoLeafParentSplitOnce(toParent, startIndexInParent, leafToInsert, numLeafToInsert);
        return;
    }
    //new leaves cannot be put into one node
    //numLeafToInsert > Capacity

    //set pointers after startIndexInParent in toParent
    int numToParentAfterInsert = toParent->NumOfKeys - startIndexInParent;
    NodeForStree ** leafInToParentTail = new NodeForStree*[numToParentAfterInsert];
    for (int k = startIndexInParent; k < toParent->NumOfKeys; ++k) {
        leafInToParentTail[k-startIndexInParent] = toParent->Pointers[k];
        toParent->Pointers[k] = leafToInsert[k-startIndexInParent];
        toParent->Keys[k] = NodeSumKeys(toParent->Pointers[k]);
        toParent->Pointers[k]->Parent = toParent;
    }

    //for leaves in leafToInsert after numToParentAfterInsert, and in leafInToParentTail
    // at least two leaves
    int numFullParent = numLeafToInsert / Capacity;
    int remainNumLeaf = numLeafToInsert - numFullParent*Capacity;
    int parentNewSiblingNum = numFullParent + 1;
    int numInLast = remainNumLeaf, numInSecondLast = Capacity;
    if (remainNumLeaf == 0) { // all full nodes
        numInLast = Capacity;
        parentNewSiblingNum--;
    } else if (numInLast < Min) {
        numInSecondLast = (remainNumLeaf + Capacity)/2;
        numInLast = remainNumLeaf + Capacity - numInSecondLast;
    }
    NodeForStree ** parentNewSibling = new NodeForStree*[parentNewSiblingNum];
    int LeafToInsertIdx = numToParentAfterInsert;
    int leafInToParentTailIdx = 0;
    for (int k = 0; k < parentNewSiblingNum; ++k) {
        parentNewSibling[k] = new NodeForStree(Capacity);
        if (k == parentNewSiblingNum - 1) {// the last one
            for (int y = 0; y < numInLast; y++) {
                if (LeafToInsertIdx == numLeafToInsert) {
                    parentNewSibling[k]->Pointers[y] = leafInToParentTail[leafInToParentTailIdx];
                    parentNewSibling[k]->Keys[y] = NodeSumKeys(leafInToParentTail[leafInToParentTailIdx]);
                    leafInToParentTail[leafInToParentTailIdx]->Parent = parentNewSibling[k];
                    leafInToParentTailIdx++;
                } else {
                    parentNewSibling[k]->Pointers[y] = leafToInsert[LeafToInsertIdx];
                    parentNewSibling[k]->Keys[y] = NodeSumKeys(leafToInsert[LeafToInsertIdx]);
                    leafToInsert[LeafToInsertIdx]->Parent = parentNewSibling[k];
                    LeafToInsertIdx++;
                }
            }
            parentNewSibling[k]->NumOfKeys = numInLast;
        } else if (k == parentNewSiblingNum-2) { // the second last one
            for (int y = 0; y < numInSecondLast; y++) {
                if (LeafToInsertIdx == numLeafToInsert) {
                    parentNewSibling[k]->Pointers[y] = leafInToParentTail[leafInToParentTailIdx];
                    parentNewSibling[k]->Keys[y] = NodeSumKeys(leafInToParentTail[leafInToParentTailIdx]);
                    leafInToParentTail[leafInToParentTailIdx]->Parent = parentNewSibling[k];
                    leafInToParentTailIdx++;
                } else {
                    parentNewSibling[k]->Pointers[y] = leafToInsert[LeafToInsertIdx];
                    parentNewSibling[k]->Keys[y] = NodeSumKeys(leafToInsert[LeafToInsertIdx]);
                    leafToInsert[LeafToInsertIdx]->Parent = parentNewSibling[k];
                    LeafToInsertIdx++;
                }
            }
            parentNewSibling[k]->NumOfKeys = numInSecondLast;
        } else {
            for (int y = 0; y < Capacity; y++) {
                parentNewSibling[k]->Pointers[y] = leafToInsert[LeafToInsertIdx];
                parentNewSibling[k]->Keys[y] = NodeSumKeys(leafToInsert[LeafToInsertIdx]);
                leafToInsert[LeafToInsertIdx]->Parent = parentNewSibling[k];
                LeafToInsertIdx++;
            }
            parentNewSibling[k]->NumOfKeys = Capacity;
        }
    }
    insertMulIntoParent(toParent, parentNewSibling, parentNewSiblingNum);
    delete []leafInToParentTail;
}

void DAstatic::insertMulIntoParent(NodeForStree * toParent, NodeForStree ** parentNewSibling, int parentNewSiblingNum) {
    NodeForStree * parent = toParent->Parent;
    if (parent == nullptr) {
        insertMulIntoNewRoot(toParent, parentNewSibling, parentNewSiblingNum);
        return;
    }
    int toParentIndex = getIndexInParent(toParent);
    int newk = NodeSumKeys(toParent);
    int ParentIncrease = newk - parent->Keys[toParentIndex];
    parent->Keys[toParentIndex] = newk;
    if (parent->NumOfKeys+parentNewSiblingNum <= Capacity) {
        if (parentNewSibling[0]->IsLeaf) {
            for (int k = 0; k < parentNewSiblingNum; k++) {
                ParentIncrease += parentNewSibling[k]->NumOfKeys;
            }
        } else {
            for (int k = 0; k < parentNewSiblingNum; k++) {
                ParentIncrease += NodeSumKeys(parentNewSibling[k]);
            }
        }
        insertMulIntoNode(parent, toParentIndex+1, parentNewSibling, parentNewSiblingNum, ParentIncrease);
    } else {
        insertMulIntoNodeAfterSplitting(parent, toParentIndex+1, parentNewSibling, parentNewSiblingNum);
    }
}

void DAstatic::insertMulIntoNodeAfterSplitting(NodeForStree * parent, int toIndex, NodeForStree** pointerToInsert,
                                               int pointerToInsertNum) {
    NodeForStree ** allPointers = new NodeForStree*[Capacity + pointerToInsertNum];
    int j = 0;
    for (int i = 0; i < toIndex; i++) {
        allPointers[j] = parent->Pointers[i];
        j++;
    }
    for (int i = 0; i < pointerToInsertNum; i++) {
        allPointers[j] = pointerToInsert[i];
        j++;
    }
    for (int i = toIndex; i < parent->NumOfKeys; i++) {
        allPointers[j] = parent->Pointers[i];
        j++;
    }

    int totalNumPointer = pointerToInsertNum + parent->NumOfKeys;
    parent->NumOfKeys = 0;
    int numFullParent = totalNumPointer / Capacity;
    int remainNumLeaf = totalNumPointer - numFullParent*Capacity;
    int min_keys = Min;
    if (remainNumLeaf == 0 || remainNumLeaf >= min_keys) { // full nodes + remainnumleaf
        int i = 0;
        for (; i < Capacity; i++) {
            parent->Pointers[i] = allPointers[i];
            parent->Keys[i] = NodeSumKeys(allPointers[i]);
            allPointers[i]->Parent = parent;
        }
        parent->NumOfKeys = Capacity;
        int parentNewSiblingNum = numFullParent - 1;
        if (remainNumLeaf != 0) {
            parentNewSiblingNum++;
        }
        NodeForStree ** parentNewSibling = new NodeForStree*[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = new NodeForStree(Capacity);
            for (int y = 0; y < Capacity; y++) {
                parentNewSibling[k]->Pointers[y] = allPointers[i];
                parentNewSibling[k]->Keys[y] = NodeSumKeys(allPointers[i]); //allPointers[i]->NumOfKeys
                parentNewSibling[k]->NumOfKeys++;
                allPointers[i]->Parent = parentNewSibling[k];
                i++;
                if (i == totalNumPointer) {
                    break;
                }
            }
        }
        if (parent->Parent == nullptr) {
            insertMulIntoNewRoot(parent, parentNewSibling, parentNewSiblingNum);
        } else {
            insertMulIntoParent(parent, parentNewSibling, parentNewSiblingNum); //insert after "parent"
        }
        return;
        delete []parentNewSibling;
    } else {
        int parentNewSiblingNum = numFullParent;
        numFullParent--;
        int numkeysInSecondLastParent = (totalNumPointer - numFullParent*Capacity) / 2;
        int numkeysInLastParent = totalNumPointer - numFullParent*Capacity - numkeysInSecondLastParent;
        int i_allpointer = 0;
        if (numFullParent == 0) { //toParent is the second last one
            for (int i = 0; i < numkeysInSecondLastParent; i++) {
                parent->Pointers[i] = allPointers[i_allpointer];
                parent->Keys[i] = NodeSumKeys(allPointers[i_allpointer]); //allPointers[i_allpointer]->NumOfKeys
                parent->NumOfKeys++;
                if (allPointers[i_allpointer] != nullptr) {
                    allPointers[i_allpointer]->Parent = parent;
                }
                i_allpointer++;
            }
        } else {
            for (int i = 0; i < Capacity; i++) {
                parent->Pointers[i] = allPointers[i_allpointer];
                parent->Keys[i] = NodeSumKeys(allPointers[i_allpointer]); //allPointers[i_allpointer]->NumOfKeys
                parent->NumOfKeys++;
                allPointers[i_allpointer]->Parent = parent;
                i_allpointer++;
            }
        }
        //set parent's new siblings
        NodeForStree **parentNewSibling = new NodeForStree*[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = new NodeForStree(Capacity);
            if (k == parentNewSiblingNum-1) { // the last one
                for (int y = 0; y < numkeysInLastParent; y++) {
                    parentNewSibling[k]->Pointers[y] = allPointers[i_allpointer];
                    parentNewSibling[k]->Keys[y] = NodeSumKeys(allPointers[i_allpointer]); //allPointers[i_allpointer]->NumOfKeys
                    parentNewSibling[k]->NumOfKeys++;
                    if (allPointers[i_allpointer] != nullptr) {
                        allPointers[i_allpointer]->Parent = parentNewSibling[k];
                    }
                    i_allpointer++;
                }
            } else if (k == parentNewSiblingNum-2) { // the second last one
                for (int y = 0; y < numkeysInSecondLastParent; y++) {
                    parentNewSibling[k]->Pointers[y] = allPointers[i_allpointer];
                    parentNewSibling[k]->Keys[y] = NodeSumKeys(allPointers[i_allpointer]); //allPointers[i_allpointer]->NumOfKeys
                    parentNewSibling[k]->NumOfKeys++;
                    if (allPointers[i_allpointer] != nullptr) {
                        allPointers[i_allpointer]->Parent = parentNewSibling[k];
                    }
                    i_allpointer++;
                }
            } else {
                for (int y = 0; y < Capacity; y++) {
                    parentNewSibling[k]->Pointers[y] = allPointers[i_allpointer];
                    parentNewSibling[k]->Keys[y] = NodeSumKeys(allPointers[i_allpointer]); //allPointers[i_allpointer]->NumOfKeys
                    parentNewSibling[k]->NumOfKeys++;
                    if (allPointers[i_allpointer] != nullptr) {
                        allPointers[i_allpointer]->Parent = parentNewSibling[k];
                    }
                    i_allpointer++;
                }
            }
        }
        if (parent->Parent == nullptr) {
            insertMulIntoNewRoot(parent, parentNewSibling, parentNewSiblingNum);
        } else {
            insertMulIntoParent(parent, parentNewSibling, parentNewSiblingNum);
        }
        delete []parentNewSibling;
    }
    delete []allPointers;
}

void DAstatic::insertMulIntoNode(NodeForStree * parent, int toIndex, NodeForStree** parentNewSibling,
                                 int parentNewSiblingNum, int ParentIncrease) const {
    NodeForStree ** tmpNodes = new NodeForStree*[Capacity];
    int i = toIndex;
    int i_tmpNodes = 0;
    for (int j = 0; j < parentNewSiblingNum; j++) {
        tmpNodes[i_tmpNodes] = parentNewSibling[j];
        i_tmpNodes++;
    }
    for (; i < parent->NumOfKeys; i++) {
        tmpNodes[i_tmpNodes] = parent->Pointers[i];
        parent->Pointers[i] = nullptr;
        i_tmpNodes++;
    }
    parent->NumOfKeys += parentNewSiblingNum;

    int r = 0;
    for (int k = toIndex; k < Capacity; k++) {
        parent->Pointers[k] = tmpNodes[r];
        parent->Keys[k] = NodeSumKeys(tmpNodes[r]);
        tmpNodes[r]->Parent = parent;
        r++;
        if (r == i_tmpNodes) {
            break;
        }
    }
    NodeForStree * c = parent;
    parent = c->Parent;
    while (parent != nullptr ){
        for (i = 0; i < parent->NumOfKeys; i++) {
            if (parent->Pointers[i] == c) {
                parent->Keys[i] += ParentIncrease;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}


void DAstatic::insertMulIntoNewRoot(NodeForStree * toParent, NodeForStree ** parentNewSibling, int parentNewSiblingNum) {
    int totalNum = parentNewSiblingNum + 1;
    int numFullNodes = totalNum / Capacity;
    if ((numFullNodes == 0) || (totalNum == Capacity)) { // new root!
        Root = new NodeForStree(Capacity);
        Root->NumOfKeys = parentNewSiblingNum + 1;
        Root->Keys[0] = NodeSumKeys(toParent);
        Root->Pointers[0] = toParent;
        toParent->Parent = Root;
        for (int i= 0; i < parentNewSiblingNum; i++) {
            Root->Pointers[i+1] = parentNewSibling[i];
            Root->Keys[i+1] = NodeSumKeys(parentNewSibling[i]);
            parentNewSibling[i]->Parent = Root;
        }
        return;
    }
    if (totalNum%Capacity == 0) { //all full nodes
        NodeForStree * leftNode = new NodeForStree(Capacity);
        int rightSiblingNum = numFullNodes - 1;
        NodeForStree ** rightSibling = new NodeForStree*[rightSiblingNum];
        int i_rightchild = 0;
        // set keys/pointers in leftNode
        leftNode->Pointers[0] = toParent;
        leftNode->Keys[0] = NodeSumKeys(toParent);
        leftNode->NumOfKeys = 1;
        toParent->Parent = leftNode;
        for (int i = 1; i < Capacity; i++) {
            leftNode->Pointers[i] = parentNewSibling[i_rightchild];
            leftNode->Keys[i] = NodeSumKeys(parentNewSibling[i_rightchild]);
            leftNode->NumOfKeys++;
            parentNewSibling[i_rightchild]->Parent = leftNode;
            i_rightchild++;
        }
        for (int i = 0; i < rightSiblingNum; i++) {
            rightSibling[i] = new NodeForStree(Capacity);
            for (int j = 0; j < Capacity; j++) {
                rightSibling[i]->Pointers[j] = parentNewSibling[i_rightchild];
                rightSibling[i]->Keys[j] = NodeSumKeys(parentNewSibling[i_rightchild]);
                rightSibling[i]->NumOfKeys++;
                parentNewSibling[i_rightchild]->Parent = rightSibling[i];
                i_rightchild++;
            }
        }
        insertMulIntoNewRoot(leftNode, rightSibling, rightSiblingNum);
        delete []rightSibling;
    } else { // not all full nodes
        NodeForStree * leftNode = new NodeForStree(Capacity);
        leftNode->Pointers[0] = toParent;
        leftNode->Keys[0] = NodeSumKeys(toParent);
        leftNode->NumOfKeys = 1;
        toParent->Parent = leftNode;
        int rightSiblingNum = numFullNodes;
        numFullNodes--;
        NodeForStree ** rightSibling = new NodeForStree*[rightSiblingNum];
        int i_rightchild = 0;
        int remainer = totalNum % Capacity;
        int min_keys = Min;
        int  numkeysInSecondLast, numkeysInLast;
        if (remainer >= min_keys) {
            numkeysInSecondLast = Capacity;
            numkeysInLast = remainer;
        } else {
            numkeysInSecondLast = (totalNum - numFullNodes*Capacity)/2;
            numkeysInLast = totalNum - numFullNodes*Capacity - numkeysInSecondLast;
        }
        if (numFullNodes == 0) { //leftNode is the second last one
            for (int i = 1; i < numkeysInSecondLast; i++) { // leftNode
                leftNode->Pointers[i] = parentNewSibling[i_rightchild];
                leftNode->Keys[i] = NodeSumKeys(parentNewSibling[i_rightchild]);
                parentNewSibling[i_rightchild]->Parent = leftNode;
                i_rightchild++;
            }
            leftNode->NumOfKeys = numkeysInSecondLast;
            rightSibling[0] = new NodeForStree(Capacity);
            for (int i = 0; i < numkeysInLast; i++) {
                rightSibling[0]->Pointers[i] = parentNewSibling[i_rightchild];
                rightSibling[0]->Keys[i] = NodeSumKeys(parentNewSibling[i_rightchild]);
                parentNewSibling[i_rightchild]->Parent = rightSibling[0];
                i_rightchild++;
            }
            rightSibling[0]->NumOfKeys = numkeysInLast;
        } else { // leftnode is not the second last one
            for (int i = 1; i < Capacity; i++) { // leftNode
                leftNode->Pointers[i] = parentNewSibling[i_rightchild];
                leftNode->Keys[i] = parentNewSibling[i_rightchild]->NumOfKeys;
                parentNewSibling[i_rightchild]->Parent = leftNode;
                i_rightchild++;
            }
            leftNode->NumOfKeys = Capacity;
            for (int k = 0; k < rightSiblingNum; k++) {
                rightSibling[k] = new NodeForStree(Capacity);
                if (k == rightSiblingNum-1) { // the last one
                    for (int y = 0; y < numkeysInLast; y++) {
                        rightSibling[k]->Pointers[y] = parentNewSibling[i_rightchild];
                        rightSibling[k]->Keys[y] = NodeSumKeys(parentNewSibling[i_rightchild]);
                        parentNewSibling[i_rightchild]->Parent = rightSibling[k];
                        i_rightchild++;
                    }
                    rightSibling[k]->NumOfKeys = numkeysInLast;
                } else if (k == rightSiblingNum-2) { // the second last one
                    for (int y = 0; y < numkeysInSecondLast; y++) {
                        rightSibling[k]->Pointers[y] = parentNewSibling[i_rightchild];
                        rightSibling[k]->Keys[y] = NodeSumKeys(parentNewSibling[i_rightchild]);
                        parentNewSibling[i_rightchild]->Parent = rightSibling[k];
                        i_rightchild++;
                    }
                    rightSibling[k]->NumOfKeys = numkeysInSecondLast;
                } else {
                    for (int y = 0; y < Capacity; y++) {
                        rightSibling[k]->Pointers[y] = parentNewSibling[i_rightchild];
                        rightSibling[k]->Keys[y] = NodeSumKeys(parentNewSibling[i_rightchild]);
                        parentNewSibling[i_rightchild]->Parent = rightSibling[k];
                        i_rightchild++;
                    }
                    rightSibling[k]->NumOfKeys = Capacity;
                }
            }
        }
        insertMulIntoNewRoot(leftNode, rightSibling, rightSiblingNum);
        delete []rightSibling;
    }
}


NodeForStree * DAstatic::movePointerBefore(NodeForStree * fromLeaf, int startIndex, int endIndex, NodeForStree * toLeaf, int toIndex) {
    int * keysToInsert = new int[Capacity];
    for (int i= startIndex; i <= endIndex; i++) {
        keysToInsert[i-startIndex] = fromLeaf->Keys[i];
    }
    int numKeysToInsert = endIndex - startIndex + 1;
    insertKeysIntoLeaf(keysToInsert, numKeysToInsert, toLeaf, toIndex);

    //delete
    for (int i = endIndex + 1; i < fromLeaf->NumOfKeys; i++) {
        fromLeaf->Keys[i-endIndex-1] = fromLeaf->Keys[i];
        fromLeaf->Pointers[i-endIndex-1] = fromLeaf->Pointers[i];
    }
    fromLeaf->NumOfKeys -= numKeysToInsert;
    delete []keysToInsert;
    return adjustAfterDeleteKeysInNode(fromLeaf);
}

void DAstatic::AdjustAncestorKeysAccordingToCurNode(NodeForStree * n) const {
    if (n->Parent == nullptr || n == Root) {
        return;
    }
    int delta = 0;
    NodeForStree * c = n;
    NodeForStree * parent = c->Parent;
    for (int i = 0; i < parent->NumOfKeys; i++) {
        if (parent->Pointers[i] == c) {
            int x = NodeSumKeys(c);
            delta = x-parent->Keys[i];
            parent->Keys[i] =x;
            break;
        }
    }
    if (delta != 0) {
        c = n->Parent;
        parent = c->Parent;
        while (parent != nullptr) {
            for (int i = 0; i < parent->NumOfKeys; i++) {
                if (parent->Pointers[i] == c) {
                    parent->Keys[i] += delta;
                    c = parent;
                    parent = c->Parent;
                    break;
                }
            }
        }
    }
}


void DAstatic::SimplyAdjustAncestorKeysBy(NodeForStree * node, int increase) const {
    if (node == Root) {
        return;
    }
    NodeForStree * c = node;
    NodeForStree * parent = c->Parent;
    while (parent != nullptr) {
        for (int i = 0; i < parent->NumOfKeys; ++i) {
            if (parent->Pointers[i] == c) {
                parent->Keys[i] += increase;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

NodeForStree * DAstatic::CompleteAdjustAncestorNodesAfterDelete(NodeForStree * n){
    if (n->NumOfKeys < Min) {
        NodeForStree * re = nullptr;
        NodeForStree * neighbour ;
        int neighbour_index = getLeftNeighborIndex(n);
        if (neighbour_index == -1) {
            neighbour = n->Parent->Pointers[1];
        } else {
            neighbour = n->Parent->Pointers[neighbour_index];
        }
        if (neighbour->NumOfKeys + n->NumOfKeys <= Capacity) {
            re = coalesceNodes(n, neighbour, neighbour_index);
        } else {
            redistributeNodes(n, neighbour, neighbour_index);
        }
        return re;
    } else {
        AdjustAncestorKeysAccordingToCurNode(n); // useless reduce
        return nullptr;
    }
}

void DAstatic::replaceKeysInLeaf(NodeForStree * leaf, int startIdx, int endIdx, int * keysToInsert, int numToInsert) {
    int numInLeaf = endIdx-startIdx+1;
    int exchange = numInLeaf;
    if (exchange > numToInsert) {
        exchange = numToInsert;
    }
    for (int i = 0; i < exchange; ++i) {
        leaf->Keys[i+startIdx] = keysToInsert[i];
    }
    if (numInLeaf == numToInsert) {
        AdjustAncestorKeysAccordingToCurNode(leaf);
        return;
    } else if (numInLeaf < numToInsert) {
        insertKeysIntoLeaf(&keysToInsert[exchange], numToInsert-exchange, leaf, endIdx+1);
        return;
    } else { // numInLeaf > numToInsert
        movePointerInNodeForward(leaf, endIdx+1, leaf->NumOfKeys-1, startIdx+exchange);
        leaf->NumOfKeys -= numInLeaf - numToInsert;
        CompleteAdjustAncestorNodesAfterDelete(leaf);
        return;
    }
}


//call this function when
//n->NumOfKeys = startIndex ----1
//insertKeysIntoLeaf() ----2
//maybe after inserting, n->NumOfKeys < Min
void DAstatic::CheckAndMergeRedistributeNode(NodeForStree *n) {
    if (n->NumOfKeys < Min) {
        NodeForStree * neighbour ;
        int neighbour_index = getLeftNeighborIndex(n);
        if (neighbour_index == -1) {
            neighbour = n->Parent->Pointers[1];
        } else {
            neighbour = n->Parent->Pointers[neighbour_index];
        }
        if (neighbour->NumOfKeys + n->NumOfKeys <= Capacity) {
            coalesceNodes(n, neighbour, neighbour_index);
        } else {
            redistributeNodes(n, neighbour, neighbour_index);
        }
    }
}


//adjust from this node to root, and then redistribute/merge
//numKeysDeleted = -1 if omitted
NodeForStree * DAstatic::adjustAfterDeleteKeysInNode(NodeForStree * fromLeaf, int numKeysDeleted) {
    if (fromLeaf == Root) {
        adjustRoot();
        return nullptr;
    }
    if (fromLeaf->NumOfKeys >= Min) {
        if (numKeysDeleted == -1) {
            AdjustAncestorKeysAccordingToCurNode(fromLeaf);
        } else {
            SimplyAdjustAncestorKeysBy(fromLeaf, numKeysDeleted);
        }
        return nullptr;
    } else {
        int neighbour_index = getLeftNeighborIndex(fromLeaf);
        NodeForStree * neighbour;
        if (neighbour_index == -1) {
            neighbour = fromLeaf->Parent->Pointers[1];
        } else {
            neighbour = fromLeaf->Parent->Pointers[neighbour_index];
        }
        if (neighbour->NumOfKeys+fromLeaf->NumOfKeys <= Capacity) {
            return coalesceNodes(fromLeaf, neighbour, neighbour_index);
        } else {
            redistributeNodes(fromLeaf, neighbour, neighbour_index);
            return nullptr;
        }
    }
}


NodeForStree* DAstatic::findPrevious(NodeForStree * node) {
    if (node == Root || node->Parent == nullptr) {
        return nullptr;
    }
    int idxInParent = getIndexInParent(node);
    if (idxInParent > 0) {
        NodeForStree * cur = node->Parent->Pointers[idxInParent-1];
        while (!cur->IsLeaf) {
            cur = cur->Pointers[cur->NumOfKeys-1];
        }
        return cur;
    } else {
        return findPrevious(node->Parent);
    }
}

//only delete entries, no redistribution or merging
NodeForStree * DAstatic::deleteMulEntry(NodeForStree ** entryToDelete, int numEntryToDelete){
    NodeForStree * pre = findPrevious(entryToDelete[0]);
    NodeForStree ** newEntryToDelete = new NodeForStree*[numEntryToDelete/Min+2];
    int i_newEntryToDelete = 0;
    NodeForStree * lastParent = entryToDelete[0]->Parent;
    NodeForStree * FirstParent = lastParent;
    int StartIndexFirstParent = getIndexInParent(entryToDelete[0]);
    NodeForStree * curParent = nullptr;
    int OldNumKeysInCurParent = FirstParent->NumOfKeys;

    for (int i = 0; i < numEntryToDelete; i++) {
        NodeForStree * curEntry = entryToDelete[i];
        curParent = curEntry->Parent;
        curParent->NumOfKeys--;
        if (curParent != lastParent) {
            OldNumKeysInCurParent = curParent->NumOfKeys + 1;
            if (lastParent->NumOfKeys == 0) {
                newEntryToDelete[i_newEntryToDelete] = lastParent;
                i_newEntryToDelete++;
            }
            lastParent = curParent;
        }
    }

    int EndIndex = getIndexInParent(entryToDelete[numEntryToDelete-1]);
    if (FirstParent != curParent) {
        if (curParent->NumOfKeys == 0) {
            newEntryToDelete[i_newEntryToDelete] = lastParent;
            i_newEntryToDelete++;
        } else {
            movePointerInNodeForward(curParent, EndIndex+1, OldNumKeysInCurParent-1, 0);
        }
    } else {
        if (curParent->NumOfKeys == 0) {
            newEntryToDelete[i_newEntryToDelete] = lastParent;
            i_newEntryToDelete++;
        } else if (curParent->NumOfKeys != StartIndexFirstParent) {
            movePointerInNodeForward(curParent, EndIndex+1, OldNumKeysInCurParent-1, StartIndexFirstParent);
        }
    }

    if (i_newEntryToDelete > 0) {
        deleteMulEntry(newEntryToDelete, i_newEntryToDelete);
    }
    delete []newEntryToDelete;
    return pre;
}


//move pointers forward without changing NumOfKeys
void DAstatic::movePointerInNodeForward(NodeForStree * node, int startIndex, int endIndex, int toIndex){
    int delta = startIndex - toIndex;
    for (int i = startIndex; i <= endIndex; i++) {
        node->Keys[i-delta] = node->Keys[i];
        node->Pointers[i-delta] = node->Pointers[i];
    }
}


// call this function after deleting a continuous range of leaves
void DAstatic::RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeForStree * node) {
    if (node == nullptr) {
        return;
    }
    if (!node->IsLeaf) {
        for (int i = 0; i < node->NumOfKeys; i++) {
            node->Keys[i] = NodeSumKeys(node->Pointers[i]);
        }
    }
    if (node == Root) {
        if (node->NumOfKeys == 1) {
            NodeForStree *newroot = node->Pointers[0];
            newroot->Parent = nullptr;
            Root = newroot;
        }
        return;
    }
    if (node->NumOfKeys == 0) {
        printf("impossible! In RedistributeMergeRecomputeKeysInAncestorsLeftPart, node->NumOfKeys=0\n");
        return;
    }
    NodeForStree * parent = node->Parent;
    int index = getIndexInParent(node);
    NodeForStree * neighbour;
    parent->Keys[index] = NodeSumKeys(node);
    if (node->NumOfKeys >= Min) {
        RedistributeMergeRecomputeKeysInAncestorsLeftPart(parent);
        return;
    } else {
        int neighbour_index = getLeftNeighborIndex(node);
        if (neighbour_index == -1) {
            if (parent->NumOfKeys == 1) {
                RedistributeMergeRecomputeKeysInAncestorsLeftPart(parent);
                RedistributeMergeRecomputeKeysInAncestorsLeftPart(node);
                return;
            }
            neighbour = parent->Pointers[1];
        } else {
            neighbour = parent->Pointers[neighbour_index];
        }
        if (neighbour->NumOfKeys+node->NumOfKeys <= Capacity) {
            node = coalesceNodesStopAtParent(node, neighbour, neighbour_index); //still too empty????? there is another pass when dealing with rightpart
            parent = node->Parent;
            RedistributeMergeRecomputeKeysInAncestorsLeftPart(parent);
            return;
        } else {
            redistributeNodes(node, neighbour, neighbour_index);
            parent = node->Parent;
            RedistributeMergeRecomputeKeysInAncestorsLeftPart(parent);
            return;
        }
    }
}


void DAstatic::adjustAncestorKeysAfterExchangingLeaves(NodeForStree ** nodeArray, int numNodeInArray) {
    if (nodeArray[0]->Parent == nullptr) {
        NodeForStree * root = nodeArray[0];
        for (int i = 0; i < root->NumOfKeys; i++) {
            root->Keys[i] = NodeSumKeys(root->Pointers[i]);
        }
        return; //root
    }
    NodeForStree ** newArray = new NodeForStree *[numNodeInArray];
    int numNewArray = 0;
    NodeForStree * lastParent = nullptr, * parent = nullptr;
    lastParent = nullptr;
    for (int i = 0; i < numNodeInArray; i++) {
        parent = nodeArray[i]->Parent;
        if (lastParent != parent) {
            newArray[numNewArray] = parent;
            lastParent = parent;
            numNewArray++;
        }
        NodeForStree * cur = nodeArray[i];
        for (int j = 0; j < cur->NumOfKeys; j++) {
            cur->Keys[j] = NodeSumKeys(cur->Pointers[j]);
        }
    }
    if (numNewArray > 1) {
        adjustAncestorKeysAfterExchangingLeaves(newArray, numNewArray);
        return;
    } else {
        for (int i = 0; i < parent->NumOfKeys; ++i) {
            parent->Keys[i] = NodeSumKeys(parent->Pointers[i]);
        }
        AdjustAncestorKeysAccordingToCurNode(parent);
    }
    delete []newArray;
}


void DAstatic::swapHeadTail11(NodeForStree * inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                              int endIndexLeaf2) {

    int num = endIndexLeaf2 - startIndexLeaf1 + 1;
    int * Keys = new int[num];
    int ik = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    int jk = 0;
    for (int i = startIndexLeaf1; i <= endIndexLeaf2; i++) {
        inter1->Keys[i] = Keys[jk];
        jk++;
    }
    delete []Keys;
}


void DAstatic::swapHeadTail12(NodeForStree * inter1, NodeForStree * inter2, int startIndexLeaf1, int endIndexLeaf1,
                              int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2 + 1 + inter1->NumOfKeys-startIndexLeaf1;
    int * Keys = new int[num];
    int  ik = 0;
    for (int i = startIndexLeaf2; i < inter1->NumOfKeys; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    int jk = 0;
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        inter1->Keys[i] = Keys[jk];
        jk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter2->Keys[i] = Keys[jk];
        jk++;
    }
    delete []Keys;
}


void DAstatic::swapHeadTail21(NodeForStree * inter1, NodeForStree * inter2, int startIndexLeaf1, int endIndexLeaf1,
                              int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2 + 1 + inter1->NumOfKeys-startIndexLeaf1;
    int * Keys = new int[num];
    int ik = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter2->Keys[i];
        ik++;
    }
    int jk = 0;
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        inter1->Keys[i] = Keys[jk];
        jk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter2->Keys[i] = Keys[jk];
        jk++;
    }
    delete []Keys;
}


void DAstatic::swapHeadTail22(NodeForStree * inter1, NodeForStree * inter2, NodeForStree * inter3, int startIndexLeaf1,
                              int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1 + inter2->NumOfKeys + inter1->NumOfKeys-startIndexLeaf1;
    int * Keys = new int[num];
    int ik = 0;
    for (int i = startIndexLeaf2; i < inter2->NumOfKeys; i++) {
        Keys[ik] = inter2->Keys[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter3->Keys[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        Keys[ik] = inter1->Keys[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter2->Keys[i];
        ik++;
    }

    int jk = 0;
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        inter1->Keys[i] = Keys[jk];
        jk++;
    }
    for (int i = 0; i < inter2->NumOfKeys; i++) {
        inter2->Keys[i] = Keys[jk];
        jk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter3->Keys[i] = Keys[jk];
        jk++;
    }
    delete []Keys;
}


void DAstatic::swapHeadTailLen1Is1(NodeForStree **inter2, int numLeaf2, int startIndexLeaf1,
                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int NumToMove = startIndexLeaf2 - startIndexLeaf1;
    int* KeysToMove = new int[NumToMove];
    int ik = 0;
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < inter2[0]->NumOfKeys-NumToMove; i++) {
        inter2[0]->Keys[i] = inter2[0]->Keys[i+NumToMove];
    }
    inter2[0]->NumOfKeys -= NumToMove;
    CompleteAdjustAncestorNodesAfterDelete(inter2[0]);
    insertKeysIntoLeaf(KeysToMove, NumToMove, inter2[numLeaf2-1], endIndexLeaf2 + 1);
    delete []KeysToMove;
}


void DAstatic::swapHeadTailLen2Is1(NodeForStree **inter1, int numLeaf1, int startIndexLeaf1,
                                   int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    NodeForStree * n = inter1[numLeaf1-1];
    int NumToMove = endIndexLeaf2 - endIndexLeaf1;
    int* KeysToMove = new int[NumToMove];
    int ik = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        KeysToMove[ik] = n->Keys[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = n->Keys[i];
        ik++;
    }
    for (int i = endIndexLeaf2 + 1; i < n->NumOfKeys; i++) {
        n->Keys[i-NumToMove] = n->Keys[i];
    }
    n->NumOfKeys -= NumToMove;
    CompleteAdjustAncestorNodesAfterDelete(n);
    insertKeysIntoLeaf(KeysToMove, NumToMove, inter1[0], startIndexLeaf1);
    delete []KeysToMove;
}


void DAstatic::swapHeadTailLen1Is2(NodeForStree **inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2,
                                   int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    NodeForStree * n = inter1[0];
    int NumToMove = startIndexLeaf2 + inter1[0]->NumOfKeys-startIndexLeaf1;
    int* KeysToMove = new int[NumToMove];
    int ik = 0;
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < n->NumOfKeys; i++) {
        KeysToMove[ik] = n->Keys[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        ik++;
    }

    //delete range 1
    DeleteMidPartOfTwoAdjacentLeaves(inter1[0], startIndexLeaf1, inter1[1], startIndexLeaf2-1);
    //insert
    NodeForStree * y = inter2[numLeaf2-1];
    insertKeysIntoLeaf(KeysToMove, NumToMove, y, endIndexLeaf2 + 1);
    delete []KeysToMove;
}


void DAstatic::swapHeadTailLen2Is2(NodeForStree **inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2,
                                   int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    NodeForStree * n = inter2[0];
    int NumToMove = endIndexLeaf2 + 1 + inter2[0]->NumOfKeys-endIndexLeaf1-1;
    int* KeysToMove = new int[NumToMove];
    int ik = 0;
    for (int i = startIndexLeaf2; i < n->NumOfKeys; i++) {
        KeysToMove[ik] = n->Keys[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        KeysToMove[ik] = inter2[1]->Keys[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = n->Keys[i];
        ik++;
    }
    //delete range 2
    DeleteMidPartOfTwoAdjacentLeaves(inter2[0], endIndexLeaf1+1, inter2[1], endIndexLeaf2);

    //insert
    NodeForStree * y = inter1[0];
    insertKeysIntoLeaf(KeysToMove, NumToMove, y, startIndexLeaf1);
    delete []KeysToMove;
}


void DAstatic::SwapRangesAdjacentWhenLensAreBoth2(NodeForStree **inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2,
                                                  int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  inter1[0]->NumOfKeys-startIndexLeaf1 + inter1[1]->NumOfKeys + inter2[0]->NumOfKeys + endIndexLeaf2+1;
    int * keys = new int[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; i++) {
        keys[tk] = inter2[0]->Keys[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        keys[tk] = inter2[1]->Keys[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < inter1[1]->NumOfKeys; i++) {
        keys[tk] = inter1[1]->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; i++) {
        keys[tk] = inter1[0]->Keys[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        keys[tk] = inter1[1]->Keys[i];
        tk++;
    }
    tk = 0;
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; i++) {
        inter1[0]->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i < inter1[1]->NumOfKeys; i++) {
        inter1[1]->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i < inter2[0]->NumOfKeys; i++) {
        inter2[0]->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter2[1]->Keys[i] = keys[tk];
        tk++;
    }
    delete []keys;
}

//starting start is two adjacent leaves
// in this function, we delete the end of first leaf and start of the second leaf
// then adjust these two leaves and their ancestors
NodeForStree * DAstatic::DeleteMidPartOfTwoAdjacentLeaves(NodeForStree * leaf1, int startIndexInLeaf1, NodeForStree * leaf2, int endIndexInLeaf2) {
    leaf1->NumOfKeys = startIndexInLeaf1;
    movePointerInNodeForward(leaf2, endIndexInLeaf2+1, leaf2->NumOfKeys-1, 0);
    leaf2->NumOfKeys -= endIndexInLeaf2 + 1;
    if (leaf1->NumOfKeys < Min) { // leaf1 < Min
        NodeForStree * neighbour,* newl;
        int neighbour_index = getLeftNeighborIndex(leaf1);
        if (neighbour_index == -1) { // leaf1 index is 0
            neighbour = leaf1->Parent->Pointers[1];
            if (neighbour->NumOfKeys + leaf1->NumOfKeys <= Capacity) { // leaf1 and leaf2 coalesce
                newl = coalesceNodes(leaf1, neighbour, neighbour_index);
                if (newl->NumOfKeys < Min) {
                    int nei_index = getLeftNeighborIndex(newl);
                    NodeForStree * nei;
                    if (nei_index == -1) {
                        nei = newl->Parent->Pointers[1];
                    } else {
                        nei = newl->Parent->Pointers[nei_index];
                    }
                    if (nei->NumOfKeys + newl->NumOfKeys <= Capacity) {
                        newl = coalesceNodes(newl, nei, nei_index);
                    } else {
                        redistributeNodes(newl, nei, nei_index);
                    }
                }
                return newl;
            } else { // leaf1 and leaf 2 redistribute
                redistributeNodes(leaf1, neighbour, neighbour_index);
                return leaf1;
            }
        } else { //leaf1 index >= 1
            NodeForStree * re = nullptr;
            neighbour = leaf1->Parent->Pointers[neighbour_index];
            if (neighbour->NumOfKeys+leaf1->NumOfKeys <= Capacity) {
                re  = coalesceNodes(leaf1, neighbour, neighbour_index);
            } else {
                redistributeNodes(leaf1, neighbour, neighbour_index);
            }
            CompleteAdjustAncestorNodesAfterDelete(leaf2);
            return re;
        }
    } else { // leaf1 enough
        AdjustAncestorKeysAccordingToCurNode(leaf1);
        CompleteAdjustAncestorNodesAfterDelete(leaf2);
        return leaf1;
    }
}

//to be chekced
void DAstatic::SwapRangesAdjacentRightIs2(NodeForStree **inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2,
                                          int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2 +1 + inter2[0]->NumOfKeys + inter1[numLeaf1-1]->NumOfKeys - endIndexLeaf1-1;
    int * keys = new int[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; i++) {
        keys[tk] = inter2[0]->Keys[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        keys[tk] = inter2[1]->Keys[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < inter1[numLeaf1-1]->NumOfKeys; i++) {
        keys[tk] = inter1[numLeaf1-1]->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        tk++;
    }
    //delete
    NodeForStree * pre = inter1[numLeaf1-1]; // findPrevious(inter2[0]);
    pre->Next = inter2[1]; //->Next;
    /*
    inter2[0]->Previous->Next = inter2[0]->Next;
    if (inter2[0]->Next != nullptr) {
        inter2[0]->Next->Previous = inter2[0]->Previous;
    }
     */
    inter2[0]->NumOfKeys=0;
    deleteEntry(inter2[0]->Parent, getIndexInParent(inter2[0]));

    DeleteMidPartOfTwoAdjacentLeaves(inter1[numLeaf1-1], endIndexLeaf1 + 1, inter2[1], endIndexLeaf2);
    insertKeysIntoLeaf(keys, num, inter1[0], startIndexLeaf1);
    delete []keys;
}


void DAstatic::SwapRangesAdjacentLeftIs2(NodeForStree **inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2,
                                         int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  startIndexLeaf2 + inter1[1]->NumOfKeys + inter1[0]->NumOfKeys - startIndexLeaf1;
    int * keys = new int[num];
    int tk = 0;
    for (int i = endIndexLeaf1 + 1; i < inter1[1]->NumOfKeys; i++) {
        keys[tk] = inter1[1]->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; i++) {
        keys[tk] = inter1[0]->Keys[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        keys[tk] = inter1[1]->Keys[i];
        tk++;
    }
    //delete
    NodeForStree ** LeafToDelete = new NodeForStree*[1];
    LeafToDelete[0] = inter1[1];
    inter1[0]->Next = inter1[1]->Next;
    /*
    if (inter1[0]->Next != nullptr) {
        inter1[0]->Next->Previous = inter1[0];
    }
     */
    deleteEntry(inter1[1]->Parent, getIndexInParent(inter1[1]));
    inter1[1]->NumOfKeys=0;

    DeleteMidPartOfTwoAdjacentLeaves(inter1[0], startIndexLeaf1, inter2[0], startIndexLeaf2-1);

    insertKeysIntoLeaf(keys,num, inter2[numLeaf2-1], endIndexLeaf2+1);
    delete []keys;
}



void DAstatic::SwapRangesAdjacentWhenLensAreBoth1(NodeForStree * leftnode, NodeForStree * rightnode, int startIndexLeaf1,
                                                  int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1+leftnode->NumOfKeys-startIndexLeaf1;
    int * keys = new int[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        keys[tk] = rightnode->Keys[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < leftnode->NumOfKeys; i++) {
        keys[tk] = leftnode->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = rightnode->Keys[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        keys[tk] = leftnode->Keys[i];
        tk++;
    }
    tk = 0;
    for (int i = startIndexLeaf1; i < leftnode->NumOfKeys; i++) {
        leftnode->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        rightnode->Keys[i] = keys[tk];
        tk++;
    }
    delete []keys;
}


void DAstatic::SwapRangesAdjacentWhenLensAre1And2(NodeForStree * leftnode, NodeForStree* right1, NodeForStree * right2,
                                                  int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1+right1->NumOfKeys+leftnode->NumOfKeys-startIndexLeaf1;
    int * keys = new int[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
        keys[tk] = right1->Keys[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        keys[tk] = right2->Keys[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < leftnode->NumOfKeys; i++) {
        keys[tk] = leftnode->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = right1->Keys[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        keys[tk] = leftnode->Keys[i];
        tk++;
    }

    tk = 0;
    for (int i = startIndexLeaf1; i < leftnode->NumOfKeys; i++) {
        leftnode->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i < right1->NumOfKeys; i++) {
        right1->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        right2->Keys[i] = keys[tk];
        tk++;
    }
    delete []keys;
}


void DAstatic::SwapRangesAdjacentWhenLensAre2And1(NodeForStree * left1, NodeForStree * left2, NodeForStree * right,
                                                  int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1 + left2->NumOfKeys + left1->NumOfKeys-startIndexLeaf1;
    int * keys = new int[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < left2->NumOfKeys; i++) {
        keys[tk] = left2->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
        keys[tk] = left1->Keys[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        keys[tk] = left2->Keys[i];
        tk++;
    }

    tk = 0;
    for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
        left1->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i < left2->NumOfKeys; i++) {
        left2->Keys[i] = keys[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        right->Keys[i] = keys[tk];
        tk++;
    }
    delete []keys;
}


void DAstatic::SwapRangesAdjacentWhenLeftIs1(NodeForStree * left, NodeForStree ** inter2, int numLeaf2,
                                             int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int NumToMove = startIndexLeaf2 + left->NumOfKeys - startIndexLeaf1;
    int * keys = new int[NumToMove];
    int tk = 0;
    for (int i = endIndexLeaf1 + 1; i < left->NumOfKeys; i++) {
        keys[tk] = left->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        keys[tk] = left->Keys[i];
        tk++;
    }
    //delete
    DeleteMidPartOfTwoAdjacentLeaves(left, startIndexLeaf1, inter2[0], startIndexLeaf2-1);
    //insert
    insertKeysIntoLeaf(keys, NumToMove, inter2[numLeaf2 - 1], endIndexLeaf2 + 1);
    delete []keys;
}



void DAstatic::SwapRangesAdjacentWhenRightIs1(NodeForStree **inter1, int numLeaf1, NodeForStree * right,
                                              int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int numToMove = endIndexLeaf2+1 + inter1[numLeaf1-1]->NumOfKeys - endIndexLeaf1-1;
    int * keys = new int[numToMove];
    int tk = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < inter1[numLeaf1-1]->NumOfKeys; i++) {
        keys[tk] = inter1[numLeaf1-1]->Keys[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        tk++;
    }
    //delete
    DeleteMidPartOfTwoAdjacentLeaves(inter1[numLeaf1-1], endIndexLeaf1 + 1, right, endIndexLeaf2);

    //insert
    insertKeysIntoLeaf(keys, numToMove, inter1[0], startIndexLeaf1);
    delete []keys;
}


//to be ckeched
void DAstatic::SwapRangesAdjacent(NodeForStree ** inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2,
                                  int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2){
    if (numLeaf1 == 1 && numLeaf2 == 1) {
        SwapRangesAdjacentWhenLensAreBoth1(inter1[0], inter2[0], startIndexLeaf1, endIndexLeaf1,
                                           startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf1 == 1 && numLeaf2 == 2) {
        SwapRangesAdjacentWhenLensAre1And2(inter1[0], inter2[0], inter2[1], startIndexLeaf1, endIndexLeaf1,
                                           startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf1 == 2 && numLeaf2 == 1) {
        SwapRangesAdjacentWhenLensAre2And1(inter1[0], inter1[1], inter2[0], startIndexLeaf1, endIndexLeaf1,
                                           startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf1 == 1) {
        SwapRangesAdjacentWhenLeftIs1(inter1[0], inter2, numLeaf2, startIndexLeaf1, endIndexLeaf1,
                                      startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf2 == 1) {
        SwapRangesAdjacentWhenRightIs1(inter1, numLeaf1, inter2[0], startIndexLeaf1, endIndexLeaf1,
                                       startIndexLeaf2, endIndexLeaf2);
        return;
    }
    if (numLeaf1 == 2 && numLeaf2 == 2) {
        SwapRangesAdjacentWhenLensAreBoth2(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1,
                                           startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf1 == 2) {
        SwapRangesAdjacentLeftIs2(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1,
                                  startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf2 == 2) {
        SwapRangesAdjacentRightIs2(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1,
                                   startIndexLeaf2, endIndexLeaf2);
        return;
    }
    swapWholeLeafAndGoUpwards(inter1, &inter2[1], numLeaf1-1, numLeaf2-1);

    int numToMove1 = endIndexLeaf1 + 1 + inter2[numLeaf2-1]->NumOfKeys - endIndexLeaf2 - 1;
    int* KeyToMove1 = new int[numToMove1];
    int i1 = 0;
    for (int i = 0; i <= endIndexLeaf1; i++) {
        KeyToMove1[i1] = inter1[numLeaf1-1]->Keys[i];
        i1++;
    }
    for (int i = endIndexLeaf2 + 1; i < inter2[numLeaf2-1]->NumOfKeys; i++) {
        KeyToMove1[i1] = inter2[numLeaf2-1]->Keys[i];
        i1++;
    }

    int numToMove2 = startIndexLeaf1 + inter2[0]->NumOfKeys - startIndexLeaf2;
    int * KeyToMove2 = new int[numToMove2];
    int i2 = 0;
    for (int i = 0; i < startIndexLeaf1; i++) {
        KeyToMove2[i2] = inter1[0]->Keys[i];
        i2++;
    }
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; i++) {
        KeyToMove2[i2] = inter2[0]->Keys[i];
        i2++;
    }

    //insert for range 1
    insertKeysIntoLeaf(KeyToMove1, numToMove1, inter1[numLeaf1-2], inter1[numLeaf1 - 2]->NumOfKeys);

    //insert for range 2
    bool changePointer = insertKeysIntoLeaf(KeyToMove2, numToMove2, inter2[1], 0);
    if (changePointer) {
        inter2[1] = inter2[1]->Next;
    }

    //middle pointers
    int NumMiddlePointers = (inter1[numLeaf1-1]->NumOfKeys - endIndexLeaf1 - 1) + (startIndexLeaf2);
    int * Middlekeys = new int[NumMiddlePointers];
    int mi = 0;
    for (int i = endIndexLeaf1 + 1; i < inter1[numLeaf1-1]->NumOfKeys; i++) {
        Middlekeys[mi] = inter1[numLeaf1-1]->Keys[i];
        mi++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        Middlekeys[mi] = inter2[0]->Keys[i];
        mi++;
    }

    //delete two leaves in the middle
    inter1[numLeaf1-1]->NumOfKeys=0;
    inter2[0]->NumOfKeys=0;
    NodeForStree ** ToDelete = new NodeForStree*[2];
    ToDelete[0] = inter1[numLeaf1-1];
    ToDelete[1] = inter2[0];
    NodeForStree * pre = inter2[numLeaf2-1];// inter1[numLeaf1-2];
    if (pre != nullptr) {
        pre->Next = inter2[0]->Next;
    }
    /*
    if (inter1[numLeaf1-1]->Previous != nullptr) {
        inter1[numLeaf1-1]->Previous->Next = inter2[0]->Next;
        if (inter2[0]->Next != nullptr) {
            inter2[0]->Next->Previous = inter1[numLeaf1-1]->Previous;
        }
    } else {
        if (inter2[0]->Next != nullptr) {
            inter2[0]->Next->Previous = inter1[numLeaf1-1]->Previous;
        }
    }
    */
    NodeForStree * preLeaf = deleteMulEntry(ToDelete, 2);
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, ToDelete, 2);
    ReplaceMidOfTwoAdjacentLeaves(inter2[numLeaf2-1], inter1[0], endIndexLeaf2+1, startIndexLeaf1-1,
                                  Middlekeys, NumMiddlePointers);
    delete []KeyToMove1;
    delete []KeyToMove2;
    delete []Middlekeys;
    delete []ToDelete;
}

//return true if toIndex == 0 and insertMulKeysIntoLeafAfterOneSplitting returns true
bool DAstatic::insertKeysIntoLeaf(int * keysToInsert, int numKeysToInsert, NodeForStree * toLeaf, int toIndex) {
    if (toLeaf->NumOfKeys + numKeysToInsert <= Capacity) {
        insertMulKeysIntoLeaf(keysToInsert, numKeysToInsert, toLeaf, toIndex);
        return false;
    } else if (toLeaf->NumOfKeys + numKeysToInsert <= 2*Capacity) {
        return insertMulKeysIntoLeafAfterOneSplitting(keysToInsert, numKeysToInsert, toLeaf, toIndex);
    } else {
        insertMulKeysIntoLeafAfterMulSplitting(keysToInsert, numKeysToInsert, toLeaf, toIndex);
        return false;
    }
}

//split maybe more than once
void DAstatic::insertMulKeysIntoLeafAfterMulSplitting(int * keysToInsert, int numKeysToInsert,
                                                      NodeForStree * toLeaf, int toIndex) {
    int totalNumKeys = numKeysToInsert + toLeaf->NumOfKeys;
    int * newKeys = new int[totalNumKeys];
    int i = 0;
    for (int j = 0; j < toIndex; j++) {
        newKeys[i] = toLeaf->Keys[j];
        i++;
    }
    for (int j = 0; j < numKeysToInsert; j++) {
        newKeys[i] = keysToInsert[j];
        i++;
    }
    for (int j = toIndex; j < toLeaf->NumOfKeys; j++) {
        newKeys[i] = toLeaf->Keys[j];
        i++;
    }
    toLeaf->NumOfKeys = 0;
    int numFullParent = totalNumKeys / Capacity;
    int remainNumLeaf = totalNumKeys - numFullParent*Capacity;
    if (remainNumLeaf == 0 || remainNumLeaf >= Min) { // full nodes + remainnumleaf
        int ik = 0;
        for (; ik < Capacity; ik++) {
            toLeaf->Keys[ik] = newKeys[ik];
        }
        toLeaf->NumOfKeys = Capacity;
        int parentNewSiblingNum = numFullParent - 1;
        if (remainNumLeaf != 0) {
            parentNewSiblingNum++;
        }
        NodeForStree ** parentNewSibling = new NodeForStree*[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = makeLeafForStree(Capacity);
            for (int y = 0; y < Capacity; y++) {
                parentNewSibling[k]->Keys[y] = newKeys[ik]; //allPointers[i];->NumOfKeys
                parentNewSibling[k]->NumOfKeys++;
                ik++;
                if (ik == totalNumKeys) {
                    break;
                }
            }
        }

        for (int p = 0; p < parentNewSiblingNum-1; p ++) {
            parentNewSibling[p]->Next = parentNewSibling[p+1];
            //parentNewSibling[p+1]->Previous = parentNewSibling[p];
        }

        NodeForStree * pa = toLeaf->Parent;
        int startindex = getIndexInParent(toLeaf)+1;
        pa->Keys[startindex-1] = NodeSumKeys(toLeaf);
        insertLeavesIntoLeafParent(pa, startindex, parentNewSibling, parentNewSiblingNum);
        delete []newKeys;
        delete []parentNewSibling;
        return;
    } else {
        int parentNewSiblingNum = numFullParent;
        numFullParent--;
        int numkeysInSecondLastParent = (totalNumKeys - numFullParent*Capacity) / 2;
        int numkeysInLastParent = totalNumKeys - numFullParent*Capacity - numkeysInSecondLastParent;
        int i_allpointer = 0;
        if (numFullParent == 0) { //toParent is the second last one
            for (int y = 0; y < numkeysInSecondLastParent; y++) {
                toLeaf->Keys[y] = newKeys[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                i_allpointer++;
            }
            toLeaf->NumOfKeys = numkeysInSecondLastParent;
        } else {
            for (int y = 0; y < Capacity; y++) {
                toLeaf->Keys[y] = newKeys[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                i_allpointer++;
            }
            toLeaf->NumOfKeys = Capacity;
        }
        //set parent's new siblings
        NodeForStree ** parentNewSibling  = new NodeForStree*[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = makeLeafForStree(Capacity);
            if (k == parentNewSiblingNum-1) { // the last one
                for (int y = 0; y < numkeysInLastParent; y++) {
                    parentNewSibling[k]->Keys[y] = newKeys[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                    i_allpointer++;
                }
                parentNewSibling[k]->NumOfKeys = numkeysInLastParent;
            } else if (k == parentNewSiblingNum-2) { // the second last one
                for (int y = 0; y < numkeysInSecondLastParent; y++) {
                    parentNewSibling[k]->Keys[y] = newKeys[i_allpointer];//allPointers[i_allpointer]->NumOfKeys
                    i_allpointer++;
                }
                parentNewSibling[k]->NumOfKeys = numkeysInSecondLastParent;
            } else {
                for (int y = 0; y < Capacity; y++) {
                    parentNewSibling[k]->Keys[y] = newKeys[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                    i_allpointer++;
                }
                parentNewSibling[k]->NumOfKeys = Capacity;
            }
        }
        for (int p = 0; p < parentNewSiblingNum-1; p++) {
            parentNewSibling[p]->Next = parentNewSibling[p+1];
            //parentNewSibling[p+1]->Previous = parentNewSibling[p];
        }

        NodeForStree ** paNode = &parentNewSibling[0];
        NodeForStree * pa = toLeaf->Parent;
        int startindex = getIndexInParent(toLeaf)+1;
        pa->Keys[startindex-1] = NodeSumKeys(toLeaf);
        insertLeavesIntoLeafParent(pa, startindex, paNode, parentNewSiblingNum);
        delete []newKeys;
        delete []parentNewSibling;
    }
}

void DAstatic::SwapWhenLeftLenIs2(NodeForStree * left1, NodeForStree * left2, int startIndexLeaf1, int endIndexLeaf1,
                                  NodeForStree ** inter2, int numLeaf2, int startIndexLeaf2, int endIndexLeaf2) {
    //delete leaves
    inter2[0]->Next = inter2[numLeaf2-1];
    //inter2[numLeaf2-1]->Previous = inter2[0];

    //only delete entries, no redistribution or merging
    NodeForStree * preLeaf = deleteMulEntry(&inter2[1], numLeaf2-2);
    // adjust keys in ancestors
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, &inter2[1], numLeaf2-2);
    //insert leaves
    NodeForStree * toParent = left1->Parent;
    int insertIndexInToParent = getIndexInParent(left1) + 1;
    insertLeavesIntoLeafParent(toParent, insertIndexInToParent, &inter2[1], numLeaf2-2);

    //insert keys at the end of inter2[0] to the beginning of inter2[1], without deleting these keys
    int numToInsert20 = inter2[0]->NumOfKeys - startIndexLeaf2;
    int * keysToInset20 = new int[numToInsert20];
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; ++i) {
        keysToInset20[i-startIndexLeaf2] = inter2[0]->Keys[i];
    }
    bool changePointer = insertKeysIntoLeaf(keysToInset20, numToInsert20, inter2[1], 0);
    if (changePointer) {
        inter2[1] = inter2[1]->Next;
    }

    //insert keys at the beginning of inter2[numLeaf2-1] to the end of inter2[numLeaf2-2], without deleting these keys
    int numToInsert22 = endIndexLeaf2 + 1;
    int * keysToInset22 = new int[numToInsert22];
    for (int j = 0; j <= endIndexLeaf2; ++j) {
        keysToInset22[j] = inter2[numLeaf2-1]->Keys[j];
    }
    insertKeysIntoLeaf(keysToInset22, numToInsert22, inter2[numLeaf2-2], inter2[numLeaf2-2]->NumOfKeys);

    //keys and pointers of left1, left2
    int numToMoveInLeft = left1->NumOfKeys - startIndexLeaf1 + endIndexLeaf1 + 1;
    int * keysToMoveInLeft = new int[numToMoveInLeft];
    int idxToMoveInLeft = 0;
    for (int i = startIndexLeaf1; i < left1->NumOfKeys; ++i) {
        keysToMoveInLeft[idxToMoveInLeft] = left1->Keys[i];
        idxToMoveInLeft++;
    }
    for (int i = 0; i <= endIndexLeaf1; ++i) {
        keysToMoveInLeft[idxToMoveInLeft] = left2->Keys[i];
        idxToMoveInLeft++;
    }

    //replace keys in inter2[0], inter2[numLeaf2-1] with pointersToMoveInLeft, keysToMoveInLeft
    ReplaceMidOfTwoAdjacentLeaves(inter2[0], inter2[numLeaf2-1], startIndexLeaf2, endIndexLeaf2,
                                  keysToMoveInLeft, numToMoveInLeft);
    //delete end keys in left
    left1->NumOfKeys = startIndexLeaf1;
    CompleteAdjustAncestorNodesAfterDelete(left1);
    //delete start keys in right
    movePointerInNodeForward(left2, endIndexLeaf1+1, left2->NumOfKeys-1, 0);
    left2->NumOfKeys -= endIndexLeaf1 + 1;
    CompleteAdjustAncestorNodesAfterDelete(left2);
    delete []keysToInset20;
    delete []keysToInset22;
    delete []keysToMoveInLeft;
}

//replace the mid part of two adjacent leaves and go upwards
//without deleting anything
void DAstatic::ReplaceMidOfTwoAdjacentLeaves(NodeForStree * left, NodeForStree * right, int startIndexLeft, int endIndexRight,
                                             int * keysToInsert, int numToInsert) {
    int numToBeReplaced = left->NumOfKeys - startIndexLeft + endIndexRight + 1;
    int idxToMoveInLeft = 0;
    if (numToBeReplaced == numToInsert) {
        for (int i = startIndexLeft; i < left->NumOfKeys; ++i) {
            left->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        for (int i = 0; i <= endIndexRight; ++i) {
            right->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        AdjustAncestorKeysAccordingToCurNode(left);
        AdjustAncestorKeysAccordingToCurNode(right);
        return;
    } else if (numToBeReplaced < numToInsert) {
        for (int i = startIndexLeft; i < left->NumOfKeys; ++i) {
            left->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        for (int i = 0; i <= endIndexRight; ++i) {
            right->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        AdjustAncestorKeysAccordingToCurNode(left);
        insertKeysIntoLeaf(&keysToInsert[numToBeReplaced],
                           numToInsert - numToBeReplaced, right, endIndexRight+1);
        return;
    } else {// numToBeReplaced > numToInsert
        int numToBeReplaced1 = left->NumOfKeys - startIndexLeft, numToBeReplaced2 = endIndexRight + 1;
        if (numToInsert < numToBeReplaced1) {
            int li = startIndexLeft;
            for (; idxToMoveInLeft < numToInsert; ++idxToMoveInLeft) {
                left->Keys[li] = keysToInsert[idxToMoveInLeft];
                li++;
            }
            DeleteMidPartOfTwoAdjacentLeaves(left, li, right, endIndexRight);
            return;
        } else if (numToInsert == numToBeReplaced1) {
            for (int i = startIndexLeft; i < left->NumOfKeys; ++i) {
                left->Keys[i] = keysToInsert[idxToMoveInLeft];
                idxToMoveInLeft++;
            }
            movePointerInNodeForward(right, endIndexRight+1, right->NumOfKeys-1, 0);
            right->NumOfKeys -= endIndexRight + 1;
            //adjust two adjacent leaves one by one
            AdjustAncestorKeysAccordingToCurNode(left);
            CompleteAdjustAncestorNodesAfterDelete(right);
            return;
        } else { // numToInsert < numToBeReplaced
            for (int i = startIndexLeft; i < left->NumOfKeys; ++i) {
                left->Keys[i] = keysToInsert[idxToMoveInLeft];
                idxToMoveInLeft++;
            }
            int ri = 0;
            for (; idxToMoveInLeft < numToInsert; ++idxToMoveInLeft) {
                right->Keys[ri] = keysToInsert[idxToMoveInLeft];
                ri++;
            }
            movePointerInNodeForward(right, endIndexRight+1, right->NumOfKeys-1, ri);
            right->NumOfKeys -= endIndexRight + 1 - ri;
            //adjust two adjacent leaves one by one
            AdjustAncestorKeysAccordingToCurNode(left);
            CompleteAdjustAncestorNodesAfterDelete(right);
            return;
        }
    }
}


void DAstatic::SwapWhenBothLensAre2(NodeForStree * left1, NodeForStree * left2, int startIndexLeaf1, int endIndexLeaf1,
                                    NodeForStree * right1, NodeForStree * right2, int startIndexLeaf2, int endIndexLeaf2) {
    int len11 = left1->NumOfKeys - startIndexLeaf1;
    int len12 = endIndexLeaf1+1;
    int len21 = right1->NumOfKeys - startIndexLeaf2;
    int len22 = endIndexLeaf2+1;
    int len1 = len11+len12;
    int len2 = len21+len22;

    if (len1 < len21) {
        int * keys1 = new int[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            i2++;
            if (i2 == len1) {
                break;
            }
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            i2++;
        }
        int newstart2 = startIndexLeaf2+len1;
        int numKeysToMove = len2-len1;
        int * keysToMove = new int[numKeysToMove];
        int ik = 0;
        for (int i = newstart2; i < right1->NumOfKeys; i++) {
            keysToMove[ik] = right1->Keys[i];
            ik++;
        }
        for (int i = 0; i <= endIndexLeaf2; i++) {
            keysToMove[ik] = right2->Keys[i];
            ik++;
        }
        insertKeysIntoLeaf(keysToMove, numKeysToMove, left2, endIndexLeaf1 + 1);
        AdjustAncestorKeysAccordingToCurNode(left1);
        DeleteMidPartOfTwoAdjacentLeaves(right1, startIndexLeaf2 + len1, right2, endIndexLeaf2);
        delete []keys1;
        delete []keysToMove;
        return;
    } else if (len1 == len21) {
        int * keys1 = new int[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int  tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            i2++;
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            i2++;
        }
        int numKeysToMove = len22;
        int * keysToMove = new int[numKeysToMove];
        int ik = 0;
        for (int i = 0; i <= endIndexLeaf2; i++) {
            keysToMove[ik] = right2->Keys[i];
            ik++;
        }
        insertKeysIntoLeaf(keysToMove, numKeysToMove, left2, endIndexLeaf1 + 1);
        AdjustAncestorKeysAccordingToCurNode(left1);
        movePointerInNodeForward(right2, endIndexLeaf2+1, right2->NumOfKeys-1, 0);
        right2->NumOfKeys -= len22;
        CompleteAdjustAncestorNodesAfterDelete(right2);
        AdjustAncestorKeysAccordingToCurNode(right1);
        delete []keys1;
        delete []keysToMove;
        return;
    } else if (len1 < len2) { // len2 > len1 > len21
        int * keys1 = new int[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            i2++;
        }
        int newstart2 = 0;
        for (int i = 0; i <= endIndexLeaf2; i++) {
            int tmp = right2->Keys[i];
            right2->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            i2++;
            if (i2 == len1) {
                newstart2 = i+1;
                break;
            }
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            i2++;
        }
        int numKeysToMove = len2 - len1;
        int * keysToMove = new int[numKeysToMove];
        int ik = 0;
        for (int i = newstart2; i <= endIndexLeaf2; i++) {
            keysToMove[ik] = right2->Keys[i];
            ik++;
        }
        AdjustAncestorKeysAccordingToCurNode(left1);
        insertKeysIntoLeaf(keysToMove, numKeysToMove, left2, endIndexLeaf1 + 1);
        AdjustAncestorKeysAccordingToCurNode(right1);
        movePointerInNodeForward(right2, endIndexLeaf2+1, right2->NumOfKeys-1, newstart2);
        right2->NumOfKeys -= numKeysToMove;
        CompleteAdjustAncestorNodesAfterDelete(right2);
        delete []keys1;
        delete []keysToMove;
        return;
    } else if (len1 == len2) {
        int * keys1 = new int[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf2; i++) {
            int tmp = right2->Keys[i];
            right2->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            i2++;
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            i2++;
        }
        AdjustAncestorKeysAccordingToCurNode(left1);
        AdjustAncestorKeysAccordingToCurNode(left2);
        AdjustAncestorKeysAccordingToCurNode(right1);
        AdjustAncestorKeysAccordingToCurNode(right2);
        delete []keys1;
        return;
    } else { // len1 > len2
        SwapWhenBothLensAre2(right1, right2, startIndexLeaf2, endIndexLeaf2, left1, left2, startIndexLeaf1, endIndexLeaf1);
        return;
    }
}



void DAstatic::SwapWhenBothLensAre1(NodeForStree * leaf1, int startIndex1, int endIndex1, NodeForStree * leaf2,
                                    int startIndex2, int endIndex2) {
    int numToMove1 = endIndex1 - startIndex1 + 1;
    int numToMove2 = endIndex2 - startIndex2 + 1;
    int exchangeNum = numToMove1;
    if (exchangeNum > numToMove2) {
        exchangeNum = numToMove2;
    }

    for (int i = 0; i < exchangeNum; i++) {
        int tmpk = leaf1->Keys[startIndex1+i];
        leaf1->Keys[startIndex1+i] = leaf2->Keys[startIndex2+i];
        leaf2->Keys[startIndex2+i] = tmpk;
    }
    if (numToMove1 == numToMove2) {
        AdjustAncestorKeysAccordingToCurNode(leaf1);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        return;
    } else if (numToMove1 < numToMove2) {
        int numKeys = numToMove2 - numToMove1;
        int * keys = new int[numKeys];
        for (int i = 0; i < numKeys; ++i) {
            keys[i] = leaf2->Keys[startIndex2 + exchangeNum + i];
        }
        movePointerInNodeForward(leaf2, endIndex2+1, leaf2->NumOfKeys-1, startIndex2 + exchangeNum);
        leaf2->NumOfKeys -= numKeys;
        CompleteAdjustAncestorNodesAfterDelete(leaf2);
        insertKeysIntoLeaf(keys, numKeys, leaf1, endIndex1+1);
        delete []keys;
        return;
    } else { //numToMove1 > numToMove2
        int numKeys = numToMove1 - numToMove2;
        int * keys = new int[numKeys];
        for (int i = 0; i < numKeys; ++i) {
            keys[i] = leaf1->Keys[startIndex1 + exchangeNum + i];
        }
        movePointerInNodeForward(leaf1, endIndex1+1, leaf1->NumOfKeys-1, startIndex1 + exchangeNum);
        leaf1->NumOfKeys -= numKeys;
        CompleteAdjustAncestorNodesAfterDelete(leaf1);
        insertKeysIntoLeaf(keys, numKeys, leaf2, endIndex2+1);
        delete []keys;
        return;
    }
}


void DAstatic::SwapWhenLensAre1And2(NodeForStree * leaf1, int startIndex1, int endIndex1, NodeForStree * leaf2,
                                    int startIndex2, NodeForStree * leaf3, int endIndex3) {
    int len1 = endIndex1 - startIndex1 + 1;
    int len2 = leaf2->NumOfKeys - startIndex2;
    int len3 = endIndex3 + 1;
    if (len1 < len2) {
        for (int i = 0; i < len1; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[startIndex1+i];
            leaf1->Keys[startIndex1+i] = tmpk;
        }
        int numToMove = len2 - len1 + len3;
        int * keysToMove = new int[numToMove];
        int tk = 0;
        for (int i = startIndex2 + len1; i < leaf2->NumOfKeys; i++) {
            keysToMove[tk] = leaf2->Keys[i];
            tk++;
        }
        for (int i = 0; i < len3; i++) {
            keysToMove[tk] = leaf3->Keys[i];
            tk++;
        }
        insertKeysIntoLeaf(keysToMove, numToMove, leaf1, endIndex1 + 1);
        DeleteMidPartOfTwoAdjacentLeaves(leaf2, startIndex2 + len1, leaf3, endIndex3);
        delete []keysToMove;
        return;
    } else if (len1 == len2) {
        for (int i = 0; i < len1; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[startIndex1+i];
            leaf1->Keys[startIndex1+i] = tmpk;
        }
        int * keysToMove = new int[len3];
        for (int i = 0; i < len3; i++) {
            keysToMove[i] = leaf3->Keys[i];
        }
        movePointerInNodeForward(leaf3, endIndex3+1, leaf3->NumOfKeys-1, 0);
        leaf3->NumOfKeys -= len3;
        CompleteAdjustAncestorNodesAfterDelete(leaf3);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        insertKeysIntoLeaf(keysToMove, len3, leaf1, endIndex1 + 1);
        delete []keysToMove;
        return;
    } else if (len1 < len2+len3) {
        int indexIn1 = startIndex1;
        for (int i = 0; i < len2; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            indexIn1++;
        }
        int newStartIn3 = len1 - len2;
        for (int i = 0; i < newStartIn3; i++) {
            int tmpk = leaf3->Keys[i];
            leaf3->Keys[i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            indexIn1++;
        }
        int NumKeysToMove = endIndex3 - newStartIn3 + 1;
        int * keysToMove = new int[NumKeysToMove];
        for (int i = 0; i < NumKeysToMove; i++) {
            keysToMove[i] = leaf3->Keys[newStartIn3+i];
        }
        insertKeysIntoLeaf(keysToMove, NumKeysToMove, leaf1, endIndex1 + 1);
        movePointerInNodeForward(leaf3, len3, leaf3->NumOfKeys-1, len1-len2);
        leaf3->NumOfKeys -= NumKeysToMove;
        CompleteAdjustAncestorNodesAfterDelete(leaf3);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        delete []keysToMove;
        return;
    } else if (len1 == len2+len3) {
        int indexIn1 = startIndex1;
        for (int i = 0; i < len2; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            indexIn1++;
        }
        for (int i = 0; i < len3; i++) {
            int tmpk = leaf3->Keys[i];
            leaf3->Keys[i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            indexIn1++;
        }
        AdjustAncestorKeysAccordingToCurNode(leaf1);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        AdjustAncestorKeysAccordingToCurNode(leaf3);
    } else if (len1 > len2+len3) { //len1 > len21+len22
        int indexIn1 = startIndex1;
        for (int i = 0; i < len2; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            indexIn1++;
        }
        for (int i = 0; i < len3; i++) {
            int tmpk = leaf3->Keys[i];
            leaf3->Keys[i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            indexIn1++;
        }
        int newStart3 = indexIn1;
        int NumToMove = len1 - len2 - len3;
        int* KeysToMove = new int[NumToMove];
        for (int i = newStart3; i <= endIndex1; i++) {
            KeysToMove[i-newStart3] = leaf1->Keys[i];
        }
        insertKeysIntoLeaf(KeysToMove, NumToMove, leaf3, endIndex3 + 1);
        for (int i = endIndex1 + 1; i < leaf1->NumOfKeys; i++) {
            leaf1->Keys[i-NumToMove] = leaf1->Keys[i];
            leaf1->Pointers[i-NumToMove] = leaf1->Pointers[i];
        }
        leaf1->NumOfKeys -= NumToMove;
        CompleteAdjustAncestorNodesAfterDelete(leaf1);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        delete []KeysToMove;
        return;
    }
}


NodeForStree * DAstatic::coalesceNodesStopAtParent(NodeForStree * n, NodeForStree * neighbour, int  neighbour_index) {
    int i, j, neighbour_insertion_index;
    NodeForStree * tmp;
    int n_index = neighbour_index + 1;
    //set neighbor to be the node on the left
    if (neighbour_index == -1) {
        tmp = n;
        n = neighbour;
        neighbour = tmp;
        n_index = 1;
        neighbour_index = 0;
    }
    neighbour_insertion_index = neighbour->NumOfKeys;
    i = neighbour_insertion_index;
    int newParentKey = NodeSumKeys(neighbour) + NodeSumKeys(n);

    for (j = 0; j < n->NumOfKeys; j++) {
        neighbour->Keys[i] = n->Keys[j];
        neighbour->Pointers[i] = n->Pointers[j];
        tmp = neighbour->Pointers[i];
        if (tmp != nullptr) {
            tmp->Parent = neighbour;
        }
        i += 1;
    }
    neighbour->NumOfKeys = neighbour->NumOfKeys + n->NumOfKeys;
    neighbour->Parent->Keys[neighbour_index] = newParentKey;
    if (neighbour->IsLeaf) {
        neighbour->Next = n->Next;
        /*
        if (neighbour->Next != nullptr) {
            NodeForStree * tmp = neighbour->Next;
            tmp->Previous = neighbour;
        }
         */
    }
    n->NumOfKeys = 0;
    removeEntryFromNode(neighbour->Parent, n_index);
    neighbour->Parent->NumOfKeys--;
    if (neighbour->Parent == Root) {
        adjustRoot();
    }
    return neighbour;
}


//return merging node
// LeftRight = 1 if rightleaf more keys
//LeftRight = 2 if left leaf more keys
NodeForStree * DAstatic::swapStartPointer(NodeForStree * leftLeaf, int endIndexInLeftLeaf, NodeForStree *rightLeaf, int endIndexInRightLeaf) {
    int i = 0;
    int j = 0;
    int exchangeNum = endIndexInLeftLeaf;
    if (exchangeNum > endIndexInRightLeaf) {
        exchangeNum = endIndexInRightLeaf;
    }
    for (; i <= exchangeNum; i++) {
        int tmpKey = leftLeaf->Keys[i];
        leftLeaf->Keys[i] = rightLeaf->Keys[j];
        rightLeaf->Keys[j] = tmpKey;
        j++;
    }

    if (endIndexInRightLeaf > exchangeNum) { // rightleaf more keys
        return movePointerAfter(rightLeaf, j, endIndexInRightLeaf, leftLeaf, i);
    } else if (endIndexInLeftLeaf > exchangeNum) {
        return movePointerAfter(leftLeaf, i, endIndexInLeftLeaf, rightLeaf, j);
    } else { // equal number of separate pointers in left/right sides
        AdjustAncestorKeysAccordingToCurNode(leftLeaf);
        AdjustAncestorKeysAccordingToCurNode(rightLeaf);
        return nullptr;
    }
}


void DAstatic::SwapForTailHead(NodeForStree ** inter1, NodeForStree ** inter2, int numLeaf1, int numLeaf2,
                               int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    swapWholeLeafAndGoUpwards(&inter1[1], &inter2[1], numLeaf1-2, numLeaf2-2);
    int numMidKeys = endIndexLeaf2+1 + startIndexLeaf2 - endIndexLeaf1 - 1 + inter1[0]->NumOfKeys - startIndexLeaf1;
    int * midKeys = new int[numMidKeys];
    int midIdx = 0;
    for (int i = 0; i <= endIndexLeaf2; ++i) {
        midKeys[midIdx] = inter2[numLeaf2-1]->Keys[i];
        midIdx++;
    }
    for (int i = endIndexLeaf1+1; i < startIndexLeaf2; ++i) {
        midKeys[midIdx] = inter2[0]->Keys[i];
        midIdx++;
    }
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; ++i) {
        midKeys[midIdx] = inter1[0]->Keys[i];
        midIdx++;
    }

    int numBegin2 = inter2[0]->NumOfKeys - startIndexLeaf2;
    int * keysBegin2 = new int[numBegin2];
    for (int i = 0; i < numBegin2; ++i) {
        keysBegin2[i] = inter2[0]->Keys[startIndexLeaf2+i];
    }

    int numEnd1 = endIndexLeaf1+1;
    int * keysEnd1 = new int[numEnd1];
    for (int i = 0; i < numEnd1; ++i) {
        keysEnd1[i] = inter1[numLeaf1-1]->Keys[i];
    }

    inter2[numLeaf2-2]->Next = inter1[1];
    //inter1[1]->Previous = inter2[numLeaf2-2];
    deleteEntry(inter2[0]->Parent, getIndexInParent(inter2[0]));
    inter2[0]->NumOfKeys = 0;

    //insert mid keys
    insertKeysIntoLeaf(midKeys, numMidKeys, inter2[numLeaf2-2], inter2[numLeaf2-2]->NumOfKeys);

    //keys at the beginning of range 2
    inter1[0]->NumOfKeys = startIndexLeaf1;
    insertKeysIntoLeaf(keysBegin2, numBegin2, inter1[0], inter1[0]->NumOfKeys);
    CheckAndMergeRedistributeNode(inter1[0]);

    //keys at the end of range 1
    replaceKeysInLeaf(inter2[numLeaf2-1], 0, endIndexLeaf2, keysEnd1, numEnd1);
    delete []midKeys;
    delete []keysBegin2;
    delete []keysEnd1;
}


void DAstatic::SwapWhenLenIs1(NodeForStree * LoneLeaf, NodeForStree **inter2, int numLeaf2, int startIndexLeaf1,
                              int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int numInRightPart = LoneLeaf->NumOfKeys - endIndexLeaf1 - 1;
    int numToInsertAfterFullLeaves = numInRightPart + endIndexLeaf2 + 1;
    int * keysToInsertAfterFullLeaves = new int[numToInsertAfterFullLeaves];// make([]int, NumInRightPart);
    for (int i = 0; i <= endIndexLeaf2; ++i) {
        keysToInsertAfterFullLeaves[i] = inter2[numLeaf2-1]->Keys[i];
    }
    for (int i = 0; i < numInRightPart; i++) {
        keysToInsertAfterFullLeaves[endIndexLeaf2+1+i] = LoneLeaf->Keys[endIndexLeaf1+1+i];
    }

    //keys to move in LoneLeaf
    int numToMoveInLoneLeaf = endIndexLeaf1 - startIndexLeaf1 + 1;
    int * keysToMoveInLoneLeaf = new int[numToMoveInLoneLeaf];
    for (int i = 0; i < numToMoveInLoneLeaf; ++i) {
        keysToMoveInLoneLeaf[i] = LoneLeaf->Keys[startIndexLeaf1+i];
    }
    //keys to move in the beginning of inter2[0]
    int numToMoveInBeginInter2 = inter2[0]->NumOfKeys - startIndexLeaf2;
    int * keysToMoveInBeginInter2 = new int[numToMoveInBeginInter2];
    for (int i = 0; i < numToMoveInBeginInter2; ++i) {
        keysToMoveInBeginInter2[i] = inter2[0]->Keys[startIndexLeaf2+i];
    }

    NodeForStree ** leafToInsert = &inter2[1];
    int numLeafToDelete = numLeaf2 - 2;
    //delete leaves
    NodeForStree * firstl = leafToInsert[0];
    NodeForStree * lastl = leafToInsert[numLeafToDelete-1];
    NodeForStree * pre = inter2[0];
    if (pre != nullptr) {
        pre->Next = lastl->Next;
    }
    /*
    if (firstl->Previous != nullptr) {
        firstl->Previous->Next = lastl->Next;
    }
    if (lastl->Next != nullptr) {
        lastl->Next->Previous = firstl->Previous;
    }
     */
    NodeForStree * preLeaf = deleteMulEntry(leafToInsert, numLeafToDelete);
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, leafToInsert, numLeafToDelete);

    //insert leaves
    NodeForStree * toParent = LoneLeaf->Parent;
    int startIndexInParent = getIndexInParent(LoneLeaf) + 1;
    insertLeavesIntoLeafParent(toParent, startIndexInParent, leafToInsert, numLeafToDelete);

    //insert keysToInsertAfterFullLeaves
    insertKeysIntoLeaf(keysToInsertAfterFullLeaves, numToInsertAfterFullLeaves,
                       inter2[numLeaf2-2], inter2[numLeaf2-2]->NumOfKeys);
    LoneLeaf->NumOfKeys = startIndexLeaf1;
    insertKeysIntoLeaf(keysToMoveInBeginInter2, numToMoveInBeginInter2, LoneLeaf, LoneLeaf->NumOfKeys);
    CheckAndMergeRedistributeNode(LoneLeaf);

    //replace keys in middle of inter2[0], inter2[numLeaf2-1]
    ReplaceMidOfTwoAdjacentLeaves(inter2[0], inter2[numLeaf2-1], startIndexLeaf2, endIndexLeaf2,
                                  keysToMoveInLoneLeaf, numToMoveInLoneLeaf);
    delete []keysToInsertAfterFullLeaves;
    delete []keysToMoveInBeginInter2;
    delete []keysToMoveInLoneLeaf;
}


void DAstatic::Move(int start_, int end_, int des_) {
    int start = start_, end = end_, des = des_;
    if (des > NumItems) {
        des = NumItems;
    }
    if (des >= start && des <= end+1) {
        return;
    }
    if (des > end) {
        int len1 = end - start + 1;
        int len2 = des - end - 1;
        if (len1 > len2) {
            start = end_ + 1;
            end = des_ - 1;
            des = start_;
        }
    } else { // des<start
        int len1 = end - start + 1;
        int len2 = start - des;
        if (len2 < len1) {
            start = des_;
            end = start_ - 1;
            des = end_ + 1;
        }
    }

    int toindex = -1;
    NodeForStree * toleaf = findLeaf(des, &toindex);
    if (des > NumItems) {
        toindex ++;
    }
    int startIndexLeaf1, endIndexLeaf1;
    NodeForStree ** inter1 = nullptr;
    int numLeaf1 = RangePosInLeafPointer(start, end, &inter1, &startIndexLeaf1, &endIndexLeaf1);
    RealMove(startIndexLeaf1, endIndexLeaf1, inter1, numLeaf1, toleaf, toindex);
}

void DAstatic::RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeForStree ** inter1, int numLeaf1, NodeForStree * toleaf, int toindex) {
    if (numLeaf1 == 1) {
        MoveWhenLenIs1(inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1, toleaf, toindex);
        return;
    } else if (numLeaf1 == 2) {
        MoveWhenLenIs2(inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1, toleaf, toindex);
        return;
    }else if (toleaf == inter1[0]) {
        MoveToLeafIsFirstLeaf(inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1, toleaf, toindex);
        return;
    } else if (toleaf == inter1[numLeaf1-1]) {
        MoveToLeafIsLastLeaf(inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1, toleaf, toindex);
        return;
    } else if (toleaf== inter1[numLeaf1-1]->Next) {
        if (endIndexLeaf1== inter1[numLeaf1-1]->NumOfKeys-1) {
            NodeForStree ** leaves = new NodeForStree*[1];
            leaves[0] = toleaf;
            SwapRangesAdjacent(inter1, leaves, numLeaf1, 1, startIndexLeaf1, endIndexLeaf1, 0, toindex-1);
            delete []leaves;
        } else {
            NodeForStree ** leaves = new NodeForStree*[2];
            leaves[0] = inter1[numLeaf1-1];
            leaves[1] = toleaf;
            swapHeadTailLen2Is2(inter1, leaves, numLeaf1, 2, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toindex-1);
            delete []leaves;
        }
    } else if (toleaf->Next == inter1[0]) {
        if (startIndexLeaf1==0) {
            NodeForStree ** leaves = new NodeForStree*[1];
            leaves[0] = toleaf;
            SwapRangesAdjacent(leaves, inter1, 1, numLeaf1, toindex, toleaf->NumOfKeys-1, startIndexLeaf1, endIndexLeaf1);
            delete []leaves;
        } else {
            NodeForStree ** leaves = new NodeForStree*[2];
            leaves[0] = toleaf;
            leaves[1] = inter1[0];
            swapHeadTailLen1Is2(leaves, inter1, 2, numLeaf1, toindex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
            delete []leaves;
        }
    }else {
        Move1(inter1 , numLeaf1 , startIndexLeaf1, endIndexLeaf1, toleaf, toindex);
        return;
    }
}

void DAstatic::MoveToLeafIsFirstLeaf(NodeForStree ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree * toleaf, int toindex) {
    if (numLeaf1 == 1) {
        swapHeadTail11(toleaf, toindex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        return;
    }else if (numLeaf1==2) {
        swapHeadTail12(inter1[0], inter1[1], toindex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        return;
    } else {
        swapHeadTailLen1Is1(inter1, numLeaf1, toindex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        return;
    }
}

void DAstatic::MoveToLeafIsLastLeaf(NodeForStree ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree * toleaf, int toindex) {
    if (numLeaf1==1) {
        swapHeadTail11(toleaf, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toindex-1);
        return;
    } else if (numLeaf1==2) {
        swapHeadTail21(inter1[0], inter1[1], startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toindex-1);
        return;
    } else {
        swapHeadTailLen2Is1(inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toindex-1);
        return;
    }
}

void DAstatic::Move1(NodeForStree ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree * toleaf, int toindex) {
    //NodeForStree ** leafToMove = new NodeForStree* [numLeaf1-2];
    int NumLeafToMove = numLeaf1-2;
    NodeForStree ** leafToMove = &inter1[1]; // only move whole leaves
    NodeForStree * fl = inter1[0];
    if (fl != nullptr) {
        fl->Next = leafToMove[NumLeafToMove-1]->Next;
        /*
        if (fl->Next != nullptr) {
            fl->Next->Previous = fl;
        }
         */
    }
    NodeForStree * preLeaf = deleteMulEntry(leafToMove, NumLeafToMove);
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, leafToMove, NumLeafToMove);

    int startIndex = getIndexInParent(toleaf)+1;
    NodeForStree * toParent = toleaf->Parent;
    insertLeavesIntoLeafParent(toParent, startIndex, leafToMove, NumLeafToMove);

    int numKeysBefore = inter1[0]->NumOfKeys - startIndexLeaf1;
    int * KeysBefore = new int[numKeysBefore];
    for (int i = 0; i < numKeysBefore; i ++) {
        KeysBefore[i] = inter1[0]->Keys[startIndexLeaf1+i];
    }

    int numKeysAfter = endIndexLeaf1+1 + toleaf->NumOfKeys - toindex;
    int * KeysAfter = new int[numKeysAfter];// make([]int, numKeysAfter);
    int afterIdx = 0;
    for (int i = 0; i < endIndexLeaf1+1; i ++) {
        KeysAfter[i] = inter1[numLeaf1-1]->Keys[i];
        afterIdx++;
    }
    for (int j = toindex; j < toleaf->NumOfKeys; ++j) {
        KeysAfter[afterIdx] = toleaf->Keys[j];
        afterIdx++;
    }

    insertKeysIntoLeaf(KeysAfter, numKeysAfter, inter1[numLeaf1-2], inter1[numLeaf1-2]->NumOfKeys);
    insertKeysIntoLeaf(KeysBefore, numKeysBefore, inter1[1], 0);
    toleaf->NumOfKeys = toindex;
    CompleteAdjustAncestorNodesAfterDelete(toleaf);
    DeleteMidPartOfTwoAdjacentLeaves(inter1[0], startIndexLeaf1, inter1[numLeaf1-1], endIndexLeaf1);
    delete []KeysAfter;
    delete []KeysBefore;
}


void DAstatic::MoveWhenLenIs1(NodeForStree ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree * toleaf, int toIndex) {
    NodeForStree * fromLeaf = inter1[0];
    if (fromLeaf->Next == toleaf) {
        if (endIndexLeaf1 < fromLeaf->NumOfKeys-1) {
            swapHeadTail12(fromLeaf, toleaf, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toIndex-1);
        } else {
            SwapRangesAdjacentWhenLensAreBoth1(fromLeaf, toleaf, startIndexLeaf1, endIndexLeaf1, 0, toIndex-1);
        }
        return;
    } else if (toleaf->Next == fromLeaf) {
        if (startIndexLeaf1 > 0) {
            swapHeadTail21(toleaf, fromLeaf, toIndex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        } else {
            SwapRangesAdjacentWhenLensAreBoth1(toleaf, fromLeaf, toIndex, toleaf->NumOfKeys-1, 0, endIndexLeaf1);
        }
        return;
    }

    if (fromLeaf != toleaf) {
        int NumKeys = endIndexLeaf1-startIndexLeaf1+1;
        int * Keys = new int[NumKeys];// make([]int, NumKeys)
        for (int i = startIndexLeaf1; i <= endIndexLeaf1; i ++) {
            Keys[i-startIndexLeaf1] = fromLeaf->Keys[i];
        }
        insertKeysIntoLeaf(Keys, NumKeys, toleaf, toIndex);
        movePointerInNodeForward(fromLeaf, endIndexLeaf1+1, fromLeaf->NumOfKeys-1, startIndexLeaf1);
        fromLeaf->NumOfKeys -= NumKeys;
        CompleteAdjustAncestorNodesAfterDelete(fromLeaf);
        delete []Keys;
        return;
    } else {
        if (toIndex > endIndexLeaf1) {
            swapHeadTail11(toleaf, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toIndex-1);
        } else {
            swapHeadTail11(toleaf, toIndex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        }
    }
}


void DAstatic::MoveWhenLenIs2(NodeForStree ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeForStree * toleaf, int toIndex) {
    NodeForStree * fromLeaf1 = inter1[0];
    NodeForStree * fromLeaf2 = inter1[1];
    if (toleaf->Next == fromLeaf1) {
        if (startIndexLeaf1 > 0) {
            swapHeadTail22(toleaf, fromLeaf1, fromLeaf2, toIndex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        } else {
            SwapRangesAdjacentWhenLensAre1And2(toleaf, fromLeaf1, fromLeaf2, toIndex, toleaf->NumOfKeys-1, startIndexLeaf1, endIndexLeaf1);
        }
        return;
    } else if (fromLeaf2->Next ==toleaf) {
        if (endIndexLeaf1< fromLeaf2->NumOfKeys-1) {
            swapHeadTail22(fromLeaf1, fromLeaf2, toleaf, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toIndex-1);
        } else {
            SwapRangesAdjacentWhenLensAre2And1(fromLeaf1, fromLeaf2, toleaf, startIndexLeaf1, fromLeaf2->NumOfKeys-1, 0, toIndex-1);
        }
        return;
    }

    if (fromLeaf1 != toleaf  && fromLeaf2 != toleaf) {
        int NumKeys = endIndexLeaf1 + 1 + fromLeaf1->NumOfKeys - startIndexLeaf1;
        int * Keys = new int[NumKeys];// make([]int, NumKeys)
        int ik = 0;
        for (int i = startIndexLeaf1; i < fromLeaf1->NumOfKeys; i ++) {
            Keys[ik] = fromLeaf1->Keys[i];
            ik++;
        }
        for (int i = 0; i <= endIndexLeaf1; i ++) {
            Keys[ik] = fromLeaf2->Keys[i];
            ik++;
        }
        insertKeysIntoLeaf(Keys, NumKeys, toleaf, toIndex);
        DeleteMidPartOfTwoAdjacentLeaves(fromLeaf1, startIndexLeaf1, fromLeaf2, endIndexLeaf1);
        delete []Keys;
        return;
    } else {
        if (toleaf == fromLeaf1) {
            swapHeadTail12(fromLeaf1, fromLeaf2, toIndex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        } else {
            swapHeadTail21(fromLeaf1, fromLeaf2, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toIndex-1);
        }
    }
}

