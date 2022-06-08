#include <vector>
#include "DynamicArray.h"

NodeDA::NodeDA(int capacity, int numkeys, NodeDA * parent, bool isleaf) {
    NumOfKeys = numkeys;
    Parent = parent;
    Pointers = new NodeDA * [capacity];
    Keys = new int[capacity];
    Next = nullptr;
    IsLeaf = isleaf;
}


NodeDA::NodeDA(int capacity, NodeDA * parent, std::string str, bool isleaf) {
    Pointers = new NodeDA * [capacity];
    Keys = new int[capacity];
    Next = nullptr;
    IsLeaf = isleaf;
    Parent = parent;
    int i = 0, j = str.find(' '), k = 0;
    while (j != std::string::npos) {
        Keys[k] = std::stoi(str.substr(i, j-i));
        if (Keys[k] == 0) {
            NumOfKeys = k;
            return;
        }
        i = j + 1;
        j = str.find(' ', i);
        k ++;
    }
    if (k < capacity) { // check whether the last key is 0
        int last_key = std::stoi(str.substr(i, str.length()-i));
        if (last_key == 0) {
            NumOfKeys = k;
            return;
        }
        Keys[k] = last_key;
        NumOfKeys = k+1;
    }
}

NodeDA::~NodeDA() {
    delete []Pointers;
    delete []Keys;
}

int NodeDA::SumOfKeys() const {
    int ans = 0;
    for (int i = 0; i < NumOfKeys; ++i) {
        ans += Keys[i];
    }
    return ans;
}

DynamicArray::DynamicArray() {
    Root = nullptr;
    NumItems = 0;
    Capacity = 4;
    Min = 2;
}


DynamicArray::DynamicArray(int * & array, int numItems, int capacity) {
    if (array == nullptr) {
        Root = nullptr;
        Capacity = capacity;
        numItems = 0;
        Min = ceil(Capacity * 1 / 2);
        return;
    }
    Root = new NodeDA(capacity, 1, nullptr, true);
    NumItems = numItems;
    Capacity = capacity;
    Min = ceil(Capacity * 1.0 / 2.0);
    int *newa = new int[numItems];
    for (int i = 0; i < numItems; ++i) {
        newa[i] = array[i];
    }
    Root->Keys[0] = numItems;
    Root->Pointers[0] = reinterpret_cast<NodeDA *>(newa);
}

/*
 * Initialize a new ADA, given array, num of items, and fanout (degree)
 * Has exactly the same functionality as DynamicArray::DynamicArray
 */
DynamicArray * NewDynamicArray(int * & array, int numItems, int degree ){
    auto * da =new DynamicArray(array, numItems, degree);
    return da;
}

/*
 * unsolved problem: how to free space of arrays manually
 * because
 * 1. some part of the array is lost track of during array splitting
 * 2. Eg, int * a = new int[10], but the key corresponding to a was changed to 4 afterwards
 * in this case, delete []a will delete 10 int spaces rather than 4
 * which can make anther array deletion run into error
 */
DynamicArray::~DynamicArray() {
    std::list<NodeDA *> toDelete;
    std::list<NodeDA *> toFindChild;
    toFindChild.push_back(Root);
    while (!toFindChild.empty()) {
        NodeDA * ele = toFindChild.front();
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
        NodeDA * ele = toDelete.front();
        toDelete.pop_front();
        delete ele;
    }
}



////return length of ans, ans is answer array
// repeat point query
//int* DynamicArray::RangeQuery(int start, int end, int * lenOfAns) {
//    int dastart = 1;
//    int daend = NumItems;
//    int realstart = start;
//    int realend = end;
//    if (realstart < dastart) {
//        realstart = dastart;
//    }
//    if (realend > daend) {
//        realend = daend;
//    }
//    int realnum = realend - realstart + 1;
//    *lenOfAns = realnum;
//    if (realnum <= 0) {
//        return nullptr;
//    }
//    int *ans = new int[realnum];
//    NodeDA * c = Root;
//    int p = realstart;
//    while (!c->IsLeaf) {
//        for (int i = 0; i < c->NumOfKeys; ++i) {
//            if (p <= c->Keys[i]) {
//                c = c->Pointers[i];
//                break;
//            }
//            p -= c->Keys[i];
//        }
//    }
//    int i = 0;
//    for (; i < c->NumOfKeys; ++i) {
//        if (p <= c->Keys[i]) {
//            break;
//        }
//        p -= c->Keys[i];
//    }
//    p--;
//    int k = 0;
//    bool flag = false;
//    while (k < realnum) {
//        for (; i < c->NumOfKeys; ++i) {
//            int * curarray = reinterpret_cast<int *>(c->Pointers[i]);
//            for ( ; p < c->Keys[i]; ++p) {
//                ans[k] = curarray[p];
//                ++k;
//                if (k == realnum) {
//                    flag = true;
//                    break;
//                }
//            }
//            if (flag) {
//                break;
//            }
//            p = 0;
//        }
//        if (flag) {
//            break;
//        }
//        if (c->Next == nullptr) {
//            printf("error\n");
//        }
//        c = c->Next;
//        i = 0;
//    }
//    return ans;
//}


// use c++ copy function to accelerate
int* DynamicArray::RangeQuery(int start, int end, int * lenOfAns) {
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
    NodeDA * c = Root;
    int p = realstart;
    while (!c->IsLeaf) {
        for (int i = 0; i < c->NumOfKeys; ++i) {
            if (p <= c->Keys[i]) {
                c = c->Pointers[i];
                break;
            }
            p -= c->Keys[i];
        }
    }
    int i = 0;
    for (; i < c->NumOfKeys; ++i) {
        if (p <= c->Keys[i]) {
            break;
        }
        p -= c->Keys[i];
    }
    p--;
    int k = 0;
    bool flag = false;
    while (k < realnum) {
        for (; i < c->NumOfKeys; ++i) {
            int * curarray = reinterpret_cast<int *>(c->Pointers[i]);
            if (k + c->Keys[i] - p <= realnum) {
                std::copy(curarray + p, curarray + c->Keys[i], ans + k);
                k += c->Keys[i] - p;
            } else {
                int len = realnum - k;
                std::copy(curarray + p, curarray + p + len, ans + k);
                flag = true;
                break;
                k += len;
            }
            if (flag) {
                break;
            }
            p = 0;
        }
        if (flag) {
            break;
        }
        if (c->Next == nullptr) {
            printf("error\n");
        }
        c = c->Next;
        i = 0;
    }
    return ans;
}


int DynamicArray::Query(int pos) const {
    if (pos > NumItems) {
        pos = NumItems;
    }
    NodeDA * c = Root;
    int p = pos;
    while (!c->IsLeaf) {
        for (int i = 0; i < c->NumOfKeys; ++i) {
            if (p <= c->Keys[i]) {
                c = c->Pointers[i];
                break;
            }
            p -= c->Keys[i];
        }
    }
    int i = 0;
    for (; i < c->NumOfKeys; ++i) {
        if (p <= c->Keys[i]) {
            break;
        }
        p -= c->Keys[i];
    }
    p--;
    int * curarray = reinterpret_cast<int *>(c->Pointers[i]);
    return curarray[p];
}



//make a leaf with capacity
NodeDA * makeLeafonedp(int capacity) {
        auto * n = new NodeDA(capacity);
    n->IsLeaf = true;
    return n;
}


void DynamicArray::Insert(int newID, int pos) {
    NumItems ++;
    if (pos > NumItems + 1) {
        pos = NumItems + 1;
    }
    if (Root == nullptr) {
        Root = makeLeafonedp(Capacity);
        Root->Keys[0] = 1;
        int * newa = new int[Capacity];
        newa[0] = newID;
        Root->Pointers[0] = reinterpret_cast<NodeDA *>(newa);
        Root->Parent = nullptr;
        Root->Next = nullptr;
        // Root->Previous = nullptr;
        Root->NumOfKeys = 1;
        return;
    }
    NodeDA * leaf;
    int preIndexInLeaf, preIndexInArray;
    leaf = findLeaf(pos-1, &preIndexInLeaf, &preIndexInArray);
    if (preIndexInLeaf == -1) { // insert into the starting position
        int * olda = reinterpret_cast<int *>(leaf->Pointers[0]);
        int tmp = olda[0];
        olda[0] = newID;
        newID = tmp;
        preIndexInArray = 0;
        preIndexInLeaf = 0;
    }
    int oldL = leaf->Keys[preIndexInLeaf];
    int newL = leaf->Keys[preIndexInLeaf] + 1;
    int * olda = reinterpret_cast<int *>(leaf->Pointers[preIndexInLeaf]);
    if (oldL < Capacity) { // have empty slots
        for (int i = oldL; i > preIndexInArray ; --i) {
            olda[i] = olda[i-1];
        }
        olda[preIndexInArray + 1] = newID;
        leaf->Keys[preIndexInLeaf]++;
        SimplyAdjustAncestorKeysBy(leaf, 1);
        return;
    }
    int rightlen = newL - preIndexInArray - 1 - Min;
    int leftlen = preIndexInArray + 1;
    if (rightlen >= Capacity && leftlen >= Capacity) { //split and add, result in three arrays
        int midlen = Min;
        int *mida = new int[Capacity];
        mida[0] = newID;
        for (int i = 1; i < Min; ++i) {
            mida[i] = olda[i + preIndexInArray];
        }
        int rightstartidx = newL - rightlen - 1;
        int *righta = &olda[rightstartidx];
        leaf->Pointers[preIndexInLeaf] = reinterpret_cast<NodeDA *>(olda);
        leaf->Keys[preIndexInLeaf] = leftlen;
        int a[2] = {midlen, rightlen};
        int *b[2] = {mida, righta};
        insertKeysIntoLeaf(a, reinterpret_cast<NodeDA **>(b), 2, leaf, preIndexInLeaf + 1);
        return;
    }
    // only add one array
    if (leftlen <= newL / 2) { //add to the left side
        int * lefta = new int[Capacity];
        if (leftlen < Min) { // newID is in the newly created subarray
            int i = 0;
            for ( ; i < leftlen; i++) {
                lefta[i] = olda[i];
            }
            lefta[i] = newID;
            i++;
            for ( ; i < Min; i++) {
                lefta[i] = olda[i-1];
            }
            rightlen = newL - Min;
            int * righta;
            if (rightlen < Capacity) { // need to shift
                int shift = Capacity - rightlen;
                int rightstartidx = Min - 1;
                for (int j = rightstartidx; j < oldL; ++j) {
                    olda[j-shift] = olda[j];
                }
                righta = &olda[rightstartidx - shift];
            } else { // no need of shifting
                righta = &olda[Min - 1];
            }
            leaf->Pointers[preIndexInLeaf] = reinterpret_cast<NodeDA *>(lefta);
            leaf->Keys[preIndexInLeaf] = Min;
            if (leaf->NumOfKeys+1 <= Capacity) {
                insertOneIntoLeaf(leaf, righta, rightlen, preIndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightlen, preIndexInLeaf+1);
            }
            return;
        } else { // newID is in the original subarray
            int i = 0;
            for ( ; i < Min; ++i) {
                lefta[i] = olda[i];
            }
            rightlen = newL - Min;
            int * righta;
            if (rightlen >= Capacity) { // need to shift
                for (int j = Min - 1; j < preIndexInArray; ++j) {
                    olda[j] = olda[j+1];
                }
                olda[preIndexInArray] = newID;
                int rightstartidx = Min - 1;
                righta = &olda[rightstartidx];
            } else { // no need of shifting
                int shift = Capacity - rightlen;
                int k = oldL - Capacity;
                int j = Min;
                for ( ; j < preIndexInArray; ++j) {
                    olda[k] = olda[j];
                    k++;
                }
                olda[k] = newID;
                k++;
                for ( ; j < oldL; ++j) {
                    olda[k] = olda[j];
                    k++;
                }
                int rightstartidx = oldL - Capacity;
                righta = &olda[rightstartidx];
            }
            leaf->Pointers[preIndexInLeaf] = reinterpret_cast<NodeDA *>(lefta);
            leaf->Keys[preIndexInLeaf] = Min;
            if (leaf->NumOfKeys+1 <= Capacity) {
                insertOneIntoLeaf(leaf, righta, rightlen, preIndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightlen, preIndexInLeaf+1);
            }
            return;
        }
    } else { // add to the right side
        rightlen = newL - (preIndexInArray + 1);
        if (rightlen <= Min) { // newID is in the newly created subarray
            int * righta = new int[Capacity];
            leftlen = newL - Min;
            int j = leftlen, i = 0;
            for ( ; j <= preIndexInArray; ++j) {
                righta[i] = olda[j];
                i++;
            }
            righta[i] = newID;
            i++;
            for ( ; i < Min; ++i) {
                righta[i] = olda[j];
                j++;
            }
            leaf->Pointers[preIndexInLeaf] = reinterpret_cast<NodeDA *>(olda);
            leaf->Keys[preIndexInLeaf] = leftlen;
            rightlen = Min;
            if (leaf->NumOfKeys+1 <= Capacity) {
                insertOneIntoLeaf(leaf, righta, rightlen, preIndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightlen, preIndexInLeaf+1);
            }
            return;
        } else { // newID is in the original subarray
            int * righta = new int[Capacity];
            leftlen = newL - Min;
            int j = oldL - 1;
            for (int i = Min - 1; i >= 0; --i) {
                righta[i] = olda[j];
                j--;
            }
            j++;
            for ( ; j >= preIndexInArray + 2; --j) {
                olda[j] = olda[j-1];
            }
            olda[preIndexInArray+1] = newID;
            leaf->Pointers[preIndexInLeaf] = reinterpret_cast<NodeDA *>(olda);
            leaf->Keys[preIndexInLeaf] = leftlen;
            rightlen = Min;
            if (leaf->NumOfKeys+1 <= Capacity) {
                insertOneIntoLeaf(leaf, righta, rightlen, preIndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightlen, preIndexInLeaf+1);
            }
            return;
        }
    }
}


void DynamicArray::PrintTree() const {
    printf("\n");
    std::list<NodeDA *> lst;
    int i = 0, rank = 0, newRank = 0;
    if (Root == nullptr) {
        printf("Empty Tree\n");
        return;
    }
    lst.push_back(Root);
    while (!lst.empty()) {
        NodeDA * ele = lst.front();
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
                        NodeDA * c = ele->Pointers[i];
                        lst.push_back(c);
                    }
                }
            }
            printf(" | ");
        }
    }
    printf("\n");
}

void DynamicArray::PrintDA() {
    PrintTree();
    int preIndexInLeaf, preIndexInArray;
    NodeDA * leaf = findLeaf(1, &preIndexInLeaf, &preIndexInArray);
    while (leaf != nullptr) {
        int NumKeysInLeaf = leaf->NumOfKeys;
        for (int i = 0; i < NumKeysInLeaf; ++i) {
            int len = leaf->Keys[i];
            printf("(");
            int * s = reinterpret_cast<int *>(leaf->Pointers[i]);
            for (int j = 0; j < len; ++j) {
                printf("%d ", s[j]);
            }
            printf(")\n");
        }
        leaf = leaf->Next;
    }
    printf("\n");
}

void DynamicArray::PrintArray() {
    int num;
    int * ans = RangeQuery(1, NumItems, &num);
    printf("[");
    for (int i = 0; i < num; ++i) {
        printf("%d ", ans[i]);
    }
    printf("]\n");
}

int DynamicArray::pathToRoot(NodeDA * child) const {
    int length = 0;
    NodeDA * c = child;
    while (c != Root) {
        c = c->Parent;
        length ++;
    }
    return length;
}

//depth of tree
int DynamicArray::Depth() const {
    if (Root == nullptr) {
        return 0;
    }
    int ans = 1;
    NodeDA * c = Root;
    while (!c->IsLeaf) {
        ans ++;
        c = c->Pointers[0];
    }
    return ans;
}

int DynamicArray::GetFanout() const {
    return Capacity;
}

int DynamicArray::GetNumItems() const {
    return NumItems;
}

void DynamicArray::insertMulKeysIntoLeaf(const int * keysToInsert, NodeDA ** pointersToInsert,
                                         int numKeysToInsert, NodeDA * toLeaf, int toIndex) {
    int newNumKeys = numKeysToInsert + toLeaf->NumOfKeys;
    int startInsertedIdx = toIndex, endInsertedIdx = startInsertedIdx + numKeysToInsert - 1;
    for (int l = newNumKeys - 1; l > endInsertedIdx; --l) {
        toLeaf->Pointers[l] = toLeaf->Pointers[l-numKeysToInsert];
        toLeaf->Keys[l] = toLeaf->Keys[l-numKeysToInsert];
    }
    for (int i = startInsertedIdx; i <= endInsertedIdx; ++i) {
        toLeaf->Pointers[i] = pointersToInsert[i - startInsertedIdx];
        toLeaf->Keys[i] = keysToInsert[i-startInsertedIdx];
    }
    toLeaf->NumOfKeys += numKeysToInsert;
    AdjustAncestorKeysAccordingToCurNode(toLeaf);
}

//only split once
//note that in some cases, we inserting into beginning/end of a node, mostly happening in swap/move
//when insert into the beginning, we need to make pointer toLeaf still point to the second leaf
//return true if toIndex == 0
bool DynamicArray::insertMulKeysIntoLeafAfterOneSplitting(const int * keysToInsert, NodeDA ** pointersToInsert,
                                                          int numKeysToInsert, NodeDA * toLeaf, int toIndex) {
    int numGoRight = toLeaf->NumOfKeys - toIndex;
    int * keysGoRight = new int[numGoRight];
    auto ** pointersGoRight = new NodeDA *[numGoRight];
    for (int i = 0; i < numGoRight; ++i) {
        pointersGoRight[i] = toLeaf->Pointers[i + toIndex];
        keysGoRight[i] = toLeaf->Keys[i+toIndex];
    }
    int totalNum = toLeaf->NumOfKeys + numKeysToInsert;
    int leftNum = totalNum/2, rightNum = totalNum - leftNum;
    int rightSumKeys = 0;
    int toInsertIdx = 0, goRightIdx = 0;
    int leftidx = toIndex, rightidx = 0;
    for (; leftidx < leftNum; ++leftidx) {
        toLeaf->Pointers[leftidx] = pointersToInsert[toInsertIdx];
        toLeaf->Keys[leftidx] = keysToInsert[toInsertIdx];
        toInsertIdx++;
        if (toInsertIdx == numKeysToInsert) {
            leftidx++;
            break;
        }
    }
    for (; leftidx < leftNum; ++leftidx) {
        toLeaf->Pointers[leftidx] = pointersGoRight[goRightIdx];
        toLeaf->Keys[leftidx] = keysGoRight[goRightIdx];
        goRightIdx++;
    }
    NodeDA * newLeaf = makeLeafonedp(Capacity);
    for (int k = leftNum; k < toIndex; ++k) {
        newLeaf->Pointers[rightidx] = toLeaf->Pointers[k];
        newLeaf->Keys[rightidx] = toLeaf->Keys[k];
        rightSumKeys += newLeaf->Keys[rightidx];
        rightidx++;
    }
    for (; toInsertIdx < numKeysToInsert; ++toInsertIdx) {
        newLeaf->Pointers[rightidx] = pointersToInsert[toInsertIdx];
        newLeaf->Keys[rightidx] = keysToInsert[toInsertIdx];
        rightSumKeys += newLeaf->Keys[rightidx];
        rightidx++;
    }
    for (; goRightIdx < numGoRight; ++goRightIdx) {
        newLeaf->Pointers[rightidx] = pointersGoRight[goRightIdx];
        newLeaf->Keys[rightidx] = keysGoRight[goRightIdx];
        rightSumKeys += newLeaf->Keys[rightidx];
        rightidx++;
    }
    delete []keysGoRight;
    delete []pointersGoRight;
    toLeaf->NumOfKeys = leftNum;
    newLeaf->NumOfKeys = rightNum;
    newLeaf->Parent = toLeaf->Parent;
    newLeaf->Next = toLeaf->Next;
    toLeaf->Next = newLeaf;
    //newLeaf->Previous = toLeaf;
    int leftSumKeys = NodeSumKeys(toLeaf);
    insertIntoParent(toLeaf, leftSumKeys, newLeaf, rightSumKeys);
    if (toIndex == 0) {
        return true;
    }
    return false;
}

void DynamicArray::insertOneIntoLeaf(NodeDA * leaf, int * newPointer, int length, int indexInLeaf) {
    int * pointerToStore = newPointer;
    int keyToStore = length;
    leaf->NumOfKeys ++;
    int tmpKey;
    int i = indexInLeaf;
    for (; i < leaf->NumOfKeys - 1; i++) {
        tmpKey = leaf->Keys[i];
        int * tmpPointer = reinterpret_cast<int *>(leaf->Pointers[i]);
        leaf->Keys[i] = keyToStore;
        leaf->Pointers[i] = reinterpret_cast<NodeDA *>(pointerToStore);
        keyToStore = tmpKey;
        pointerToStore = tmpPointer;
    }
    leaf->Keys[i] = keyToStore;
    leaf->Pointers[i] = reinterpret_cast<NodeDA *>(pointerToStore);

    if (leaf == Root) {
        return;
    }
    NodeDA * c = leaf;
    NodeDA * parent = c->Parent;
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


void DynamicArray::insertOneIntoLeafAfterSplitting(NodeDA * leaf, int * newPointer, int length, int indexInLeaf, bool needGoupwards) {
    NodeDA * newLeaf;
    int split=0, i=0, LeafNumPointers=0, NewLeafNumPointers=0;
    newLeaf = makeLeafonedp(Capacity);
    split = Min;
    leaf->NumOfKeys = split;
    newLeaf->NumOfKeys = Capacity + 1 - split;
    if (indexInLeaf < split) {// new array is in leaf (old leaf)
        int j = Capacity - 1;
        for (i = newLeaf->NumOfKeys - 1; i >= 0 ; --i) {
            newLeaf->Keys[i] = leaf->Keys[j];
            newLeaf->Pointers[i] = leaf->Pointers[j];
            j--;
            NewLeafNumPointers += newLeaf->Keys[i];
        }
        for (i = split - 1; i > indexInLeaf ; --i) {
            leaf->Pointers[i] = leaf->Pointers[i-1];
            leaf->Keys[i] = leaf->Keys[i-1];
            LeafNumPointers += leaf->Keys[i];
        }
        LeafNumPointers += length;
        leaf->Keys[i] = length;
        leaf->Pointers[i] = reinterpret_cast<NodeDA *>(newPointer);
        i--;
        for (; i >= 0; --i) {
            LeafNumPointers += leaf->Keys[i];
        }
    } else {// new array is in new_leaf
        indexInLeaf -= split;
        for (i = 0; i < newLeaf->NumOfKeys ; ++i) {
            if (i == indexInLeaf) {
                NewLeafNumPointers += length;
                newLeaf->Keys[i] = length;
                newLeaf->Pointers[i] = reinterpret_cast<NodeDA *>(newPointer);
                break;
            } else {
                newLeaf->Keys[i] = leaf->Keys[i + split];
                newLeaf->Pointers[i] = leaf->Pointers[i+split];
                NewLeafNumPointers += newLeaf->Keys[i];
            }
        }
        i++;
        for (; i < newLeaf->NumOfKeys; i++) {
            newLeaf->Keys[i] = leaf->Keys[i-1+split];
            newLeaf->Pointers[i] = leaf->Pointers[i-1+split];
            NewLeafNumPointers += newLeaf->Keys[i];
        }
        for (int y = 0; y < leaf->NumOfKeys; ++y) {
            LeafNumPointers += leaf->Keys[y];
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
    insertIntoParent(leaf, LeafNumPointers, newLeaf, NewLeafNumPointers, needGoupwards);
}

//insert right after left
void DynamicArray::insertIntoParent(NodeDA * left, int sumKeysLeft, NodeDA * right, int sumKeysRight, bool needGoupwards) {
    NodeDA * parent = left->Parent;
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
void DynamicArray::insertIntoNode(NodeDA * n, int leftIndex, int sumKeysRight, NodeDA * right, bool needGoUpward) {
    n->NumOfKeys ++;
    int i = 0;
    int keyToStore = sumKeysRight;
    NodeDA * pointerToStore = right;
    right->Parent = n;
    i = leftIndex + 1;
    for (; i < n->NumOfKeys - 1; i++) {
        int tmpKey = n->Keys[i];
        NodeDA * tmpPointer = n->Pointers[i];
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
    NodeDA * c = n;
    NodeDA * parent = c->Parent;
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

void DynamicArray::insertIntoNodeAfterSplitting(NodeDA * parent, int leftIndex, int sumKeysRight, NodeDA * right, bool needGoUpwards) {
    int i, j, leftSumKeys = 0, rightSumKeys = 0;
    NodeDA * newNode;
    int * tempKeys;
    NodeDA ** tempPointers;
    tempPointers = new NodeDA *[Capacity + 1];
    tempKeys = new int[Capacity+1];
    j = 0;
    for (i = 0; i < parent->NumOfKeys; ++i) {
        if (j == leftIndex + 1) {
            j ++;
        }
        tempPointers[j] = parent->Pointers[i];
        tempKeys[j] = parent->Keys[i];
        //parent->Keys[i] = 0;
        //parent->Pointers[i] = nullptr;
        j++;
    }
    tempPointers[leftIndex + 1] =right;
    tempKeys[leftIndex + 1] = sumKeysRight;
    newNode = new NodeDA(Capacity);
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
    delete []tempKeys;
    delete []tempPointers;
    newNode->NumOfKeys = j;
    newNode->Parent = parent->Parent;
    insertIntoParent(parent, leftSumKeys, newNode, rightSumKeys, needGoUpwards);
}

int DynamicArray::NodeSumKeys(NodeDA * n) {
    int ans = 0;
    for (int i = 0; i < n->NumOfKeys; ++i) {
        ans += n->Keys[i];
    }
    return ans;
}

int DynamicArray::getLeftIndex(NodeDA * parent, NodeDA * left) const {
    int leftIndex = 0;
    while(leftIndex < Capacity && parent->Pointers[leftIndex] != left) {
        leftIndex += 1;
    }
    return leftIndex;
}

void DynamicArray::insertIntoNewRoot(NodeDA * left, int sumKeysLeft, NodeDA * right, int sumKeysRight) {
    Root = new NodeDA(Capacity);
    Root->Keys[0] = sumKeysLeft;
    Root->Pointers[0] = left;
    Root->Keys[1] = sumKeysRight;
    Root->Pointers[1] = right;
    Root->Parent = nullptr;
    left->Parent = Root;
    right->Parent = Root;
    Root->NumOfKeys = 2;
}

/*
 * Used to locate the pos-th item.
 * Return the pointer to the leaf containing the pos-th item.
 * int *preIndexInLeaf, int *preIndexInArray serve as return values
 * preIndexInLeaf is the index of the array containing current pos-th item in the leaf
 * preIndexInArray is the index of the current pos-th item in the array
 * if pos = 0, preIndexInArray=preIndexInLeaf=-1, return the leaf containing the first item
 */
NodeDA * DynamicArray::findLeaf(int pos, int * preIndexInLeaf, int * preIndexInArray) {
    if (pos > NumItems) {
        pos = NumItems;
    }
    bool flag = false;
    NodeDA * c = Root;
    int p = pos;
    if (pos == 0) {
        p = 1;
        flag = true;
    }
    int i;
    if (c == nullptr) {
        return c;
    }
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
        * preIndexInArray = -1;
        * preIndexInLeaf = -1;
        return c;
    }
    for (i = 0; i < c->NumOfKeys; i ++ ) {
        if (p <= c->Keys[i]) {
            break;
        }
        p -= c->Keys[i];
    }
    *preIndexInLeaf = i;
    *preIndexInArray = p-1;
    return c;
}



// return  the deleted rid
int DynamicArray::Delete(int pos) {
    if (pos < 1) {
        pos = 1;
    }
    if (pos > NumItems) {
        pos = NumItems;
    }
    int IndexInLeaf, IndexInArray;
    NodeDA * leaf = findLeaf(pos, &IndexInLeaf, &IndexInArray);
    int * array = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
    int IDdeleted = array[IndexInArray];
    int oldlen = leaf->Keys[IndexInLeaf];
    if (oldlen == Min) { //merge
        MergeOrRedistributeArrayAfterDelete(leaf, IndexInLeaf, IndexInArray);
        NumItems--;
        return IDdeleted;
    } else if (oldlen < 2 * Capacity) { //must one array
        int * olda = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        for (int i = IndexInArray; i < oldlen - 1; i ++) {
            olda[i] = olda[i+1];
        }
        leaf->Keys[IndexInLeaf]--;
        SimplyAdjustAncestorKeysBy(leaf, -1);
        NumItems--;
        return IDdeleted;
    } else {
        int leftlen = IndexInArray;
        int rightlen = oldlen -1 -leftlen;
        if (leftlen >= Capacity && rightlen >= Capacity) {
            int * olda = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            int * newa = &olda[IndexInArray+1];
            leaf->Keys[IndexInLeaf] = leftlen;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(olda);
            if (leaf->NumOfKeys + 1 <= Capacity) {
                insertOneIntoLeafForDeletion(leaf, newa, rightlen, IndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, newa, rightlen, IndexInLeaf+1);
            }
            NumItems--;
            return IDdeleted;
        } else if (leftlen < Capacity) {
            int * olda = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            for ( int i = IndexInArray; i >= 1; i --) {
                olda[i] = olda[i-1];
            }
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(&(olda[1]));
            leaf->Keys[IndexInLeaf] --;
            SimplyAdjustAncestorKeysBy(leaf, -1);
            NumItems--;
            return IDdeleted;
        } else if (rightlen < Capacity) {
            int * olda = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            int oldl = leaf->Keys[IndexInLeaf];
            for (int i = IndexInArray; i < oldl-1; i ++) {
                olda[i] = olda[i+1];
            }
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(olda);
            leaf->Keys[IndexInLeaf]--;
            SimplyAdjustAncestorKeysBy(leaf, -1);
            NumItems--;
            return IDdeleted;
        }
    }
    NumItems--;
    return IDdeleted;
}

void DynamicArray::MergeOrRedistributeArrayAfterDelete(NodeDA * leaf, int IndexInLeaf, int IndexInArray) {
    if (IndexInLeaf == 0 && leaf->NumOfKeys == 1) { //root
        if (IndexInArray == leaf->Keys[0]-1) {
            leaf->Keys[0]--;
        } else {
            int oldL = leaf->Keys[0];
            int * olda = reinterpret_cast<int *>(leaf->Pointers[0]);
            for (int i = IndexInArray; i < oldL-1; i ++) {
                olda[i] = olda[i+1];
            }
            leaf->Keys[0]--;
        }
        return;
    } else if( IndexInLeaf == 0 ){ // can only merge or redistribute with right neighbour
        if (leaf->Keys[1] - 1 >= Capacity) { //redistribute
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[0]);
            int leftlen = leaf->Keys[0];
            for (int i = IndexInArray; i < leftlen-1; i ++) {
                lefta[i] = lefta[i+1];
            }
            lefta[leftlen-1] = reinterpret_cast<int *>(leaf->Pointers[1])[0];
            int * t = reinterpret_cast<int *>(leaf->Pointers[1]);
            leaf->Pointers[1] = reinterpret_cast<NodeDA *>(&(t[1]));
            leaf->Keys[1] --;
            SimplyAdjustAncestorKeysBy(leaf, -1);
            return;
        } else    if (leaf->Keys[1] - 1 >= Min) { //redistribute
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[0]);
            int leftlen = leaf->Keys[0];
            for (int i = IndexInArray; i < leftlen-1; i ++) {
                lefta[i] = lefta[i+1];
            }
            lefta[leftlen-1] = reinterpret_cast<int *>(leaf->Pointers[1])[0];
            int * righta = reinterpret_cast<int *>(leaf->Pointers[1]);
            int rightoldL = leaf->Keys[1];
            for (int i = 0; i < rightoldL - 1; i ++ ){
                righta[i] = righta[i+1];
            }
            leaf->Keys[1] --;
            SimplyAdjustAncestorKeysBy(leaf, -1);
            return;
        } else { // merge
            int newl = leaf->Keys[0] + leaf->Keys[1] - 1;
            int * newa = new int[Capacity];
            int i = 0;
            int * lefta= reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            int * righta = reinterpret_cast<int*>(leaf->Pointers[IndexInLeaf + 1]);
            for (int k = 0; k < IndexInArray; k ++) {
                newa[i] = lefta[k];
                i++;
            }
            for (int k = IndexInArray+1; k < leaf->Keys[0]; k++){
                newa[i] = lefta[k];
                i++;
            }
            for (int k = 0; k < leaf->Keys[1]; k ++) {
                newa[i] = righta[k];
                i++;
            }
            leaf->Keys[0] = newl;
            leaf->Pointers[0] = reinterpret_cast<NodeDA *>(newa);
            deleteEntryForDeleteOperatoin(leaf, 1);
            return;
        }
    } else if (IndexInLeaf == leaf->NumOfKeys-1) {// can only merge or redistribute with left neighbour
        int leftlen = leaf->Keys[IndexInLeaf-1];
        if (leftlen - 1 >= Capacity) { //redistribute
            int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            for (int i = IndexInArray; i > 0; i --) {
                righta[i] = righta[i-1];
            }
            righta[0] = reinterpret_cast<int*>(leaf->Pointers[IndexInLeaf-1])[leftlen-1];
            leaf->Keys[IndexInLeaf-1] --;
            //leaf->Pointers[IndexInLeaf-1] = leaf->Pointers[IndexInLeaf-1][:leftlen-1];
            SimplyAdjustAncestorKeysBy(leaf, -1);
            return;
        } else if (leftlen - 1 >= Min) { //redistribute
            int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            for (int i = IndexInArray; i > 0; i --) {
                righta[i] = righta[i-1];
            }
            righta[0] = reinterpret_cast<int*>(leaf->Pointers[IndexInLeaf-1])[leftlen-1];
            leaf->Keys[IndexInLeaf-1] --;
            SimplyAdjustAncestorKeysBy(leaf, -1);
            return;
        } else { // merge
            int newl = leaf->Keys[IndexInLeaf] + leaf->Keys[IndexInLeaf-1] - 1;
            int * newa = new int[Capacity];
            int i = 0;
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf - 1]);
            int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            for (int k = 0; k < leaf->Keys[IndexInLeaf-1]; k ++) {
                newa[i] = lefta[k];
                i++;
            }
            for (int k = 0; k < IndexInArray; k ++) {
                newa[i] = righta[k];
                i++;
            }
            for (int k = IndexInArray+1; k < leaf->Keys[IndexInLeaf];k++) {
                newa[i] = righta[k];
                i++;
            }

            leaf->Keys[IndexInLeaf] = newl;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(newa);
            deleteEntryForDeleteOperatoin(leaf, IndexInLeaf-1);
            return;
        }
    } else {
        MergeOrRedistributeArrayAfterDeleteInMiddle(leaf, IndexInLeaf, IndexInArray);
    }
}

void DynamicArray::MergeOrRedistributeArrayAfterDeleteInMiddle(NodeDA * leaf, int IndexInLeaf, int IndexInArray) {
    if (leaf->Keys[IndexInLeaf+1] - 1 >= Capacity) { //redistribute with right
        int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        int leftlen = leaf->Keys[IndexInLeaf];
        for (int i = IndexInArray; i < leftlen-1; i ++) {
            lefta[i] = lefta[i+1];
        }
        int * t = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf+1]);
        lefta[leftlen-1] = t[0];
        leaf->Keys[IndexInLeaf+1] --;
        leaf->Pointers[IndexInLeaf+1] = reinterpret_cast<NodeDA *>(&(t[1]));
        SimplyAdjustAncestorKeysBy(leaf, -1);
        return;
    } else if (leaf->Keys[IndexInLeaf-1] - 1 >= Capacity) { //redistribute with left
        int leftlen = leaf->Keys[IndexInLeaf-1];
        int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        //rightlen = leaf->Keys[IndexInLeaf];
        for (int i = IndexInArray; i > 0; i --) {
            righta[i] = righta[i-1];
        }
        righta[0] = reinterpret_cast<int*>(leaf->Pointers[IndexInLeaf-1])[leftlen-1];
        leaf->Keys[IndexInLeaf-1] --;
        //leaf->Pointers[IndexInLeaf-1] = reinterpret_cast<int*>(leaf->Pointers[IndexInLeaf-1])[:leftlen-1];
        SimplyAdjustAncestorKeysBy(leaf, -1);
        return;
    }  else if (leaf->Keys[IndexInLeaf+1] - 1 >= Min) { //redistribute with right
        int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        int leftlen = leaf->Keys[IndexInLeaf];
        for (int i = IndexInArray; i < leftlen-1; i ++) {
            lefta[i] = lefta[i+1];
        }
        lefta[leftlen-1] = reinterpret_cast<int*>(leaf->Pointers[IndexInLeaf+1])[0];
        int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf + 1]);
        int rightoldL = leaf->Keys[IndexInLeaf+1];
        for (int i = 0; i < rightoldL - 1; i ++) {
            righta[i] = righta[i+1];
        }
        leaf->Keys[IndexInLeaf+1] --;
        SimplyAdjustAncestorKeysBy(leaf, -1);
        return;
    } else if (leaf->Keys[IndexInLeaf-1] - 1 >= Min) { //redistribute with left
        int leftlen = leaf->Keys[IndexInLeaf-1];
        int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        for (int i = IndexInArray; i > 0; i --) {
            righta[i] = righta[i-1];
        }
        righta[0] = reinterpret_cast<int*>(leaf->Pointers[IndexInLeaf-1])[leftlen-1];
        leaf->Keys[IndexInLeaf-1] --;
        SimplyAdjustAncestorKeysBy(leaf, -1);
        return;
    } else if (leaf->Keys[IndexInLeaf-1] < leaf->Keys[IndexInLeaf+1]) { //merge with left
        int newl = leaf->Keys[IndexInLeaf] + leaf->Keys[IndexInLeaf-1] - 1;
        int * newa = new int[Capacity];
        int i = 0;
        int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf - 1]);
        int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        for (int k = 0; k < leaf->Keys[IndexInLeaf-1]; k ++) {
            newa[i] = lefta[k];
            i++;
        }
        for (int k = 0; k < IndexInArray; k ++) {
            newa[i] = righta[k];
            i++;
        }
        for (int k = IndexInArray+1; k < leaf->Keys[IndexInLeaf];k++) {
            newa[i] = righta[k];
            i++;
        }
        leaf->Keys[IndexInLeaf] = newl;
        leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(newa);
        deleteEntryForDeleteOperatoin(leaf, IndexInLeaf-1);
        return;
    } else { //merge with right
        int newl = leaf->Keys[IndexInLeaf+1] + leaf->Keys[IndexInLeaf] - 1;
        int * newa = new int[Capacity];
        int i = 0;
        int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf + 1]);
        for (int k = 0; k < IndexInArray; k ++) {
            newa[i] = lefta[k];
            i++;
        }
        for (int k = IndexInArray+1; k < leaf->Keys[IndexInLeaf];k++ ){
            newa[i] = lefta[k];
            i++;
        }
        for (int k = 0; k < leaf->Keys[IndexInLeaf+1]; k ++) {
            newa[i] = righta[k];
            i++;
        }
        leaf->Keys[IndexInLeaf] = newl;
        leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(newa);
        deleteEntryForDeleteOperatoin(leaf, IndexInLeaf+1);
        return;
    }
}

void DynamicArray::insertOneIntoLeafForDeletion(NodeDA * leaf, int * newa, int length, int indexInLeaf) {
    int * pointerToStore = newa;
    int keyToStore = length;
    leaf->NumOfKeys++;
    int tmpKey;
    int i = indexInLeaf;
    for (; i < leaf->NumOfKeys-1; i++) {
        tmpKey = leaf->Keys[i];
        NodeDA * tmpPointer = leaf->Pointers[i];
        leaf->Keys[i] = keyToStore;
        leaf->Pointers[i] = reinterpret_cast<NodeDA *>(pointerToStore);
        keyToStore = tmpKey;
        pointerToStore = reinterpret_cast<int *>(tmpPointer);
    }
    leaf->Keys[i] = keyToStore;
    leaf->Pointers[i] = reinterpret_cast<NodeDA *>(pointerToStore);

    if (leaf == Root) {
        return;
    }
    NodeDA * c = leaf;
    NodeDA * parent = c->Parent;
    while (parent != nullptr) {
        for (int i = 0; i < parent->NumOfKeys; i++) {
            //if (reflecDeepEqual(parenPointers[i], c) {
            if (parent->Pointers[i] == c ) {
                parent->Keys[i] --;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}


//delete an entry and adjust ancestors
void DynamicArray::deleteEntry(NodeDA * n, int indexInNode) {
    int neighbour_index = 0;
    NodeDA * neighbour;
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
NodeDA * DynamicArray::coalesceNodes(NodeDA * n, NodeDA * neighbour, int neighbour_index) {
    int i, j;
    NodeDA * tmp;
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
    }
    deleteEntry(neighbour->Parent, n_index);
    delete n;
    return neighbour;
}

int DynamicArray::getLeftNeighborIndex(NodeDA * n) const {
    int i = 0, nei = -1;
    NodeDA * parent = n->Parent;
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


void DynamicArray::redistributeNodes(NodeDA * n, NodeDA * neighbour, int neighbour_index) {
    int i, leftNumKeys, rightNumKeys, leftIndex, leftSumKeys = 0, rightSumKeys = 0;
    if (neighbour_index != -1) { // left neighbour
        int numGoRight = neighbour->NumOfKeys - Min;
        leftIndex = neighbour_index;
        for (i = 0; i < Min; i++) {
            leftSumKeys += neighbour->Keys[i];
        }
        //int indexToGoRight = Min;
        leftNumKeys = Min;
        rightNumKeys = n->NumOfKeys + numGoRight;
        for (int j = rightNumKeys - 1; j >= numGoRight ; --j) {
            n->Pointers[j] = n->Pointers[j - numGoRight];
            n->Keys[j] = n->Keys[j - numGoRight];
            rightSumKeys += n->Keys[j];
        }
        i = neighbour->NumOfKeys - 1;
        for (int j = numGoRight - 1; j >= 0 ; --j) {
            n->Pointers[j] = neighbour->Pointers[i];
            n->Keys[j] = neighbour->Keys[i];
            rightSumKeys += n->Keys[j];
            if (n->Pointers[j] != nullptr) {
                if (!n->IsLeaf) {
                    NodeDA * s = n->Pointers[j];
                    s->Parent = n;
                }
            }
            i--;
        }
        n->NumOfKeys = rightNumKeys;
        neighbour->NumOfKeys = leftNumKeys;
    } else { // right neighbour
        leftIndex = 0;
        int j = 0;
        for (i = neighbour->NumOfKeys - 1; i >= 0; i--) {
            rightSumKeys += neighbour->Keys[i];
            j++;
            if (j == Min) {
                break;
            }
        }
        int indexToGoLeft = i - 1;
        rightNumKeys = Min;
        for (int k = 0; k < n->NumOfKeys; k++) {
            leftSumKeys += n->Keys[k];
        }
        int tmp_j = n->NumOfKeys;
        for (i = 0; i <= indexToGoLeft; i++) {
            n->Pointers[tmp_j] = neighbour->Pointers[i];
            n->Keys[tmp_j] = neighbour->Keys[i];
            leftSumKeys += n->Keys[tmp_j];
            if (n->Pointers[tmp_j] != nullptr) {
                if (!n->IsLeaf) {
                    NodeDA * s = n->Pointers[tmp_j];
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
    }

    NodeDA * c = n->Parent;
    if (n->IsLeaf && n->Pointers[0] == nullptr) {
        c->Keys[leftIndex] = leftNumKeys;
        c->Keys[leftIndex+1] = rightNumKeys;
    } else {
        c->Keys[leftIndex] = leftSumKeys ;
        c->Keys[leftIndex+1] = rightSumKeys;
    }
    AdjustAncestorKeysAccordingToCurNode(c);
}

void DynamicArray::adjustRoot() {
    NodeDA * newRoot = nullptr;
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


// only remove an entry from node, doing nothing else
NodeDA * DynamicArray::removeEntryFromNode(NodeDA * n, int indexInNode) {
    n->Keys[indexInNode] = 0;
    if (n->Pointers[indexInNode] != nullptr) {
        if (!n->IsLeaf) {
            NodeDA * deleted = n->Pointers[indexInNode];
            deleted->Parent = nullptr;
            n->Pointers[indexInNode] = nullptr;
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

void DynamicArray::Reorder(int start, int end, const int* neworder) {
    if (end > NumItems) {
        printf("Error: index exceeds limit!\n");
        return;
    }
    int length = end-start+1;
    NodeDA * c = Root;
    int p = start;
    int i = 0;
    if (c == nullptr) {
        return;
    }
    while (!c->IsLeaf) {
        for (int y = 0; y < c->NumOfKeys; ++y) {
            if (p <= c->Keys[y]) {
                c = c->Pointers[y];
                break;
            }
            p -= c->Keys[y];
        }
    }
    for (; i < c->NumOfKeys; ++i) {
        if (p <= c->Keys[i]) {
            break;
        }
        p -= c->Keys[i];
    }
    p--;
    int k = 0;
    bool flag = false;
    while (k < length) {
        for (; i < c->NumOfKeys; ++i) {
            int * curarray = reinterpret_cast<int *>(c->Pointers[i]);
            for (; p < c->Keys[i]; p++) {
                curarray[p] = neworder[k];
                k++;
                if (k == length) {
                    flag = true;
                    break;
                }
            }
            if (flag) {
                break;
            }
            p = 0;
        }
        if (flag) {
            break;
        }
        if (c->Next == nullptr) {
            printf("error\n");
        }
        c = c->Next;
        i = 0;
    }
}

void DynamicArray::coalesceNodesForDeleteOperation(NodeDA * n, NodeDA * neighbour, int neighbour_index) {
    int i, j, neighbour_insertion_index;
    NodeDA * tmp;
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
    return;
}

void DynamicArray::deleteEntryForDeleteOperatoin(NodeDA * n, int indexInNode) {
    int neighbour_index, capacity;
    NodeDA * neighbour;
    n = removeEntryFromNode(n, indexInNode);
    n->NumOfKeys--;
    if (n == Root) {
        adjustRoot();
        return;
    }
    int nNumKeys = n->NumOfKeys;
    if (nNumKeys >= Min) {
        NodeDA * tmp = n;
        while (tmp != Root) {
            for (int i = 0; i < Capacity; i++) {
                if (tmp->Parent->Pointers[i] == tmp) {
                    tmp->Parent->Keys[i] -=1;
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
    capacity = Capacity;
    if (neighbour->NumOfKeys+nNumKeys <= capacity) {
        coalesceNodesForDeleteOperation(n, neighbour, neighbour_index);
        return;
    } else {
        redistributeNodesForDeleteOperation(n, neighbour, neighbour_index);
        return;
    }
}

void DynamicArray::redistributeNodesForDeleteOperation(NodeDA * n, NodeDA * neighbour, int neighbour_index) {
    int i, leftNumKeys = 0, rightNumKeys = 0, leftIndex, leftSumKeys = 0, rightSumKeys = 0;
    if (neighbour_index != -1) {
        leftIndex = neighbour_index;
        for (i = 0; i < Min; i++) {
            leftSumKeys += neighbour->Keys[i];
        }
        int indexToGoRight = Min;
        leftNumKeys = Min;
        int temp_keys[2*Capacity];
        NodeDA * temp_pointers[2*Capacity];
        int tmp_j = 0;
        for (i = indexToGoRight; i < neighbour->NumOfKeys; i++) {
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
                    NodeDA * s = n->Pointers[i];
                    s->Parent = n;
                }
            }
        }
        neighbour->NumOfKeys = leftNumKeys;
        n->NumOfKeys = rightNumKeys;
    } else {
        leftIndex = 0;
        int j = 0;
        for (i = neighbour->NumOfKeys - 1; i >= 0; i--) {
            rightSumKeys += neighbour->Keys[i];
            j++;
            if (j == Min) {
                break;
            }
        }
        int indexToGoLeft = i - 1;
        rightNumKeys = Min;
        int tmp_j = n->NumOfKeys;
        leftSumKeys = NodeSumKeys(n);
        for (i = 0; i <= indexToGoLeft; i++) {
            n->Pointers[tmp_j] = neighbour->Pointers[i];
            n->Keys[tmp_j] = neighbour->Keys[i];
            leftSumKeys += n->Keys[tmp_j];
            if (!n->IsLeaf) {
                if (n->Pointers[tmp_j] != nullptr) {
                    NodeDA * s = n->Pointers[tmp_j];
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
    }
    NodeDA * c = n->Parent;
    c->Keys[leftIndex] = leftSumKeys;
    c->Keys[leftIndex+1] = rightSumKeys;
    c = n->Parent;
    NodeDA * parent = c->Parent;
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

void DynamicArray::CutArray(int pos) {
    int IndexInLeaf, IndexInArray;
    NodeDA * leaf = findLeaf(pos, &IndexInLeaf, &IndexInArray);
    if (IndexInArray == 0) {
        return;
    } else {
        int * tmp = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        int oldlen = leaf->Keys[IndexInLeaf];
        int leftl = IndexInArray;
        int rightl = oldlen-leftl;
        if (leftl >= Capacity && rightl >= Capacity) {
            leaf->Keys[IndexInLeaf] = leftl;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(tmp);
            int * righta = &tmp[IndexInArray];
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightl, IndexInLeaf+1, false);
            }
            return;
        } else if (leftl >= Capacity) {
            int * righta = new int[Capacity];
            for (int i = 0; i < rightl; i ++) {
                righta[i] = tmp[IndexInArray+i];
            }
            leaf->Keys[IndexInLeaf] = leftl;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(tmp);
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightl, IndexInLeaf+1, false);
            }
            return;
        } else if (rightl >= Capacity) {
            int * lefta = new int[Capacity];
            for (int i = 0; i < leftl; i ++) {
                lefta[i] = tmp[i];
            }
            leaf->Keys[IndexInLeaf] = leftl;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(lefta);
            int * righta = &tmp[IndexInArray];
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightl, IndexInLeaf+1, false);
            }
            return;
        } else {
            int * righta = new int[Capacity];
            for (int i = 0; i < rightl; i ++) {
                righta[i] = tmp[IndexInArray+i];
            }
            leaf->Keys[IndexInLeaf] = leftl;
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
            } else {
                insertOneIntoLeafAfterSplitting(leaf, righta, rightl, IndexInLeaf+1, false);
            }
            return;
        }
    }
}

//only insert one array into leaf, without changing ancestors' keys
void DynamicArray::insertOneIntoLeafForCut(NodeDA *  leaf, int * newarray, int length, int indexInLeaf) {
    int * pointerToStore = newarray;
    int keyToStore = length;
    leaf->NumOfKeys++;
    int tmpKey;
    int i = indexInLeaf;
    for (; i < leaf->NumOfKeys-1; i++) {
        tmpKey = leaf->Keys[i];
        NodeDA * tmpPointer = leaf->Pointers[i];
        leaf->Keys[i] = keyToStore;
        leaf->Pointers[i] = reinterpret_cast<NodeDA *>(pointerToStore);
        keyToStore = tmpKey;
        pointerToStore = reinterpret_cast<int *>(tmpPointer);
    }
    leaf->Keys[i] = keyToStore;
    leaf->Pointers[i] = reinterpret_cast<NodeDA *>(pointerToStore);
    return;
}



//=============================================swap=========================================================

void DynamicArray::Swap(int start1, int end1, int start2, int end2) {
    //cut array from edges of swapping ranges
    CutArray(start1);
    CutArray(end1+1);
    CutArray(start2);
    CutArray(end2+1);
    //printf("after cutarray:\n");
    //PrintTree();
    int numLeaf1, numLeaf2, startIndexLeaf1, startIndexLeaf2, endIndexLeaf1, endIndexLeaf2;
    NodeDA ** inter1 = nullptr;
    NodeDA ** inter2 = nullptr;
    numLeaf1 = RangePosInLeafPointer(start1, end1, &inter1, &startIndexLeaf1, &endIndexLeaf1);
    numLeaf2 = RangePosInLeafPointer(start2, end2, &inter2, &startIndexLeaf2, &endIndexLeaf2);

    swapLeaf(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
    delete []inter1;
    delete []inter2;
    int s1 = start1;
    int e1 = s1+(end2-start2);
    int e2 = end2;
    int s2 = e2 - (end1-start1);
    CheckMinArrayAfterSwap(s1-1);
    CheckMinArrayAfterSwap(s1);
    CheckMinArrayAfterSwap(e1-1);
    CheckMinArrayAfterSwap(e1);
    CheckMinArrayAfterSwap(e1+1);
    CheckMinArrayAfterSwap(s2-1);
    CheckMinArrayAfterSwap(s2);
    CheckMinArrayAfterSwap(e2-1);
    CheckMinArrayAfterSwap(e2);
    CheckMinArrayAfterSwap(e2+1);
}
/*
void DAonedp::Swap(int start1, int end1, int start2, int end2, int* CutArrayTime, int* FindLeafTime, int* swapLeafTime, int*CheckMinTime) {
    TimeVar time1 = timeNow();
    //cut array from edges of swapping ranges
    CutArray(start1);
    CutArray(end1+1);
    CutArray(start2);
    CutArray(end2+1);
    TimeVar time2 = timeNow();
    *CutArrayTime = duration(time2-time1);

    TimeVar time3 = timeNow();
    int numLeaf1, numLeaf2, startIndexLeaf1, startIndexLeaf2, endIndexLeaf1, endIndexLeaf2;
    NodeDA ** inter1 = nullptr;
    NodeDA ** inter2 = nullptr;
    numLeaf1 = RangePosInLeafPointer(start1, end1, &inter1, &startIndexLeaf1, &endIndexLeaf1);
    numLeaf2 = RangePosInLeafPointer(start2, end2, &inter2, &startIndexLeaf2, &endIndexLeaf2);
    TimeVar time4 = timeNow();
    *FindLeafTime = duration(time4-time3);

    TimeVar time5 = timeNow();
    swapLeaf(inter1, inter2, numLeaf1, numLeaf2, startIndexLeaf1, endIndexLeaf1, startIndexLeaf2, endIndexLeaf2);
    TimeVar time6 = timeNow();
    *swapLeafTime = duration(time6-time5);
    //cout<<"after swapLeaf"<<endl;
    //PrintTree();
    TimeVar time7 = timeNow();
    int s1 = start1;
    int e1 = s1+(end2-start2);
    int e2 = end2;
    int s2 = e2 - (end1-start1);
    CheckMinArrayAfterSwap(s1-1);
    CheckMinArrayAfterSwap(s1);
    CheckMinArrayAfterSwap(e1);
    CheckMinArrayAfterSwap(e1+1);
    CheckMinArrayAfterSwap(s2-1);
    CheckMinArrayAfterSwap(s2);
    CheckMinArrayAfterSwap(e2);
    CheckMinArrayAfterSwap(e2+1);
    TimeVar time8 = timeNow();
    *CheckMinTime = duration(time8-time7);
}
*/
int DynamicArray::RangePosInLeafPointer(int start, int end, NodeDA *** inter, int * startIndexLeaf, int * endIndexLeaf){
    int numLeaf = 0;
    int numLeafMost = (end-start+1)/Min + 2;
    *inter = new NodeDA * [numLeafMost];
    int i_leafArray = 0;
    NodeDA * StartLeaf;
    int StartIndexInLeaf, StartIndexInArray;
    StartLeaf = findLeaf(start, &StartIndexInLeaf, &StartIndexInArray);
    NodeDA * c = StartLeaf;
    int length = end - start + 1;
    int i_leaf = StartIndexInLeaf;
    if (StartIndexInArray != 0) {
        printf("error in RangePosInLeafPointer!\n");
    }
    while (c != nullptr) {
        *((*inter)+i_leafArray) = c;
        i_leafArray++;
        numLeaf++;
        for (int j = i_leaf; j < c->NumOfKeys; j++) {
            if (length < c->Keys[j]) {
                printf("error in RangePosInLeafPointer!\n");
            } else if (length == c->Keys[j]) {
                * startIndexLeaf = StartIndexInLeaf;
                * endIndexLeaf = j;
                return numLeaf;
            } else {
                length -= c->Keys[j];
            }
        }
        c = c->Next;
        i_leaf = 0;
    }
    delete []inter;
    inter = nullptr;
    numLeaf = 0;
    * startIndexLeaf = 0;
    * endIndexLeaf = 0;
    return numLeaf;
}

void DynamicArray::CheckMinArrayAfterSwap(int s){
    if (s == 0) {
        return;
    }
    int IndexInLeaf, IndexInArray;
    NodeDA * leaf = findLeaf(s, &IndexInLeaf, &IndexInArray);
    if (leaf->Keys[IndexInLeaf] >= Min) {
        return;
    }
    MergeRedistributeArrayAfterSwap(leaf, IndexInLeaf);
}

void DynamicArray::MergeRedistributeArrayAfterSwap(NodeDA * leaf, int IndexInLeaf) {
    if (IndexInLeaf == 0) { // can only adjust with right array
        int leftl = leaf->Keys[0];
        int rightl = leaf->Keys[1];
        if (rightl == 0) {
            return;
        }
        if (rightl + leftl <= Capacity) { // merge
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[0]);
            int * righta = reinterpret_cast<int *>(leaf->Pointers[1]);
            int  newl = leftl+rightl;
            for (int i = leftl; i < newl; i ++) {
                lefta[i] = righta[i-leftl];
            }
            leaf->Keys[0] = newl;
            deleteEntryForCheckMinArray(leaf, 1);
            return;
        } else if (rightl + leftl < Capacity + Min) { // redistribute but right needs to move keys
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[0]);
            int * righta = reinterpret_cast<int *>(leaf->Pointers[1]);
            int NumToMove = Min - leftl;
            int k = leftl;
            for (int i = 0; i < NumToMove; i ++) {
                lefta[k] = righta[i];
                k++;
            }
            rightl = rightl - NumToMove;
            leftl += NumToMove;
            for (int i = 0; i < rightl; i ++) {
                righta[i] = righta[i+NumToMove];
            }
            leaf->Keys[1] -= NumToMove;
            leaf->Keys[0] += NumToMove;
            return;
        } else { // redistribute
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[0]);
            int * righta = reinterpret_cast<int *>(leaf->Pointers[1]);
            int NumToMove = Min - leftl;
            int k = leftl;
            for (int i = 0; i < NumToMove; i ++) {
                lefta[k] = righta[i];
                k++;
            }
            leaf->Keys[0]+= NumToMove;
            leaf->Pointers[1] = reinterpret_cast<NodeDA *>(&(righta[NumToMove])); // must be &()
            leaf->Keys[1]-=NumToMove;
            return;
        }
    } else { // with left neighbour
        int leftl = leaf->Keys[IndexInLeaf-1];
        int rightl = leaf->Keys[IndexInLeaf];
        if (leftl == 0) {
            return;
        }
        if (leftl + rightl <= Capacity) { // merge
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf - 1]);
            int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            int newl = leftl+rightl;
            for (int i = leftl; i < newl; i ++) {
                lefta[i] = righta[i-leftl];
            }
            leaf->Keys[IndexInLeaf-1] = newl;
            deleteEntryForCheckMinArray(leaf, IndexInLeaf);
            return;
        } else if (leftl + rightl < Capacity + Min) { // redistribute
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf - 1]);
            int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            int NumToMove = Min - rightl;
            int j = rightl-1;
            int i = Min - 1;
            for (; i >= 0; i --) {
                righta[i] = righta[j];
                if (j == 0) {
                    break;
                }
                j--;
            }
            int k = leftl-1;
            i--;
            for (; i >= 0; i --) {
                righta[i] = lefta[k];
                k--;
            }
            leaf->Keys[IndexInLeaf-1] -= NumToMove;
            leaf->Keys[IndexInLeaf] += NumToMove;
            return;
        } else { // redistribute
            int * lefta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf - 1]);
            int * righta = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
            int NumToMove = Min - rightl;
            int j = rightl-1;
            int i = Min - 1;
            for (; i >= 0; i --) {
                righta[i] = righta[j];
                if (j == 0) {
                    break;
                }
                j--;
            }
            int k = leftl-1;
            i--;
            for (; i >= 0; i --) {
                righta[i] = lefta[k];
                k--;
            }
            leaf->Keys[IndexInLeaf-1] -= NumToMove;
            leaf->Keys[IndexInLeaf] += NumToMove;
            leaf->Pointers[IndexInLeaf-1] = reinterpret_cast<NodeDA *>(lefta);
            //leaf->Pointers[IndexInLeaf-1] = lefta[:leftl-NumToMove];
            return;
        }
    }
}

void DynamicArray::deleteEntryForCheckMinArray(NodeDA * n, int indexInNode) {
    int neighbour_index, capacity;
    NodeDA * neighbour;
    n = removeEntryFromNode(n, indexInNode);
    n->NumOfKeys--;
    if (n == Root) {
        adjustRoot();
        return;
    }
    int nNumKeys = n->NumOfKeys;
    if (nNumKeys >= Min) {
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

void DynamicArray::redistributeNodesForCheckMinArray(NodeDA * n, NodeDA * neighbour, int neighbour_index) {
    int i, leftNumKeys = 0, rightNumKeys = 0, leftIndex, leftSumKeys = 0, rightSumKeys = 0;
    if (neighbour_index != -1) { // neighbor (more keys) is on the left
        leftIndex = neighbour_index;
        for (i = 0; i < Min; i++) {
            leftSumKeys += neighbour->Keys[i];
        }
        int indexToGoRight = Min;
        leftNumKeys = Min;

        int * temp_keys = new int[Capacity*2];
        NodeDA ** temp_pointers = new NodeDA *[Capacity*2];
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
                    NodeDA * s = n->Pointers[i];
                    s->Parent = n;
                }
            }
        }
        neighbour->NumOfKeys = leftNumKeys;
        n->NumOfKeys = rightNumKeys;
        delete []temp_pointers;
    } else { // neighbor is on the right (more keys) (neighbor_index == -1)
        leftIndex = 0;
        int j = 0;
        for (i = neighbour->NumOfKeys - 1; i >= 0; i--) {
            rightSumKeys += neighbour->Keys[i];
            j++;
            if (j == Min) {
                break;
            }
        }
        int indexToGoLeft = i - 1;
        rightNumKeys = Min;

        int tmp_j = n->NumOfKeys;
        leftSumKeys = NodeSumKeys(n);
        for (i = 0; i <= indexToGoLeft; i++) {
            n->Pointers[tmp_j] = neighbour->Pointers[i];
            n->Keys[tmp_j] = neighbour->Keys[i];
            leftSumKeys += n->Keys[tmp_j];
            if (!n->IsLeaf) {
                if (n->Pointers[tmp_j] != nullptr) {
                    NodeDA * s = n->Pointers[tmp_j];
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
    }
    NodeDA * c = n->Parent;
    c->Keys[leftIndex] = leftSumKeys;
    c->Keys[leftIndex+1] = rightSumKeys;
    return;
}

void DynamicArray::coalesceNodesForCheckMinArray(NodeDA * n, NodeDA * neighbour, int neighbour_index) {
    int i, j, neighbour_insertion_index ;
    NodeDA *  tmp ;
    int n_index = neighbour_index + 1;
    //set neighbor to be the NodeDA on the left
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
            NodeDA * tmp = neighbour->Next;
            tmp->Previous = neighbour;
        }
         */
    }
    n->NumOfKeys = 0;
    deleteEntryForCheckMinArray(n->Parent, n_index);
    return;
}

int DynamicArray::getIndexInParent(NodeDA * n) {
    NodeDA * parent = n->Parent;
    for (int i = 0; i < Capacity; i++) {
        if (parent->Pointers[i] == n) {
            return i;
        }
    }
    return -1;
}


//--------------------------------------------------------- ***
void DynamicArray::swapLeaf(NodeDA ** inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2, int startIndexLeaf1,
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
    NodeDA **node1 = inter1;
    NodeDA **node2 = inter2;
    //TimeVar time1 = timeNow();
    swapWholeLeafAndGoUpwards(node1, node2, numLeaf1, numLeaf2);
    //TimeVar time2 = timeNow();
    //printf("time to swapWholeLeafAndGoUpwards = %d\n", duration(time2-time1));

    //move pointers in starting leaf
    NodeDA * lastl1 = node1[numLeaf1-1];
    NodeDA * lastl2 = node2[numLeaf2-1];
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


void DynamicArray::swapEndPointer(NodeDA * leftLeaf, int indexInLeftLeaf, int endIndexInLeftLeaf,
                                  NodeDA * rightLeaf, int indexInRightLeaf , int endIndexInRightLeaf) {
    int len1 = endIndexInLeftLeaf + 1 - indexInLeftLeaf;
    int len2 = endIndexInRightLeaf + 1 - indexInRightLeaf;

    int j = indexInRightLeaf;
    int i = indexInLeftLeaf;

    int exchangeNum = len1;
    if (exchangeNum > len2) {
        exchangeNum = len2;
    }
    for (int k = 0; k < exchangeNum; k++) {
        NodeDA * tmpPointer = leftLeaf->Pointers[i];
        leftLeaf->Pointers[i] = rightLeaf->Pointers[j];
        rightLeaf->Pointers[j] = tmpPointer;
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

NodeDA * DynamicArray::findPrevious(NodeDA * node) {
    if (node == Root || node->Parent == nullptr) {
        return nullptr;
    }
    int idxInParent = getIndexInParent(node);
    if (idxInParent > 0) {
        NodeDA * cur = node->Parent->Pointers[idxInParent-1];
        while (!cur->IsLeaf) {
            cur = cur->Pointers[cur->NumOfKeys-1];
        }
        return cur;
    } else {
        return findPrevious(node->Parent);
    }
}

//call this function after deleting a range of leaves
void DynamicArray::CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeDA * preLeaf, NodeDA ** leafToDelete, int numLeafToDelete) {
    NodeDA * leftLeafParent = leafToDelete[0]->Parent;
    NodeDA * rightLeafParent = leafToDelete[numLeafToDelete-1]->Parent;
    if (leftLeafParent->NumOfKeys != 0) {
        RedistributeMergeRecomputeKeysInAncestorsLeftPart(leftLeafParent);
    } else {
        //NodeDA * pre = findPrevious(leafToDelete[0]);
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


void DynamicArray::swapWholeLeafAndGoUpwards(NodeDA **wholeLeaf1, NodeDA **wholeLeaf2, int w1, int w2) {
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
    NodeDA * LeftEdgeLeaf = nullptr, * RightEdgeLeaf = nullptr;
    NodeDA ** LeftParentArray = new NodeDA * [exchangeNum];
    NodeDA ** RightParentArray = new NodeDA * [exchangeNum];
    int lp = 0, rp = 0;
    NodeDA * lastLeftGrandParent = nullptr;
    NodeDA * lastRightGrandParent = nullptr;

    // adjust next, previous pointers
    NodeDA * LeftStartLeaf = wholeLeaf1[0];
    NodeDA * RightStartLeaf = wholeLeaf2[0];
    NodeDA * LeftEndLeaf = wholeLeaf1[exchangeNum-1];
    NodeDA * RightEndLeaf = wholeLeaf2[exchangeNum-1];
    if (LeftEndLeaf->Next == RightStartLeaf) {
        LeftEndLeaf->Next = RightEndLeaf->Next;
        /*
        if (LeftEndLeaf->Next != nullptr) {
            LeftEndLeaf->Next->Previous = LeftEndLeaf;
        }
        RightStartLeaf->Previous = LeftStartLeaf->Previous;
         */
        NodeDA * preLeftStar = findPrevious(LeftStartLeaf);
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
            NodeDA * LeftPre = findPrevious(LeftEdgeLeaf);
            //RightEdgeLeaf->Previous = LeftPre;
            if (LeftPre != nullptr) {
                LeftPre->Next = RightEdgeLeaf;
            }
            RightEdgeLeaf->Next = LeftEdgeLeaf;
            LeftEdgeLeaf->Previous = RightEdgeLeaf;
        } else {
         */
        NodeDA * LeftPre = findPrevious(LeftEdgeLeaf);
        NodeDA * RightPre = findPrevious(RightEdgeLeaf);
        //RightEdgeLeaf->Previous = LeftPre;
        if (LeftPre != nullptr) {
            LeftPre->Next = RightEdgeLeaf;
        }
        //LeftEdgeLeaf->Previous = RightPre;
        if (RightPre != nullptr) {
            RightPre->Next = LeftEdgeLeaf;
        }

        LeftEdgeLeaf = wholeLeaf1[exchangeNum-1];
        RightEdgeLeaf = wholeLeaf2[exchangeNum-1];
        //need to adjust next previous pointers of the last two exchanging leaves
        NodeDA * LeftNext = LeftEdgeLeaf->Next;
        NodeDA * RightNext = RightEdgeLeaf->Next;
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
        NodeDA * LeftLeaf = wholeLeaf1[i];
        NodeDA * RightLeaf = wholeLeaf2[i];
        NodeDA * LeftParent = LeftLeaf->Parent;
        NodeDA * RightParent = RightLeaf->Parent;

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
    delete []LeftParentArray;
    delete []RightParentArray;
    //---------------------------end exchange corresponding leaves------------------------------------------

    //delete leaves
    if (remain != 0) {
        //delete leaves without merging/redistribution/adjusting ancestor's keys
        NodeDA **  leafToDelete;
        int numLeafToDelete ;
        NodeDA * pre = nullptr;
        if (remain == -1) { // left leaves more
            numLeafToDelete = w1 - w2;
            leafToDelete = &wholeLeaf1[exchangeNum];
            pre = wholeLeaf2[exchangeNum-1];
        } else { // remain == 1, right leaves more
            numLeafToDelete = w2 - w1;
            leafToDelete = &wholeLeaf2[exchangeNum];
            pre = wholeLeaf1[exchangeNum-1];
        }
        //NodeDA * pre = findPrevious(leafToDelete[0]); // can be optimized
        if (pre != nullptr) {
            pre->Next = leafToDelete[numLeafToDelete-1]->Next;
        }
        /*
        if (leafToDelete[numLeafToDelete-1]->Next != nullptr) {
            leafToDelete[numLeafToDelete-1]->Next->Previous = leafToDelete[0]->Previous;
        }
         */
        //only delete entries, no redistribution or merging
        NodeDA * preLeaf = deleteMulEntry(leafToDelete, numLeafToDelete);
        // adjust keys in ancestors
        CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, leafToDelete, numLeafToDelete);

        // insert leaves
        if (remain == -1) { // left leaves more
            NodeDA * toParent = RightEdgeLeaf->Parent;
            int startIndexInParent = getIndexInParent(RightEdgeLeaf) + 1;
            insertLeavesIntoLeafParent(toParent, startIndexInParent, leafToDelete, numLeafToDelete);
        } else if (remain == 1) { // right leaves more
            NodeDA *  toParent = LeftEdgeLeaf->Parent;
            int startIndexInParent = getIndexInParent(LeftEdgeLeaf) + 1;
            insertLeavesIntoLeafParent(toParent, startIndexInParent, leafToDelete, numLeafToDelete);
        }
    }
}


void DynamicArray::exchangeLeaf(NodeDA * LeftLeaf, NodeDA * LeftParent, NodeDA * RightLeaf, NodeDA * RightParent) const{
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



NodeDA * DynamicArray::movePointerAfter(NodeDA * fromLeaf, int startIndex, int endIndex, NodeDA * toLeaf, int toIndex) {
    int * keysToInsert = new int[Capacity];
    auto ** pointersToInsert = new NodeDA *[Capacity];
    for (int i = startIndex; i <= endIndex; i++) {
        keysToInsert[i-startIndex] = fromLeaf->Keys[i];
        pointersToInsert[i-startIndex] = fromLeaf->Pointers[i];
    }
    int numKeysToInsert = endIndex - startIndex + 1;
    //insert
    insertKeysIntoLeaf(keysToInsert, pointersToInsert, numKeysToInsert, toLeaf, toIndex);
    delete []pointersToInsert;
    delete []keysToInsert;
    //delete
    int q = startIndex;
    int p = endIndex + 1;
    while (p < fromLeaf->NumOfKeys) {
        fromLeaf->Keys[q] = fromLeaf->Keys[p];
        fromLeaf->Pointers[q] = fromLeaf->Pointers[p];
        p++;
        q++;
    }
    fromLeaf->NumOfKeys -= numKeysToInsert;
    return adjustAfterDeleteKeysInNode(fromLeaf);
}

void DynamicArray::insertLeavesIntoLeafParent(NodeDA * toParent, int startIndexInParent, NodeDA ** leafToInsert, int numLeafToInsert){
    if (toParent->NumOfKeys + numLeafToInsert <= Capacity) {
        insertMulLeavesIntoLeafParent(toParent, startIndexInParent, leafToInsert, numLeafToInsert);
    } else {
        insertMulLeavesIntoLeafParentAfterSplitting(toParent, startIndexInParent, leafToInsert, numLeafToInsert);
    }
}


// no need to split
void DynamicArray::insertMulLeavesIntoLeafParent(NodeDA * toParent, int startIndexInParent, NodeDA ** leafToInsert, int numLeafToInsert){
    NodeDA * last = leafToInsert[numLeafToInsert-1];
    if (toParent->NumOfKeys == startIndexInParent){
        last->Next = toParent->Pointers[startIndexInParent-1]->Next;
        //leafToInsert[0]->Previous = toParent->Pointers[startIndexInParent-1];
        /*
        if (last->Next != nullptr){
            last->Next->Previous = last;
        }
         */
        NodeDA * pre = toParent->Pointers[startIndexInParent-1];
        if (pre != nullptr){
            pre->Next = leafToInsert[0];
        }
    } else {
        last->Next = toParent->Pointers[startIndexInParent];
        //leafToInsert[0]->Previous = toParent->Pointers[startIndexInParent]->Previous;
        /*
        if (last->Next != nullptr){
            last->Next->Previous = last;
        }
         */
        NodeDA * pre = findPrevious(toParent->Pointers[startIndexInParent]);
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


// adjust keys in ancestors of NodeDA c
void DynamicArray::AdjustAncestorKeyForOneNode(NodeDA * c) {
    NodeDA * parent = c->Parent;
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
void DynamicArray::insertMulLeavesIntoLeafParentSplitOnce(NodeDA * toParent, int startIndexInParent,
                                                          NodeDA ** leafToInsert, int numLeafToInsert) {
    int allNum = toParent->NumOfKeys + numLeafToInsert;
    int leftNum = allNum / 2;
    int rightNum = allNum - leftNum;
    int leftSum = 0, rightSum = 0;
    auto ** allLeaf = new NodeDA *[Capacity + numLeafToInsert];
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
    auto * newn = new NodeDA(Capacity);
    for (int l = 0; l < rightNum; ++l) {
        newn->Pointers[l] = allLeaf[s];
        newn->Pointers[l]->Parent = newn;
        newn->Keys[l] = allKeys[s];
        rightSum += newn->Keys[l];
        s++;
    }
    newn->NumOfKeys = rightNum;
    delete []allLeaf;
    delete []allKeys;
    insertIntoParent(toParent, leftSum, newn, rightSum);
}


//may split more than once
void DynamicArray::insertMulLeavesIntoLeafParentAfterSplitting(NodeDA * toParent, int startIndexInParent,
                                                               NodeDA ** leafToInsert, int numLeafToInsert) {
    //next, previous
    NodeDA * last = leafToInsert[numLeafToInsert-1];
    if (toParent->NumOfKeys == startIndexInParent) {
        last->Next = toParent->Pointers[startIndexInParent-1]->Next;
        /*
        leafToInsert[0]->Previous = toParent->Pointers[startIndexInParent-1];
        if (last->Next != nullptr) {
            last->Next->Previous = last;
        }
         */
        NodeDA * pre = toParent->Pointers[startIndexInParent-1];
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
        NodeDA * pre = findPrevious(toParent->Pointers[startIndexInParent]);
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
    auto ** leafInToParentTail = new NodeDA *[numToParentAfterInsert];
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
    auto ** parentNewSibling = new NodeDA *[parentNewSiblingNum];
    int LeafToInsertIdx = numToParentAfterInsert;
    int leafInToParentTailIdx = 0;
    for (int k = 0; k < parentNewSiblingNum; ++k) {
        parentNewSibling[k] = new NodeDA(Capacity);
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
    delete []parentNewSibling;
}

void DynamicArray::insertMulIntoParent(NodeDA * toParent, NodeDA ** parentNewSibling, int parentNewSiblingNum) {
    NodeDA * parent = toParent->Parent;
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

void DynamicArray::insertMulIntoNodeAfterSplitting(NodeDA * parent, int toIndex, NodeDA ** pointerToInsert,
                                                   int pointerToInsertNum) {
    auto ** allPointers = new NodeDA *[Capacity + pointerToInsertNum];
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
        auto ** parentNewSibling = new NodeDA *[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = new NodeDA(Capacity);
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
        delete []parentNewSibling;
        delete [] allPointers;
        return;
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
        auto **parentNewSibling = new NodeDA *[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = new NodeDA(Capacity);
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
        delete []allPointers;
    }
}

void DynamicArray::insertMulIntoNode(NodeDA * parent, int toIndex, NodeDA ** parentNewSibling,
                                     int parentNewSiblingNum, int ParentIncrease) {
    auto ** tmpNodes = new NodeDA *[Capacity];
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
    NodeDA * c = parent;
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


void DynamicArray::insertMulIntoNewRoot(NodeDA * toParent, NodeDA ** parentNewSibling, int parentNewSiblingNum) {
    int totalNum = parentNewSiblingNum + 1;
    int numFullNodes = totalNum / Capacity;
    if ((numFullNodes == 0) || (totalNum == Capacity)) { // new root!
        Root = new NodeDA(Capacity);
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
        auto * leftNode = new NodeDA(Capacity);
        int rightSiblingNum = numFullNodes - 1;
        auto ** rightSibling = new NodeDA *[rightSiblingNum];
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
            rightSibling[i] = new NodeDA(Capacity);
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
        auto * leftNode = new NodeDA(Capacity);
        leftNode->Pointers[0] = toParent;
        leftNode->Keys[0] = NodeSumKeys(toParent);
        leftNode->NumOfKeys = 1;
        toParent->Parent = leftNode;
        int rightSiblingNum = numFullNodes;
        numFullNodes--;
        auto ** rightSibling = new NodeDA *[rightSiblingNum];
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
            rightSibling[0] = new NodeDA(Capacity);
            for (int i = 0; i < numkeysInLast; i++) {
                rightSibling[0]->Pointers[i] = parentNewSibling[i_rightchild];
                rightSibling[0]->Keys[i] = NodeSumKeys(parentNewSibling[i_rightchild]);
                parentNewSibling[i_rightchild]->Parent = rightSibling[0];
                i_rightchild++;
            }
            rightSibling[0]->NumOfKeys = numkeysInLast;
        } else { // leftNode is not the second last one
            for (int i = 1; i < Capacity; i++) { // leftNode
                leftNode->Pointers[i] = parentNewSibling[i_rightchild];
                leftNode->Keys[i] = parentNewSibling[i_rightchild]->NumOfKeys;
                parentNewSibling[i_rightchild]->Parent = leftNode;
                i_rightchild++;
            }
            leftNode->NumOfKeys = Capacity;
            for (int k = 0; k < rightSiblingNum; k++) {
                rightSibling[k] = new NodeDA(Capacity);
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


NodeDA * DynamicArray::movePointerBefore(NodeDA * fromLeaf, int startIndex, int endIndex, NodeDA * toLeaf, int toIndex) {
    int * keysToInsert = new int[Capacity];
    NodeDA ** pointersToInsert = new NodeDA *[Capacity];
    for (int i= startIndex; i <= endIndex; i++) {
        keysToInsert[i-startIndex] = fromLeaf->Keys[i];
        pointersToInsert[i-startIndex] = fromLeaf->Pointers[i];
    }
    int numKeysToInsert = endIndex - startIndex + 1;
    insertKeysIntoLeaf(keysToInsert, pointersToInsert, numKeysToInsert, toLeaf, toIndex);

    //delete
    for (int i = endIndex + 1; i < fromLeaf->NumOfKeys; i++) {
        fromLeaf->Keys[i-endIndex-1] = fromLeaf->Keys[i];
        fromLeaf->Pointers[i-endIndex-1] = fromLeaf->Pointers[i];
    }
    fromLeaf->NumOfKeys -= numKeysToInsert;
    delete []keysToInsert;
    delete []pointersToInsert;
    return adjustAfterDeleteKeysInNode(fromLeaf);
}

void DynamicArray::UpdateSumKeys(NodeDA ** nodes, int numNode) {
    if (nodes[0] == Root) {
        return;
    }
    if (numNode == 1) {
        AdjustAncestorKeysAccordingToCurNode(nodes[0]);
        return;
    }
    NodeDA ** newArray = new NodeDA *[numNode];// make([]*Node, numNode)
    int numNewArray = 0;

    NodeDA * lastParent = nullptr, * parent = nullptr;
    int index = - 1, i = -1;
    for (i = 0; i < numNode; i ++) {
        if (nodes[i]->NumOfKeys > 0) {
            index = getIndexInParent(nodes[i]);
            lastParent = nodes[i]->Parent;
            newArray[i] = lastParent;
            numNewArray++;
            //    lastParent.Keys[index] = t.NodeSumKeys(nodes[i])
            break;
        }
    }
    for (; i < numNode; i ++) {
        if (nodes[i]->NumOfKeys == 0) {
            continue;
        }
        parent = nodes[i]->Parent;
        if (lastParent != parent) {
            newArray[numNewArray] = parent;
            lastParent = parent;
            numNewArray++;
            index = 0;
        }
        parent->Keys[index] = NodeSumKeys(nodes[i]);
        index ++;
    }
    if (numNewArray > 0) {
        UpdateSumKeys(newArray, numNewArray);
    }
    delete []newArray;
}

/*
 * increase/decrease keys in the path from node n to root,
 * the difference is calculated by studying the parent of n
 */
void DynamicArray::AdjustAncestorKeysAccordingToCurNode(NodeDA * n) {
    if (n->Parent == nullptr || n == Root) {
        return;
    }
    int delta = 0;
    NodeDA * c = n;
    NodeDA * parent = c->Parent;
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

/*
 * increase every key in the path from node to root by increase
 */
void DynamicArray::SimplyAdjustAncestorKeysBy(NodeDA * node, int increase) {
    if (node == Root) {
        return;
    }
    NodeDA * c = node;
    NodeDA * parent = c->Parent;
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

/*
 * adjust ancestor nodes after delete
 * including merging and redistribution
 */
NodeDA * DynamicArray::CompleteAdjustAncestorNodesAfterDelete(NodeDA * n){
    if (n->NumOfKeys < Min) {
        NodeDA * re = nullptr;
        NodeDA * neighbour ;
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

void DynamicArray::replaceKeysInLeaf(NodeDA * leaf, int startIdx, int endIdx, int * keysToInsert, NodeDA ** pointersToInsert, int numToInsert) {
    int numInLeaf = endIdx-startIdx+1;
    int exchange = numInLeaf;
    if (exchange > numToInsert) {
        exchange = numToInsert;
    }
    for (int i = 0; i < exchange; ++i) {
        leaf->Keys[i+startIdx] = keysToInsert[i];
        leaf->Pointers[i+startIdx] = pointersToInsert[i];
    }
    if (numInLeaf == numToInsert) {
        AdjustAncestorKeysAccordingToCurNode(leaf);
        return;
    } else if (numInLeaf < numToInsert) {
        insertKeysIntoLeaf(&keysToInsert[exchange], &pointersToInsert[exchange], numToInsert-exchange, leaf, endIdx+1);
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
void DynamicArray::CheckAndMergeRedistributeNode(NodeDA *n) {
    if (n->NumOfKeys < Min) {
        NodeDA * neighbour ;
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



/*
 * adjust from node fromLeaf to root
 * at the same time redistribute or merge
 * numKeysDeleted = -1 if omitted
 */
NodeDA * DynamicArray::adjustAfterDeleteKeysInNode(NodeDA * fromLeaf, int numKeysDeleted) {
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
        NodeDA * neighbour;
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


//only delete entries, no redistribution or merging
// return previous of the first deleted leaf
NodeDA * DynamicArray::deleteMulEntry(NodeDA ** entryToDelete, int numEntryToDelete){
    NodeDA * pre = findPrevious(entryToDelete[0]);
    auto ** newEntryToDelete = new NodeDA *[numEntryToDelete/Min+2];
    int i_newEntryToDelete = 0;
    NodeDA * lastParent = entryToDelete[0]->Parent;
    NodeDA * FirstParent = lastParent;
    int StartIndexFirstParent = getIndexInParent(entryToDelete[0]);
    NodeDA * curParent = nullptr;
    int OldNumKeysInCurParent = FirstParent->NumOfKeys;

    for (int i = 0; i < numEntryToDelete; i++) {
        NodeDA * curEntry = entryToDelete[i];
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
void DynamicArray::movePointerInNodeForward(NodeDA * node, int startIndex, int endIndex, int toIndex){
    int delta = startIndex - toIndex;
    for (int i = startIndex; i <= endIndex; i++) {
        node->Keys[i-delta] = node->Keys[i];
        node->Pointers[i-delta] = node->Pointers[i];
    }
}


// call this function after deleting a continuous range of leaves
void DynamicArray::RedistributeMergeRecomputeKeysInAncestorsLeftPart(NodeDA * node) {
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
            NodeDA *newroot = node->Pointers[0];
            newroot->Parent = nullptr;
            Root = newroot;
        }
        return;
    }
    if (node->NumOfKeys == 0) {
        printf("impossible! In RedistributeMergeRecomputeKeysInAncestorsLeftPart, node->NumOfKeys=0\n");
        return;
    }
    NodeDA * parent = node->Parent;
    int index = getIndexInParent(node);
    NodeDA * neighbour;
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


void DynamicArray::adjustAncestorKeysAfterExchangingLeaves(NodeDA ** nodeArray, int numNodeInArray) {
    if (nodeArray[0]->Parent == nullptr) {
        NodeDA * root = nodeArray[0];
        for (int i = 0; i < root->NumOfKeys; i++) {
            root->Keys[i] = NodeSumKeys(root->Pointers[i]);
        }
        return; //root
    }
    auto ** newArray = new NodeDA *[numNodeInArray];
    int numNewArray = 0;
    NodeDA * lastParent = nullptr, * parent = nullptr;
    lastParent = nullptr;
    for (int i = 0; i < numNodeInArray; i++) {
        parent = nodeArray[i]->Parent;
        if (lastParent != parent) {
            newArray[numNewArray] = parent;
            lastParent = parent;
            numNewArray++;
        }
        NodeDA * cur = nodeArray[i];
        for (int j = 0; j < cur->NumOfKeys; j++) {
            cur->Keys[j] = NodeSumKeys(cur->Pointers[j]);
        }
    }
    if (numNewArray > 1) {
        adjustAncestorKeysAfterExchangingLeaves(newArray, numNewArray);
    } else {
        for (int i = 0; i < parent->NumOfKeys; ++i) {
            parent->Keys[i] = NodeSumKeys(parent->Pointers[i]);
        }
        AdjustAncestorKeysAccordingToCurNode(parent);
    }
    delete []newArray;
}


void DynamicArray::swapHeadTail11(NodeDA * inter1, int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2,
                                  int endIndexLeaf2) {

    int num = endIndexLeaf2 - startIndexLeaf1 + 1;
    int * Keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int ik = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    int jk = 0;
    for (int i = startIndexLeaf1; i <= endIndexLeaf2; i++) {
        inter1->Keys[i] = Keys[jk];
        inter1->Pointers[i] = Pointers[jk];
        jk++;
    }
    auto ** nodes = new NodeDA *[1];
    nodes[0] = inter1;
    UpdateSumKeys(nodes, 1);
    delete []nodes;
    delete []Keys;
    delete []Pointers;
}


void DynamicArray::swapHeadTail12(NodeDA * inter1, NodeDA * inter2, int startIndexLeaf1, int endIndexLeaf1,
                                  int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2 + 1 + inter1->NumOfKeys-startIndexLeaf1;
    int * Keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int  ik = 0;
    for (int i = startIndexLeaf2; i < inter1->NumOfKeys; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        Pointers[ik] = inter2->Pointers[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    int jk = 0;
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        inter1->Keys[i] = Keys[jk];
        inter1->Pointers[i] = Pointers[jk];
        jk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter2->Keys[i] = Keys[jk];
        inter2->Pointers[i] = Pointers[jk];
        jk++;
    }
    auto ** nodes = new NodeDA *[2];
    nodes[0] = inter1;
    nodes[1] = inter2;
    UpdateSumKeys(nodes, 2);
    delete []nodes;
    delete []Keys;
    delete []Pointers;
}


void DynamicArray::swapHeadTail21(NodeDA * inter1, NodeDA * inter2, int startIndexLeaf1, int endIndexLeaf1,
                                  int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2 + 1 + inter1->NumOfKeys-startIndexLeaf1;
    int * Keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int ik = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        Pointers[ik] = inter2->Pointers[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        Pointers[ik] = inter2->Pointers[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter2->Keys[i];
        Pointers[ik] = inter2->Pointers[i];
        ik++;
    }
    int jk = 0;
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        inter1->Keys[i] = Keys[jk];
        inter1->Pointers[i] = Pointers[jk];
        jk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter2->Keys[i] = Keys[jk];
        inter2->Pointers[i] = Pointers[jk];
        jk++;
    }
    auto ** nodes = new NodeDA *[2];
    nodes[0] = inter1;
    nodes[1] = inter2;
    UpdateSumKeys(nodes, 2);
    delete []nodes;
    delete []Keys;
    delete []Pointers;
}


void DynamicArray::swapHeadTail22(NodeDA * inter1, NodeDA * inter2, NodeDA * inter3, int startIndexLeaf1,
                                  int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1 + inter2->NumOfKeys + inter1->NumOfKeys-startIndexLeaf1;
    int * Keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int ik = 0;
    for (int i = startIndexLeaf2; i < inter2->NumOfKeys; i++) {
        Keys[ik] = inter2->Keys[i];
        Pointers[ik] = inter2->Pointers[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        Keys[ik] = inter3->Keys[i];
        Pointers[ik] = inter3->Pointers[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        Keys[ik] = inter2->Keys[i];
        Pointers[ik] = inter2->Pointers[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        Keys[ik] = inter1->Keys[i];
        Pointers[ik] = inter1->Pointers[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        Keys[ik] = inter2->Keys[i];
        Pointers[ik] = inter2->Pointers[i];
        ik++;
    }

    int jk = 0;
    for (int i = startIndexLeaf1; i < inter1->NumOfKeys; i++) {
        inter1->Keys[i] = Keys[jk];
        inter1->Pointers[i] = Pointers[jk];
        jk++;
    }
    for (int i = 0; i < inter2->NumOfKeys; i++) {
        inter2->Keys[i] = Keys[jk];
        inter2->Pointers[i] = Pointers[jk];
        jk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter3->Keys[i] = Keys[jk];
        inter3->Pointers[i] = Pointers[jk];
        jk++;
    }
    auto ** nodes = new NodeDA *[3];
    nodes[0] = inter1;
    nodes[1] = inter2;
    nodes[2] = inter3;
    UpdateSumKeys(nodes, 3);
    delete []nodes;
    delete []Keys;
    delete []Pointers;
}


void DynamicArray::swapHeadTailLen1Is1(NodeDA **inter2, int numLeaf2, int startIndexLeaf1,
                                       int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int NumToMove = startIndexLeaf2 - startIndexLeaf1;
    int* KeysToMove = new int[NumToMove];
    auto ** Pointers = new NodeDA *[NumToMove];
    int ik = 0;
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        Pointers[ik] = inter2[0]->Pointers[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        Pointers[ik] = inter2[0]->Pointers[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < inter2[0]->NumOfKeys-NumToMove; i++) {
        inter2[0]->Keys[i] = inter2[0]->Keys[i+NumToMove];
        inter2[0]->Pointers[i] = inter2[0]->Pointers[i+NumToMove];
    }
    inter2[0]->NumOfKeys -= NumToMove;
    CompleteAdjustAncestorNodesAfterDelete(inter2[0]);
    insertKeysIntoLeaf(KeysToMove, Pointers, NumToMove, inter2[numLeaf2-1], endIndexLeaf2 + 1);
    delete []KeysToMove;
    delete []Pointers;
}


void DynamicArray::swapHeadTailLen2Is1(NodeDA **inter1, int numLeaf1, int startIndexLeaf1,
                                       int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    NodeDA * n = inter1[numLeaf1-1];
    int NumToMove = endIndexLeaf2 - endIndexLeaf1;
    int* KeysToMove = new int[NumToMove];
    auto ** Pointers = new NodeDA *[NumToMove];
    int ik = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        KeysToMove[ik] = n->Keys[i];
        Pointers[ik] = n->Pointers[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = n->Keys[i];
        Pointers[ik] = n->Pointers[i];
        ik++;
    }
    for (int i = endIndexLeaf2 + 1; i < n->NumOfKeys; i++) {
        n->Keys[i-NumToMove] = n->Keys[i];
        n->Pointers[i-NumToMove] = n->Pointers[i];
    }
    n->NumOfKeys -= NumToMove;
    CompleteAdjustAncestorNodesAfterDelete(n);
    insertKeysIntoLeaf(KeysToMove, Pointers, NumToMove, inter1[0], startIndexLeaf1);
    delete []KeysToMove;
    delete []Pointers;
}


void DynamicArray::swapHeadTailLen1Is2(NodeDA **inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2,
                                       int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    NodeDA * n = inter1[0];
    int NumToMove = startIndexLeaf2 + inter1[0]->NumOfKeys-startIndexLeaf1;
    int* KeysToMove = new int[NumToMove];
    auto ** Pointers = new NodeDA *[NumToMove];
    int ik = 0;
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        Pointers[ik] = inter2[0]->Pointers[i];
        ik++;
    }
    for (int i = startIndexLeaf1; i < n->NumOfKeys; i++) {
        KeysToMove[ik] = n->Keys[i];
        Pointers[ik] = n->Pointers[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        KeysToMove[ik] = inter2[0]->Keys[i];
        Pointers[ik] = inter2[0]->Pointers[i];
        ik++;
    }

    //delete range 1
    DeleteMidPartOfTwoAdjacentLeaves(inter1[0], startIndexLeaf1, inter1[1], startIndexLeaf2-1);
    //insert
    NodeDA * y = inter2[numLeaf2-1];
    insertKeysIntoLeaf(KeysToMove, Pointers, NumToMove, y, endIndexLeaf2 + 1);
    delete []KeysToMove;
    delete []Pointers;
}


void DynamicArray::swapHeadTailLen2Is2(NodeDA **inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2,
                                       int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    NodeDA * n = inter2[0];
    int NumToMove = endIndexLeaf2 + 1 + inter2[0]->NumOfKeys-endIndexLeaf1-1;
    auto ** Pointers = new NodeDA *[NumToMove];
    int* KeysToMove = new int[NumToMove];
    int ik = 0;
    for (int i = startIndexLeaf2; i < n->NumOfKeys; i++) {
        KeysToMove[ik] = n->Keys[i];
        Pointers[ik] = n->Pointers[i];
        ik++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        KeysToMove[ik] = inter2[1]->Keys[i];
        Pointers[ik] = inter2[1]->Pointers[i];
        ik++;
    }
    for (int i = endIndexLeaf1 + 1; i < startIndexLeaf2; i++) {
        KeysToMove[ik] = n->Keys[i];
        Pointers[ik] = n->Pointers[i];
        ik++;
    }
    //delete range 2
    DeleteMidPartOfTwoAdjacentLeaves(inter2[0], endIndexLeaf1+1, inter2[1], endIndexLeaf2);

    //insert
    NodeDA * y = inter1[0];
    insertKeysIntoLeaf(KeysToMove, Pointers, NumToMove, y, startIndexLeaf1);
    delete []Pointers;
    delete []KeysToMove;
}



void DynamicArray::SwapRangesAdjacentWhenLensAreBoth2(NodeDA **inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2,
                                                      int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  inter1[0]->NumOfKeys-startIndexLeaf1 + inter1[1]->NumOfKeys + inter2[0]->NumOfKeys + endIndexLeaf2+1;
    int * keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; i++) {
        keys[tk] = inter2[0]->Keys[i];
        Pointers[tk] = inter2[0]->Pointers[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        keys[tk] = inter2[1]->Keys[i];
        Pointers[tk] = inter2[1]->Pointers[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < inter1[1]->NumOfKeys; i++) {
        keys[tk] = inter1[1]->Keys[i];
        Pointers[tk] = inter1[1]->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        Pointers[tk] = inter2[0]->Pointers[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; i++) {
        keys[tk] = inter1[0]->Keys[i];
        Pointers[tk] = inter1[0]->Pointers[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        keys[tk] = inter1[1]->Keys[i];
        Pointers[tk] = inter1[1]->Pointers[i];
        tk++;
    }
    tk = 0;
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; i++) {
        inter1[0]->Keys[i] = keys[tk];
        inter1[0]->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i < inter1[1]->NumOfKeys; i++) {
        inter1[1]->Keys[i] = keys[tk];
        inter1[1]->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i < inter2[0]->NumOfKeys; i++) {
        inter2[0]->Keys[i] = keys[tk];
        inter2[0]->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        inter2[1]->Keys[i] = keys[tk];
        inter2[1]->Pointers[i] = Pointers[tk];
        tk++;
    }
    auto ** nodes = new NodeDA *[4];
    nodes[0] = inter1[0];
    nodes[1] = inter1[1];
    nodes[2] = inter2[0];
    nodes[3] = inter2[1];
    UpdateSumKeys(nodes, 4);
    delete []nodes;
    delete []Pointers;
    delete []keys;
}

//starting start is two adjacent leaves
// in this function, we delete the end of first leaf and start of the second leaf
// then adjust these two leaves and their ancestors
NodeDA * DynamicArray::DeleteMidPartOfTwoAdjacentLeaves(NodeDA * leaf1, int startIndexInLeaf1, NodeDA * leaf2, int endIndexInLeaf2) {
    leaf1->NumOfKeys = startIndexInLeaf1;
    movePointerInNodeForward(leaf2, endIndexInLeaf2+1, leaf2->NumOfKeys-1, 0);
    leaf2->NumOfKeys -= endIndexInLeaf2 + 1;
    if (leaf1->NumOfKeys < Min) { // leaf1 < Min
        NodeDA * neighbour,* newl;
        int neighbour_index = getLeftNeighborIndex(leaf1);
        if (neighbour_index == -1) { // leaf1 index is 0
            neighbour = leaf1->Parent->Pointers[1];
            if (neighbour->NumOfKeys + leaf1->NumOfKeys <= Capacity) { // leaf1 and leaf2 coalesce
                newl = coalesceNodes(leaf1, neighbour, neighbour_index);
                if (newl->NumOfKeys < Min) {
                    int nei_index = getLeftNeighborIndex(newl);
                    NodeDA * nei;
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
            NodeDA * re = nullptr;
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
void DynamicArray::SwapRangesAdjacentRightIs2(NodeDA **inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2,
                                              int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2 +1 + inter2[0]->NumOfKeys + inter1[numLeaf1-1]->NumOfKeys - endIndexLeaf1-1;
    int * keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; i++) {
        keys[tk] = inter2[0]->Keys[i];
        Pointers[tk] = inter2[0]->Pointers[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        keys[tk] = inter2[1]->Keys[i];
        Pointers[tk] = inter2[1]->Pointers[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < inter1[numLeaf1-1]->NumOfKeys; i++) {
        keys[tk] = inter1[numLeaf1-1]->Keys[i];
        Pointers[tk] = inter1[numLeaf1-1]->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        Pointers[tk] = inter2[0]->Pointers[i];
        tk++;
    }
    //delete
    NodeDA * pre = inter1[numLeaf1-1]; // findPrevious(inter2[0]);
    pre->Next = inter2[1]; //->Next;
    /*
    if (inter2[0]->Next != nullptr) {
        inter2[0]->Next->Previous = inter2[0]->Previous;
    }
     */
    inter2[0]->NumOfKeys=0;
    deleteEntry(inter2[0]->Parent, getIndexInParent(inter2[0]));

    DeleteMidPartOfTwoAdjacentLeaves(inter1[numLeaf1-1], endIndexLeaf1 + 1, inter2[1], endIndexLeaf2);
    insertKeysIntoLeaf(keys, Pointers, num, inter1[0], startIndexLeaf1);

    delete []keys;
    delete []Pointers;
}


void DynamicArray::SwapRangesAdjacentLeftIs2(NodeDA **inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2,
                                             int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  startIndexLeaf2 + inter1[1]->NumOfKeys + inter1[0]->NumOfKeys - startIndexLeaf1;
    int * keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int tk = 0;
    for (int i = endIndexLeaf1 + 1; i < inter1[1]->NumOfKeys; i++) {
        keys[tk] = inter1[1]->Keys[i];
        Pointers[tk] = inter1[1]->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        Pointers[tk] = inter2[0]->Pointers[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; i++) {
        keys[tk] = inter1[0]->Keys[i];
        Pointers[tk] = inter1[0]->Pointers[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        keys[tk] = inter1[1]->Keys[i];
        Pointers[tk] = inter1[1]->Pointers[i];
        tk++;
    }
    //delete
    auto ** LeafToDelete = new NodeDA *[1];
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

    insertKeysIntoLeaf(keys, Pointers, num, inter2[numLeaf2-1], endIndexLeaf2+1);
    delete []Pointers;
    delete []keys;
}



void DynamicArray::SWapRangesAdjacentWhenLensAreBoth1(NodeDA * leftnode, NodeDA * rightnode, int startIndexLeaf1,
                                                      int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1+leftnode->NumOfKeys-startIndexLeaf1;
    int * keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        keys[tk] = rightnode->Keys[i];
        Pointers[tk] = rightnode->Pointers[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < leftnode->NumOfKeys; i++) {
        keys[tk] = leftnode->Keys[i];
        Pointers[tk] = leftnode->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = rightnode->Keys[i];
        Pointers[tk] = rightnode->Pointers[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        keys[tk] = leftnode->Keys[i];
        Pointers[tk] = leftnode->Pointers[i];
        tk++;
    }
    tk = 0;
    for (int i = startIndexLeaf1; i < leftnode->NumOfKeys; i++) {
        leftnode->Keys[i] = keys[tk];
        leftnode->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        rightnode->Keys[i] = keys[tk];
        rightnode->Pointers[i] = Pointers[tk];
        tk++;
    }
    auto ** ns = new NodeDA *[2];
    ns[0] = leftnode;
    ns[1] = rightnode;
    UpdateSumKeys(ns, 2);
    delete []ns;
    delete []Pointers;
    delete []keys;
}


void DynamicArray::SWapRangesAdjacentWhenLensAre1And2(NodeDA * leftnode, NodeDA * right1, NodeDA * right2,
                                                      int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1+right1->NumOfKeys+leftnode->NumOfKeys-startIndexLeaf1;
    int * keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
        keys[tk] = right1->Keys[i];
        Pointers[tk] = right1->Pointers[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        keys[tk] = right2->Keys[i];
        Pointers[tk] = right2->Pointers[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < leftnode->NumOfKeys; i++) {
        keys[tk] = leftnode->Keys[i];
        Pointers[tk] = leftnode->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = right1->Keys[i];
        Pointers[tk] = right1->Pointers[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        keys[tk] = leftnode->Keys[i];
        Pointers[tk] = leftnode->Pointers[i];
        tk++;
    }

    tk = 0;
    for (int i = startIndexLeaf1; i < leftnode->NumOfKeys; i++) {
        leftnode->Keys[i] = keys[tk];
        leftnode->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i < right1->NumOfKeys; i++) {
        right1->Keys[i] = keys[tk];
        right1->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        right2->Keys[i] = keys[tk];
        right2->Pointers[i] = Pointers[tk];
        tk++;
    }
    auto ** ns = new NodeDA *[3];
    ns[0] = leftnode;
    ns[1] = right1;
    ns[2] = right2;
    UpdateSumKeys(ns, 3);
    delete []ns;
    delete []keys;
    delete []Pointers;
}


void DynamicArray::SwapRangesAdjacentWhenLensAre2And1(NodeDA * left1, NodeDA * left2, NodeDA * right,
                                                      int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int num =  endIndexLeaf2+1 + left2->NumOfKeys + left1->NumOfKeys-startIndexLeaf1;
    int * keys = new int[num];
    auto ** Pointers = new NodeDA *[num];
    int tk = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        Pointers[tk] = right->Pointers[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < left2->NumOfKeys; i++) {
        keys[tk] = left2->Keys[i];
        Pointers[tk] = left2->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        Pointers[tk] = right->Pointers[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
        keys[tk] = left1->Keys[i];
        Pointers[tk] = left1->Pointers[i];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf1; i++) {
        keys[tk] = left2->Keys[i];
        Pointers[tk] = left2->Pointers[i];
        tk++;
    }

    tk = 0;
    for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
        left1->Keys[i] = keys[tk];
        left1->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i < left2->NumOfKeys; i++) {
        left2->Keys[i] = keys[tk];
        left2->Pointers[i] = Pointers[tk];
        tk++;
    }
    for (int i = 0; i <= endIndexLeaf2; i++) {
        right->Keys[i] = keys[tk];
        right->Pointers[i] = Pointers[tk];
        tk++;
    }

    auto ** ns = new NodeDA *[3];
    ns[0] = left1;
    ns[1] = left2;
    ns[2] = right;
    UpdateSumKeys(ns, 3);
    delete []ns;
    delete []keys;
    delete []Pointers;

}


void DynamicArray::SwapRangesAdjacentWhenLeftIs1(NodeDA * left, NodeDA ** inter2, int numLeaf2,
                                                 int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int NumToMove = startIndexLeaf2 + left->NumOfKeys - startIndexLeaf1;
    int * keys = new int[NumToMove];
    auto ** Pointers = new NodeDA *[NumToMove];
    int tk = 0;
    for (int i = endIndexLeaf1 + 1; i < left->NumOfKeys; i++) {
        keys[tk] = left->Keys[i];
        Pointers[tk] = left->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = inter2[0]->Keys[i];
        Pointers[tk] = inter2[0]->Pointers[i];
        tk++;
    }
    for (int i = startIndexLeaf1; i <= endIndexLeaf1; i++) {
        keys[tk] = left->Keys[i];
        Pointers[tk] = left->Pointers[i];
        tk++;
    }
    //delete
    DeleteMidPartOfTwoAdjacentLeaves(left, startIndexLeaf1, inter2[0], startIndexLeaf2-1);
    //insert
    insertKeysIntoLeaf(keys, Pointers, NumToMove, inter2[numLeaf2 - 1], endIndexLeaf2 + 1);
    delete []keys;
    delete []Pointers;
}



void DynamicArray::SwapRangesAdjacentWhenRightIs1(NodeDA **inter1, int numLeaf1, NodeDA * right,
                                                  int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int numToMove = endIndexLeaf2+1 + inter1[numLeaf1-1]->NumOfKeys - endIndexLeaf1-1;
    int * keys = new int[numToMove];
    auto ** Pointers = new NodeDA *[numToMove];
    int tk = 0;
    for (int i = startIndexLeaf2; i <= endIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        Pointers[tk] = right->Pointers[i];
        tk++;
    }
    for (int i = endIndexLeaf1 + 1; i < inter1[numLeaf1-1]->NumOfKeys; i++) {
        keys[tk] = inter1[numLeaf1-1]->Keys[i];
        Pointers[tk] = inter1[numLeaf1-1]->Pointers[i];
        tk++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        keys[tk] = right->Keys[i];
        Pointers[tk] = right->Pointers[i];
        tk++;
    }
    //delete
    DeleteMidPartOfTwoAdjacentLeaves(inter1[numLeaf1-1], endIndexLeaf1 + 1, right, endIndexLeaf2);

    //insert
    insertKeysIntoLeaf(keys, Pointers, numToMove, inter1[0], startIndexLeaf1);
    delete []keys;
    delete []Pointers;
}


//to be ckeched
void DynamicArray::SwapRangesAdjacent(NodeDA ** inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2,
                                      int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2){
    if (numLeaf1 == 1 && numLeaf2 == 1) {
        SWapRangesAdjacentWhenLensAreBoth1(inter1[0], inter2[0], startIndexLeaf1, endIndexLeaf1,
                                           startIndexLeaf2, endIndexLeaf2);
        return;
    } else if (numLeaf1 == 1 && numLeaf2 == 2) {
        SWapRangesAdjacentWhenLensAre1And2(inter1[0], inter2[0], inter2[1], startIndexLeaf1, endIndexLeaf1,
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
    auto ** PointerToMove1 = new NodeDA *[numToMove1];
    int i1 = 0;
    for (int i = 0; i <= endIndexLeaf1; i++) {
        KeyToMove1[i1] = inter1[numLeaf1-1]->Keys[i];
        PointerToMove1[i1] = inter1[numLeaf1-1]->Pointers[i];
        i1++;
    }
    for (int i = endIndexLeaf2 + 1; i < inter2[numLeaf2-1]->NumOfKeys; i++) {
        KeyToMove1[i1] = inter2[numLeaf2-1]->Keys[i];
        PointerToMove1[i1] = inter2[numLeaf2-1]->Pointers[i];
        i1++;
    }

    int numToMove2 = startIndexLeaf1 + inter2[0]->NumOfKeys - startIndexLeaf2;
    int * KeyToMove2 = new int[numToMove2];
    auto ** PointerToMove2 = new NodeDA *[numToMove2];
    int i2 = 0;
    for (int i = 0; i < startIndexLeaf1; i++) {
        KeyToMove2[i2] = inter1[0]->Keys[i];
        PointerToMove2[i2] = inter1[0]->Pointers[i];
        i2++;
    }
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; i++) {
        KeyToMove2[i2] = inter2[0]->Keys[i];
        PointerToMove2[i2] = inter2[0]->Pointers[i];
        i2++;
    }

    //insert for range 1
    insertKeysIntoLeaf(KeyToMove1, PointerToMove1, numToMove1, inter1[numLeaf1-2], inter1[numLeaf1 - 2]->NumOfKeys);

    //insert for range 2
    bool changePointer = insertKeysIntoLeaf(KeyToMove2, PointerToMove2, numToMove2, inter2[1], 0);
    if (changePointer) {
        inter2[1] = inter2[1]->Next;
    }

    //middle pointers
    int NumMiddlePointers = (inter1[numLeaf1-1]->NumOfKeys - endIndexLeaf1 - 1) + (startIndexLeaf2);
    int * Middlekeys = new int[NumMiddlePointers];
    auto ** Middlepointers = new NodeDA *[NumMiddlePointers];
    int mi = 0;
    for (int i = endIndexLeaf1 + 1; i < inter1[numLeaf1-1]->NumOfKeys; i++) {
        Middlekeys[mi] = inter1[numLeaf1-1]->Keys[i];
        Middlepointers[mi] = inter1[numLeaf1-1]->Pointers[i];
        mi++;
    }
    for (int i = 0; i < startIndexLeaf2; i++) {
        Middlekeys[mi] = inter2[0]->Keys[i];
        Middlepointers[mi] = inter2[0]->Pointers[i];
        mi++;
    }

    //delete two leaves in the middle
    inter1[numLeaf1-1]->NumOfKeys=0;
    inter2[0]->NumOfKeys=0;
    auto ** ToDelete = new NodeDA *[2];
    ToDelete[0] = inter1[numLeaf1-1];
    ToDelete[1] = inter2[0];

    NodeDA * pre = inter2[numLeaf2-1];// inter1[numLeaf1-2];
    if (pre != nullptr) {
        pre->Next = inter2[0]->Next;
        /*
        if (inter2[0]->Next != nullptr) {
            inter2[0]->Next->Previous = inter1[numLeaf1-1]->Previous;
        }

    } else {
        if (inter2[0]->Next != nullptr) {
            inter2[0]->Next->Previous = inter1[numLeaf1-1]->Previous;
        }
         */
    }

    NodeDA * preLeaf = deleteMulEntry(ToDelete, 2);
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, ToDelete, 2);
    ReplaceMidOfTwoAdjacentLeaves(inter2[numLeaf2-1], inter1[0], endIndexLeaf2+1, startIndexLeaf1-1,
                                  Middlekeys, Middlepointers, NumMiddlePointers);
    delete []PointerToMove1;
    delete []PointerToMove2;
    delete []KeyToMove1;
    delete []KeyToMove2;
    delete []Middlekeys;
    delete []Middlepointers;
    delete []ToDelete;
}

//return true if toIndex == 0 and insertMulKeysIntoLeafAfterOneSplitting returns true
bool DynamicArray::insertKeysIntoLeaf(int * keysToInsert, NodeDA ** pointersToInsert,
                                      int numKeysToInsert, NodeDA * toLeaf, int toIndex) {
    if (toLeaf->NumOfKeys + numKeysToInsert <= Capacity) {
        insertMulKeysIntoLeaf(keysToInsert, pointersToInsert, numKeysToInsert, toLeaf, toIndex);
        return false;
    } else if (toLeaf->NumOfKeys + numKeysToInsert <= 2*Capacity) {
        return insertMulKeysIntoLeafAfterOneSplitting(keysToInsert, pointersToInsert, numKeysToInsert, toLeaf, toIndex);
    } else {
        insertMulKeysIntoLeafAfterMulSplitting(keysToInsert, pointersToInsert, numKeysToInsert, toLeaf, toIndex);
        return false;
    }
}

//split maybe more than once
void DynamicArray::insertMulKeysIntoLeafAfterMulSplitting(const int * keysToInsert, NodeDA ** pointersToInsert,
                                                          int numKeysToInsert, NodeDA * toLeaf, int toIndex) {
    int totalNumKeys = numKeysToInsert + toLeaf->NumOfKeys;
    int * newKeys = new int[totalNumKeys];
    auto ** newPointers = new NodeDA *[totalNumKeys];
    int i = 0;
    for (int j = 0; j < toIndex; j++) {
        newKeys[i] = toLeaf->Keys[j];
        newPointers[i] = toLeaf->Pointers[j];
        i++;
    }
    for (int j = 0; j < numKeysToInsert; j++) {
        newKeys[i] = keysToInsert[j];
        newPointers[i] = pointersToInsert[j];
        i++;
    }
    for (int j = toIndex; j < toLeaf->NumOfKeys; j++) {
        newKeys[i] = toLeaf->Keys[j];
        newPointers[i] = toLeaf->Pointers[j];
        i++;
    }
    toLeaf->NumOfKeys = 0;
    int numFullParent = totalNumKeys / Capacity;
    int remainNumLeaf = totalNumKeys - numFullParent*Capacity;
    if (remainNumLeaf == 0 || remainNumLeaf >= Min) { // full nodes + remainnumleaf
        int ik = 0;
        for (; ik < Capacity; ik++) {
            toLeaf->Keys[ik] = newKeys[ik];
            toLeaf->Pointers[ik] = newPointers[ik];
        }
        toLeaf->NumOfKeys = Capacity;
        int parentNewSiblingNum = numFullParent - 1;
        if (remainNumLeaf != 0) {
            parentNewSiblingNum++;
        }
        auto ** parentNewSibling = new NodeDA *[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = makeLeafonedp(Capacity);
            for (int y = 0; y < Capacity; y++) {
                parentNewSibling[k]->Keys[y] = newKeys[ik]; //allPointers[i];->NumOfKeys
                parentNewSibling[k]->Pointers[y] = newPointers[ik]; //allPointers[i];->NumOfKeys
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

        NodeDA * pa = toLeaf->Parent;
        int startindex = getIndexInParent(toLeaf)+1;
        pa->Keys[startindex-1] = NodeSumKeys(toLeaf);
        insertLeavesIntoLeafParent(pa, startindex, parentNewSibling, parentNewSiblingNum);
        delete []newKeys;
        delete []newPointers;
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
                toLeaf->Pointers[y] = newPointers[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                i_allpointer++;
            }
            toLeaf->NumOfKeys = numkeysInSecondLastParent;
        } else {
            for (int y = 0; y < Capacity; y++) {
                toLeaf->Keys[y] = newKeys[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                toLeaf->Pointers[y] = newPointers[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                i_allpointer++;
            }
            toLeaf->NumOfKeys = Capacity;
        }
        //set parent's new siblings
        auto ** parentNewSibling  = new NodeDA *[parentNewSiblingNum];
        for (int k = 0; k < parentNewSiblingNum; k++) {
            parentNewSibling[k] = makeLeafonedp(Capacity);
            if (k == parentNewSiblingNum-1) { // the last one
                for (int y = 0; y < numkeysInLastParent; y++) {
                    parentNewSibling[k]->Keys[y] = newKeys[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                    parentNewSibling[k]->Pointers[y] = newPointers[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                    i_allpointer++;
                }
                parentNewSibling[k]->NumOfKeys = numkeysInLastParent;
            } else if (k == parentNewSiblingNum-2) { // the second last one
                for (int y = 0; y < numkeysInSecondLastParent; y++) {
                    parentNewSibling[k]->Keys[y] = newKeys[i_allpointer];//allPointers[i_allpointer]->NumOfKeys
                    parentNewSibling[k]->Pointers[y] = newPointers[i_allpointer];//allPointers[i_allpointer]->NumOfKeys
                    i_allpointer++;
                }
                parentNewSibling[k]->NumOfKeys = numkeysInSecondLastParent;
            } else {
                for (int y = 0; y < Capacity; y++) {
                    parentNewSibling[k]->Keys[y] = newKeys[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                    parentNewSibling[k]->Pointers[y] = newPointers[i_allpointer]; //allPointers[i_allpointer]->NumOfKeys
                    i_allpointer++;
                }
                parentNewSibling[k]->NumOfKeys = Capacity;
            }
        }
        for (int p = 0; p < parentNewSiblingNum-1; p++) {
            parentNewSibling[p]->Next = parentNewSibling[p+1];
            //parentNewSibling[p+1]->Previous = parentNewSibling[p];
        }

        NodeDA ** paNode = &parentNewSibling[0];
        NodeDA * pa = toLeaf->Parent;
        int startindex = getIndexInParent(toLeaf)+1;
        pa->Keys[startindex-1] = NodeSumKeys(toLeaf);
        insertLeavesIntoLeafParent(pa, startindex, paNode, parentNewSiblingNum);
        delete []newKeys;
        delete []newPointers;
        delete []parentNewSibling;
    }
}

void DynamicArray::SwapWhenLeftLenIs2(NodeDA * left1, NodeDA * left2, int startIndexLeaf1, int endIndexLeaf1,
                                      NodeDA ** inter2, int numLeaf2, int startIndexLeaf2, int endIndexLeaf2) {
    //delete leaves
    inter2[0]->Next = inter2[numLeaf2-1];
    //inter2[numLeaf2-1]->Previous = inter2[0];

    //only delete entries, no redistribution or merging
    NodeDA * preLeaf = deleteMulEntry(&inter2[1], numLeaf2-2);
    // adjust keys in ancestors
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, &inter2[1], numLeaf2-2);
    //insert leaves
    NodeDA * toParent = left1->Parent;
    int insertIndexInToParent = getIndexInParent(left1) + 1;
    insertLeavesIntoLeafParent(toParent, insertIndexInToParent, &inter2[1], numLeaf2-2);

    //insert keys at the end of inter2[0] to the beginning of inter2[1], without deleting these keys
    int numToInsert20 = inter2[0]->NumOfKeys - startIndexLeaf2;
    auto ** pointersToInsert20 = new NodeDA *[numToInsert20];
    int * keysToInset20 = new int[numToInsert20];
    for (int i = startIndexLeaf2; i < inter2[0]->NumOfKeys; ++i) {
        pointersToInsert20[i-startIndexLeaf2] = inter2[0]->Pointers[i];
        keysToInset20[i-startIndexLeaf2] = inter2[0]->Keys[i];
    }
    bool changePointer = insertKeysIntoLeaf(keysToInset20, pointersToInsert20, numToInsert20, inter2[1], 0);
    if (changePointer) {
        inter2[1] = inter2[1]->Next;
    }

    //insert keys at the beginning of inter2[numLeaf2-1] to the end of inter2[numLeaf2-2], without deleting these keys
    int numToInsert22 = endIndexLeaf2 + 1;
    auto ** pointersToInsert22 = new NodeDA *[numToInsert22];
    int * keysToInset22 = new int[numToInsert22];
    for (int j = 0; j <= endIndexLeaf2; ++j) {
        pointersToInsert22[j] = inter2[numLeaf2-1]->Pointers[j];
        keysToInset22[j] = inter2[numLeaf2-1]->Keys[j];
    }
    insertKeysIntoLeaf(keysToInset22, pointersToInsert22, numToInsert22, inter2[numLeaf2-2], inter2[numLeaf2-2]->NumOfKeys);

    //keys and pointers of left1, left2
    int numToMoveInLeft = left1->NumOfKeys - startIndexLeaf1 + endIndexLeaf1 + 1;
    auto ** pointersToMoveInLeft = new NodeDA *[numToMoveInLeft];
    int * keysToMoveInLeft = new int[numToMoveInLeft];
    int idxToMoveInLeft = 0;
    for (int i = startIndexLeaf1; i < left1->NumOfKeys; ++i) {
        pointersToMoveInLeft[idxToMoveInLeft] = left1->Pointers[i];
        keysToMoveInLeft[idxToMoveInLeft] = left1->Keys[i];
        idxToMoveInLeft++;
    }
    for (int i = 0; i <= endIndexLeaf1; ++i) {
        pointersToMoveInLeft[idxToMoveInLeft] = left2->Pointers[i];
        keysToMoveInLeft[idxToMoveInLeft] = left2->Keys[i];
        idxToMoveInLeft++;
    }

    //replace keys in inter2[0], inter2[numLeaf2-1] with pointersToMoveInLeft, keysToMoveInLeft
    ReplaceMidOfTwoAdjacentLeaves(inter2[0], inter2[numLeaf2-1], startIndexLeaf2, endIndexLeaf2,
                                  keysToMoveInLeft, pointersToMoveInLeft, numToMoveInLeft);
    //delete end keys in left
    left1->NumOfKeys = startIndexLeaf1;
    CompleteAdjustAncestorNodesAfterDelete(left1);
    //delete start keys in right
    movePointerInNodeForward(left2, endIndexLeaf1+1, left2->NumOfKeys-1, 0);
    left2->NumOfKeys -= endIndexLeaf1 + 1;
    CompleteAdjustAncestorNodesAfterDelete(left2);
    delete []pointersToInsert20;
    delete []pointersToInsert22;
    delete []keysToInset20;
    delete []keysToInset22;
    delete []keysToMoveInLeft;
    delete []pointersToMoveInLeft;
}

//replace the mid part of two adjacent leaves and go upwards
//without deleting anything
void DynamicArray::ReplaceMidOfTwoAdjacentLeaves(NodeDA * left, NodeDA * right, int startIndexLeft, int endIndexRight,
                                                 int * keysToInsert, NodeDA ** pointersToInsert, int numToInsert) {
    int numToBeReplaced = left->NumOfKeys - startIndexLeft + endIndexRight + 1;
    int idxToMoveInLeft = 0;
    if (numToBeReplaced == numToInsert) {
        for (int i = startIndexLeft; i < left->NumOfKeys; ++i) {
            left->Pointers[i] = pointersToInsert[idxToMoveInLeft];
            left->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        for (int i = 0; i <= endIndexRight; ++i) {
            right->Pointers[i] = pointersToInsert[idxToMoveInLeft];
            right->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        AdjustAncestorKeysAccordingToCurNode(left);
        AdjustAncestorKeysAccordingToCurNode(right);
        return;
    } else if (numToBeReplaced < numToInsert) {
        for (int i = startIndexLeft; i < left->NumOfKeys; ++i) {
            left->Pointers[i] = pointersToInsert[idxToMoveInLeft];
            left->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        for (int i = 0; i <= endIndexRight; ++i) {
            right->Pointers[i] = pointersToInsert[idxToMoveInLeft];
            right->Keys[i] = keysToInsert[idxToMoveInLeft];
            idxToMoveInLeft++;
        }
        AdjustAncestorKeysAccordingToCurNode(left);
        insertKeysIntoLeaf(&keysToInsert[numToBeReplaced], &pointersToInsert[numToBeReplaced],
                           numToInsert - numToBeReplaced, right, endIndexRight+1);
        return;
    } else {// numToBeReplaced > numToInsert
        int numToBeReplaced1 = left->NumOfKeys - startIndexLeft;
        if (numToInsert < numToBeReplaced1) {
            int li = startIndexLeft;
            for (; idxToMoveInLeft < numToInsert; ++idxToMoveInLeft) {
                left->Pointers[li] = pointersToInsert[idxToMoveInLeft];
                left->Keys[li] = keysToInsert[idxToMoveInLeft];
                li++;
            }
            DeleteMidPartOfTwoAdjacentLeaves(left, li, right, endIndexRight);
            return;
        } else if (numToInsert == numToBeReplaced1) {
            for (int i = startIndexLeft; i < left->NumOfKeys; ++i) {
                left->Pointers[i] = pointersToInsert[idxToMoveInLeft];
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
                left->Pointers[i] = pointersToInsert[idxToMoveInLeft];
                left->Keys[i] = keysToInsert[idxToMoveInLeft];
                idxToMoveInLeft++;
            }
            int ri = 0;
            for (; idxToMoveInLeft < numToInsert; ++idxToMoveInLeft) {
                right->Pointers[ri] = pointersToInsert[idxToMoveInLeft];
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


void DynamicArray::SwapWhenBothLensAre2(NodeDA * left1, NodeDA * left2, int startIndexLeaf1, int endIndexLeaf1,
                                        NodeDA * right1, NodeDA * right2, int startIndexLeaf2, int endIndexLeaf2) {
    int len11 = left1->NumOfKeys - startIndexLeaf1;
    int len12 = endIndexLeaf1+1;
    int len21 = right1->NumOfKeys - startIndexLeaf2;
    int len22 = endIndexLeaf2+1;
    int len1 = len11+len12;
    int len2 = len21+len22;

    if (len1 < len21) {
        int * keys1 = new int[len1];
        auto ** pointers1 = new NodeDA *[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            pointers1[ikeys1]=left1->Pointers[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            pointers1[ikeys1]=left2->Pointers[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            NodeDA * tmpp = right1->Pointers[i];
            right1->Pointers[i] = pointers1[i2];
            pointers1[i2] = tmpp;
            i2++;
            if (i2 == len1) {
                break;
            }
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            left1->Pointers[i] = pointers1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            left2->Pointers[i] = pointers1[i2];
            i2++;
        }
        int newstart2 = startIndexLeaf2+len1;
        int numKeysToMove = len2-len1;
        int * keysToMove = new int[numKeysToMove];
        auto ** pointersToMove = new NodeDA *[numKeysToMove];
        int ik = 0;
        for (int i = newstart2; i < right1->NumOfKeys; i++) {
            keysToMove[ik] = right1->Keys[i];
            pointersToMove[ik] = right1->Pointers[i];
            ik++;
        }
        for (int i = 0; i <= endIndexLeaf2; i++) {
            keysToMove[ik] = right2->Keys[i];
            pointersToMove[ik] = right2->Pointers[i];
            ik++;
        }
        insertKeysIntoLeaf(keysToMove, pointersToMove, numKeysToMove, left2, endIndexLeaf1 + 1);
        AdjustAncestorKeysAccordingToCurNode(left1);
        DeleteMidPartOfTwoAdjacentLeaves(right1, startIndexLeaf2 + len1, right2, endIndexLeaf2);
        delete []keys1;
        delete []pointers1;
        delete []keysToMove;
        delete []pointersToMove;
        return;
    } else if (len1 == len21) {
        int * keys1 = new int[len1];
        auto ** pointers1 = new NodeDA *[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            pointers1[ikeys1]=left1->Pointers[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            pointers1[ikeys1]=left2->Pointers[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int  tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            NodeDA * tmpp = right1->Pointers[i];
            right1->Pointers[i] = pointers1[i2];
            pointers1[i2] = tmpp;
            i2++;
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            left1->Pointers[i] = pointers1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            left2->Pointers[i] = pointers1[i2];
            i2++;
        }
        int numKeysToMove = len22;
        int * keysToMove = new int[numKeysToMove];
        auto ** pointersToMove = new NodeDA *[numKeysToMove];
        int ik = 0;
        for (int i = 0; i <= endIndexLeaf2; i++) {
            keysToMove[ik] = right2->Keys[i];
            pointersToMove[ik] = right2->Pointers[i];
            ik++;
        }
        insertKeysIntoLeaf(keysToMove, pointersToMove, numKeysToMove, left2, endIndexLeaf1 + 1);
        AdjustAncestorKeysAccordingToCurNode(left1);
        movePointerInNodeForward(right2, endIndexLeaf2+1, right2->NumOfKeys-1, 0);
        right2->NumOfKeys -= len22;
        CompleteAdjustAncestorNodesAfterDelete(right2);
        AdjustAncestorKeysAccordingToCurNode(right1);
        delete []keys1;
        delete []pointers1;
        delete []keysToMove;
        delete []pointersToMove;
        return;
    } else if (len1 < len2) { // len2 > len1 > len21
        int * keys1 = new int[len1];
        auto ** pointers1 = new NodeDA *[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            pointers1[ikeys1]=left1->Pointers[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            pointers1[ikeys1]=left2->Pointers[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;

            NodeDA * tmpp = right1->Pointers[i];
            right1->Pointers[i] = pointers1[i2];
            pointers1[i2] = tmpp;
            i2++;
        }
        int newstart2 = 0;
        for (int i = 0; i <= endIndexLeaf2; i++) {
            int tmp = right2->Keys[i];
            right2->Keys[i] = keys1[i2];
            keys1[i2] = tmp;

            NodeDA * tmpp = right2->Pointers[i];
            right2->Pointers[i] = pointers1[i2];
            pointers1[i2] = tmpp;
            i2++;
            if (i2 == len1) {
                newstart2 = i+1;
                break;
            }
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            left1->Pointers[i] = pointers1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            left2->Pointers[i] = pointers1[i2];
            i2++;
        }
        int numKeysToMove = len2 - len1;
        int * keysToMove = new int[numKeysToMove];
        auto ** pointersToMove = new NodeDA *[numKeysToMove];
        int ik = 0;
        for (int i = newstart2; i <= endIndexLeaf2; i++) {
            keysToMove[ik] = right2->Keys[i];
            pointersToMove[ik] = right2->Pointers[i];
            ik++;
        }
        AdjustAncestorKeysAccordingToCurNode(left1);
        insertKeysIntoLeaf(keysToMove, pointersToMove, numKeysToMove, left2, endIndexLeaf1 + 1);
        AdjustAncestorKeysAccordingToCurNode(right1);
        movePointerInNodeForward(right2, endIndexLeaf2+1, right2->NumOfKeys-1, newstart2);
        right2->NumOfKeys -= numKeysToMove;
        CompleteAdjustAncestorNodesAfterDelete(right2);
        delete []keys1;
        delete []pointers1;
        delete []keysToMove;
        delete []pointersToMove;
        return;
    } else if (len1 == len2) {
        int * keys1 = new int[len1];
        auto ** pointers1 = new NodeDA *[len1];
        int ikeys1 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            keys1[ikeys1]=left1->Keys[i];
            pointers1[ikeys1]=left1->Pointers[i];
            ikeys1++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            keys1[ikeys1]=left2->Keys[i];
            pointers1[ikeys1]=left2->Pointers[i];
            ikeys1++;
        }
        int i2 = 0;
        for (int i = startIndexLeaf2; i < right1->NumOfKeys; i++) {
            int tmp = right1->Keys[i];
            right1->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            NodeDA * tmpp = right1->Pointers[i];
            right1->Pointers[i] = pointers1[i2];
            pointers1[i2] = tmpp;
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf2; i++) {
            int tmp = right2->Keys[i];
            right2->Keys[i] = keys1[i2];
            keys1[i2] = tmp;
            NodeDA * tmpp = right2->Pointers[i];
            right2->Pointers[i] = pointers1[i2];
            pointers1[i2] = tmpp;
            i2++;
        }
        i2 = 0;
        for (int i = startIndexLeaf1; i < left1->NumOfKeys; i++) {
            left1->Keys[i] = keys1[i2];
            left1->Pointers[i] = pointers1[i2];
            i2++;
        }
        for (int i = 0; i <= endIndexLeaf1; i++) {
            left2->Keys[i] = keys1[i2];
            left2->Pointers[i] = pointers1[i2];
            i2++;
        }
        AdjustAncestorKeysAccordingToCurNode(left1);
        AdjustAncestorKeysAccordingToCurNode(left2);
        AdjustAncestorKeysAccordingToCurNode(right1);
        AdjustAncestorKeysAccordingToCurNode(right2);
        delete []keys1;
        delete []pointers1;
        return;
    } else { // len1 > len2
        SwapWhenBothLensAre2(right1, right2, startIndexLeaf2, endIndexLeaf2, left1, left2, startIndexLeaf1, endIndexLeaf1);
        return;
    }
}



void DynamicArray::SwapWhenBothLensAre1(NodeDA * leaf1, int startIndex1, int endIndex1, NodeDA * leaf2,
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

        NodeDA * tmpp = leaf1->Pointers[startIndex1+i];
        leaf1->Pointers[startIndex1+i] = leaf2->Pointers[startIndex2+i];
        leaf2->Pointers[startIndex2+i] = tmpp;
    }
    if (numToMove1 == numToMove2) {
        AdjustAncestorKeysAccordingToCurNode(leaf1);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        return;
    } else if (numToMove1 < numToMove2) {
        int numKeys = numToMove2 - numToMove1;
        int * keys = new int[numKeys];
        auto ** pointers = new NodeDA *[numKeys];
        for (int i = 0; i < numKeys; ++i) {
            keys[i] = leaf2->Keys[startIndex2 + exchangeNum + i];
            pointers[i] = leaf2->Pointers[startIndex2 + exchangeNum + i];
        }
        movePointerInNodeForward(leaf2, endIndex2+1, leaf2->NumOfKeys-1, startIndex2 + exchangeNum);
        leaf2->NumOfKeys -= numKeys;
        CompleteAdjustAncestorNodesAfterDelete(leaf2);
        insertKeysIntoLeaf(keys, pointers, numKeys, leaf1, endIndex1+1);
        delete []keys;
        delete []pointers;
        return;
    } else { //numToMove1 > numToMove2
        int numKeys = numToMove1 - numToMove2;
        int * keys = new int[numKeys];
        auto ** pointers = new NodeDA *[numKeys];
        for (int i = 0; i < numKeys; ++i) {
            keys[i] = leaf1->Keys[startIndex1 + exchangeNum + i];
            pointers[i] = leaf1->Pointers[startIndex1 + exchangeNum + i];
        }
        movePointerInNodeForward(leaf1, endIndex1+1, leaf1->NumOfKeys-1, startIndex1 + exchangeNum);
        leaf1->NumOfKeys -= numKeys;
        CompleteAdjustAncestorNodesAfterDelete(leaf1);
        insertKeysIntoLeaf(keys, pointers, numKeys, leaf2, endIndex2+1);
        delete []keys;
        delete []pointers;
        return;
    }
}



void DynamicArray::SwapWhenLensAre1And2(NodeDA * leaf1, int startIndex1, int endIndex1, NodeDA * leaf2,
                                        int startIndex2, NodeDA * leaf3, int endIndex3) {
    int len1 = endIndex1 - startIndex1 + 1;
    int len2 = leaf2->NumOfKeys - startIndex2;
    int len3 = endIndex3 + 1;
    if (len1 < len2) {
        for (int i = 0; i < len1; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[startIndex1+i];
            leaf1->Keys[startIndex1+i] = tmpk;
            NodeDA * tmpp = leaf2->Pointers[startIndex2+i];
            leaf2->Pointers[startIndex2+i] = leaf1->Pointers[startIndex1+i];
            leaf1->Pointers[startIndex1+i] = tmpp;
        }
        int numToMove = len2 - len1 + len3;
        int * keysToMove = new int[numToMove];
        auto ** pointersToMove = new NodeDA *[numToMove];
        int tk = 0;
        for (int i = startIndex2 + len1; i < leaf2->NumOfKeys; i++) {
            keysToMove[tk] = leaf2->Keys[i];
            pointersToMove[tk] = leaf2->Pointers[i];
            tk++;
        }
        for (int i = 0; i < len3; i++) {
            keysToMove[tk] = leaf3->Keys[i];
            pointersToMove[tk] = leaf3->Pointers[i];
            tk++;
        }
        insertKeysIntoLeaf(keysToMove, pointersToMove, numToMove, leaf1, endIndex1 + 1);
        DeleteMidPartOfTwoAdjacentLeaves(leaf2, startIndex2 + len1, leaf3, endIndex3);
        delete []keysToMove;
        delete []pointersToMove;
        return;
    } else if (len1 == len2) {
        for (int i = 0; i < len1; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[startIndex1+i];
            leaf1->Keys[startIndex1+i] = tmpk;

            NodeDA * tmpp = leaf2->Pointers[startIndex2+i];
            leaf2->Pointers[startIndex2+i] = leaf1->Pointers[startIndex1+i];
            leaf1->Pointers[startIndex1+i] = tmpp;
        }
        int * keysToMove = new int[len3];
        auto ** pointersToMove = new NodeDA *[len3];
        for (int i = 0; i < len3; i++) {
            keysToMove[i] = leaf3->Keys[i];
            pointersToMove[i] = leaf3->Pointers[i];
        }
        movePointerInNodeForward(leaf3, endIndex3+1, leaf3->NumOfKeys-1, 0);
        leaf3->NumOfKeys -= len3;
        CompleteAdjustAncestorNodesAfterDelete(leaf3);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        insertKeysIntoLeaf(keysToMove, pointersToMove, len3, leaf1, endIndex1 + 1);
        delete []keysToMove;
        delete []pointersToMove;
        return;
    } else if (len1 < len2+len3) {
        int indexIn1 = startIndex1;
        for (int i = 0; i < len2; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            NodeDA * tmpp = leaf2->Pointers[startIndex2+i];
            leaf2->Pointers[startIndex2+i] = leaf1->Pointers[indexIn1];
            leaf1->Pointers[indexIn1] = tmpp;
            indexIn1++;
        }
        int newStartIn3 = len1 - len2;
        for (int i = 0; i < newStartIn3; i++) {
            int tmpk = leaf3->Keys[i];
            leaf3->Keys[i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            NodeDA * tmpp = leaf3->Pointers[i];
            leaf3->Pointers[i] = leaf1->Pointers[indexIn1];
            leaf1->Pointers[indexIn1] = tmpp;
            indexIn1++;
        }
        int NumKeysToMove = endIndex3 - newStartIn3 + 1;
        int * keysToMove = new int[NumKeysToMove];
        auto ** pointersToMove = new NodeDA *[NumKeysToMove];
        for (int i = 0; i < NumKeysToMove; i++) {
            keysToMove[i] = leaf3->Keys[newStartIn3+i];
            pointersToMove[i] = leaf3->Pointers[newStartIn3+i];
        }
        insertKeysIntoLeaf(keysToMove, pointersToMove, NumKeysToMove, leaf1, endIndex1 + 1);
        movePointerInNodeForward(leaf3, len3, leaf3->NumOfKeys-1, len1-len2);
        leaf3->NumOfKeys -= NumKeysToMove;
        CompleteAdjustAncestorNodesAfterDelete(leaf3);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        delete []keysToMove;
        delete []pointersToMove;
        return;
    } else if (len1 == len2+len3) {
        int indexIn1 = startIndex1;
        for (int i = 0; i < len2; i++) {
            int tmpk = leaf2->Keys[startIndex2+i];
            leaf2->Keys[startIndex2+i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            NodeDA * tmpp = leaf2->Pointers[startIndex2+i];
            leaf2->Pointers[startIndex2+i] = leaf1->Pointers[indexIn1];
            leaf1->Pointers[indexIn1] = tmpp;
            indexIn1++;
        }
        for (int i = 0; i < len3; i++) {
            int tmpk = leaf3->Keys[i];
            leaf3->Keys[i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            NodeDA * tmpp = leaf3->Pointers[i];
            leaf3->Pointers[i] = leaf1->Pointers[indexIn1];
            leaf1->Pointers[indexIn1] = tmpp;
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
            NodeDA * tmpp = leaf2->Pointers[startIndex2+i];
            leaf2->Pointers[startIndex2+i] = leaf1->Pointers[indexIn1];
            leaf1->Pointers[indexIn1] = tmpp;
            indexIn1++;
        }
        for (int i = 0; i < len3; i++) {
            int tmpk = leaf3->Keys[i];
            leaf3->Keys[i] = leaf1->Keys[indexIn1];
            leaf1->Keys[indexIn1] = tmpk;
            NodeDA * tmpp = leaf3->Pointers[i];
            leaf3->Pointers[i] = leaf1->Pointers[indexIn1];
            leaf1->Pointers[indexIn1] = tmpp;
            indexIn1++;
        }
        int newStart3 = indexIn1;
        int NumToMove = len1 - len2 - len3;
        int* KeysToMove = new int[NumToMove];
        auto **PointersToMove = new NodeDA *[NumToMove];
        for (int i = newStart3; i <= endIndex1; i++) {
            KeysToMove[i-newStart3] = leaf1->Keys[i];
            PointersToMove[i-newStart3] = leaf1->Pointers[i];
        }
        insertKeysIntoLeaf(KeysToMove, PointersToMove, NumToMove, leaf3, endIndex3 + 1);
        for (int i = endIndex1 + 1; i < leaf1->NumOfKeys; i++) {
            leaf1->Keys[i-NumToMove] = leaf1->Keys[i];
            leaf1->Pointers[i-NumToMove] = leaf1->Pointers[i];
        }
        leaf1->NumOfKeys -= NumToMove;
        CompleteAdjustAncestorNodesAfterDelete(leaf1);
        AdjustAncestorKeysAccordingToCurNode(leaf2);
        delete []KeysToMove;
        delete []PointersToMove;
        return;
    }
}


NodeDA * DynamicArray::coalesceNodesStopAtParent(NodeDA * n, NodeDA * neighbour, int  neighbour_index) {
    int i, j, neighbour_insertion_index;
    NodeDA * tmp;
    int n_index = neighbour_index + 1;
    //set neighbor to be the NodeDA on the left
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
            NodeDA * tmp = neighbour->Next;
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
NodeDA * DynamicArray::swapStartPointer(NodeDA * leftLeaf, int endIndexInLeftLeaf, NodeDA *rightLeaf, int endIndexInRightLeaf) {
    int i = 0;
    int j = 0;
    int exchangeNum = endIndexInLeftLeaf;
    if (exchangeNum > endIndexInRightLeaf) {
        exchangeNum = endIndexInRightLeaf;
    }
    for (; i <= exchangeNum; i++) {
        NodeDA * tmpPointer = leftLeaf->Pointers[i];
        leftLeaf->Pointers[i] = rightLeaf->Pointers[j];
        rightLeaf->Pointers[j] = tmpPointer;
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



void DynamicArray::SwapForTailHead(NodeDA ** inter1, NodeDA ** inter2, int numLeaf1, int numLeaf2,
                                   int startIndexLeaf1, int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    swapWholeLeafAndGoUpwards(&inter1[1], &inter2[1], numLeaf1-2, numLeaf2-2);
    int numMidKeys = endIndexLeaf2+1 + startIndexLeaf2 - endIndexLeaf1 - 1 + inter1[0]->NumOfKeys - startIndexLeaf1;
    int * midKeys = new int[numMidKeys];
    auto ** midPointers = new NodeDA *[numMidKeys];
    int midIdx = 0;
    for (int i = 0; i <= endIndexLeaf2; ++i) {
        midKeys[midIdx] = inter2[numLeaf2-1]->Keys[i];
        midPointers[midIdx] = inter2[numLeaf2-1]->Pointers[i];
        midIdx++;
    }
    for (int i = endIndexLeaf1+1; i < startIndexLeaf2; ++i) {
        midKeys[midIdx] = inter2[0]->Keys[i];
        midPointers[midIdx] = inter2[0]->Pointers[i];
        midIdx++;
    }
    for (int i = startIndexLeaf1; i < inter1[0]->NumOfKeys; ++i) {
        midKeys[midIdx] = inter1[0]->Keys[i];
        midPointers[midIdx] = inter1[0]->Pointers[i];
        midIdx++;
    }

    int numBegin2 = inter2[0]->NumOfKeys - startIndexLeaf2;
    int * keysBegin2 = new int[numBegin2];
    auto ** pointersBegin2 = new NodeDA *[numBegin2];
    for (int i = 0; i < numBegin2; ++i) {
        keysBegin2[i] = inter2[0]->Keys[startIndexLeaf2+i];
        pointersBegin2[i] = inter2[0]->Pointers[startIndexLeaf2+i];
    }

    int numEnd1 = endIndexLeaf1+1;
    int * keysEnd1 = new int[numEnd1];
    auto ** pointersEnd1 = new NodeDA *[numEnd1];
    for (int i = 0; i < numEnd1; ++i) {
        keysEnd1[i] = inter1[numLeaf1-1]->Keys[i];
        pointersEnd1[i] = inter1[numLeaf1-1]->Pointers[i];
    }

    inter2[numLeaf2-2]->Next = inter1[1];
    //inter1[1]->Previous = inter2[numLeaf2-2];
    deleteEntry(inter2[0]->Parent, getIndexInParent(inter2[0]));
    inter2[0]->NumOfKeys = 0;

    //insert mid keys
    insertKeysIntoLeaf(midKeys, midPointers, numMidKeys, inter2[numLeaf2-2], inter2[numLeaf2-2]->NumOfKeys);

    //keys at the beginning of range 2
    inter1[0]->NumOfKeys = startIndexLeaf1;
    insertKeysIntoLeaf(keysBegin2, pointersBegin2, numBegin2, inter1[0], inter1[0]->NumOfKeys);
    CheckAndMergeRedistributeNode(inter1[0]);

    //keys at the end of range 1
    replaceKeysInLeaf(inter2[numLeaf2-1], 0, endIndexLeaf2, keysEnd1, pointersEnd1, numEnd1);
    delete []midKeys;
    delete []midPointers;
    delete []keysBegin2;
    delete []pointersBegin2;
    delete []keysEnd1;
    delete []pointersEnd1;
}


void DynamicArray::SwapWhenLenIs1(NodeDA * LoneLeaf, NodeDA **inter2, int numLeaf2, int startIndexLeaf1,
                                  int endIndexLeaf1, int startIndexLeaf2, int endIndexLeaf2) {
    int numInRightPart = LoneLeaf->NumOfKeys - endIndexLeaf1 - 1;
    int numToInsertAfterFullLeaves = numInRightPart + endIndexLeaf2 + 1;
    int * keysToInsertAfterFullLeaves = new int[numToInsertAfterFullLeaves];// make([]int, NumInRightPart);
    auto ** pointersToInsertAfterFullLeaves = new NodeDA *[numToInsertAfterFullLeaves];// make([]interface{}, NumInRightPart);
    for (int i = 0; i <= endIndexLeaf2; ++i) {
        keysToInsertAfterFullLeaves[i] = inter2[numLeaf2-1]->Keys[i];
        pointersToInsertAfterFullLeaves[i] = inter2[numLeaf2-1]->Pointers[i];
    }
    for (int i = 0; i < numInRightPart; i++) {
        keysToInsertAfterFullLeaves[endIndexLeaf2+1+i] = LoneLeaf->Keys[endIndexLeaf1+1+i];
        pointersToInsertAfterFullLeaves[endIndexLeaf2+1+i] = LoneLeaf->Pointers[endIndexLeaf1+1+i];
    }

    //keys to move in LoneLeaf
    int numToMoveInLoneLeaf = endIndexLeaf1 - startIndexLeaf1 + 1;
    int * keysToMoveInLoneLeaf = new int[numToMoveInLoneLeaf];
    auto ** poitnersToMoveInLoneLeaf = new NodeDA *[numToMoveInLoneLeaf];
    for (int i = 0; i < numToMoveInLoneLeaf; ++i) {
        keysToMoveInLoneLeaf[i] = LoneLeaf->Keys[startIndexLeaf1+i];
        poitnersToMoveInLoneLeaf[i] = LoneLeaf->Pointers[startIndexLeaf1+i];
    }
    //keys to move in the beginning of inter2[0]
    int numToMoveInBeginInter2 = inter2[0]->NumOfKeys - startIndexLeaf2;
    int * keysToMoveInBeginInter2 = new int[numToMoveInBeginInter2];
    auto ** poitnersToMoveInBeginInter2 = new NodeDA *[numToMoveInBeginInter2];
    for (int i = 0; i < numToMoveInBeginInter2; ++i) {
        keysToMoveInBeginInter2[i] = inter2[0]->Keys[startIndexLeaf2+i];
        poitnersToMoveInBeginInter2[i] = inter2[0]->Pointers[startIndexLeaf2+i];
    }

    NodeDA ** leafToInsert = &inter2[1];
    int numLeafToDelete = numLeaf2 - 2;
    //delete leaves
    //NodeDA * firstl = leafToInsert[0];
    NodeDA * lastl = leafToInsert[numLeafToDelete-1];
    NodeDA * pre = inter2[0]; // findPrevious(firstl);
    if (pre != nullptr) {
        pre->Next = lastl->Next;
    }
    /*
    if (lastl->Next != nullptr) {
        lastl->Next->Previous = firstl->Previous;
    }
     */
    NodeDA * preLeaf = deleteMulEntry(leafToInsert, numLeafToDelete);
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, leafToInsert, numLeafToDelete);

    //insert leaves
    NodeDA * toParent = LoneLeaf->Parent;
    int startIndexInParent = getIndexInParent(LoneLeaf) + 1;
    insertLeavesIntoLeafParent(toParent, startIndexInParent, leafToInsert, numLeafToDelete);

    //insert keysToInsertAfterFullLeaves
    insertKeysIntoLeaf(keysToInsertAfterFullLeaves, pointersToInsertAfterFullLeaves, numToInsertAfterFullLeaves,
                       inter2[numLeaf2-2], inter2[numLeaf2-2]->NumOfKeys);
    LoneLeaf->NumOfKeys = startIndexLeaf1;
    insertKeysIntoLeaf(keysToMoveInBeginInter2, poitnersToMoveInBeginInter2, numToMoveInBeginInter2, LoneLeaf, LoneLeaf->NumOfKeys);
    CheckAndMergeRedistributeNode(LoneLeaf);

    //replace keys in middle of inter2[0], inter2[numLeaf2-1]
    ReplaceMidOfTwoAdjacentLeaves(inter2[0], inter2[numLeaf2-1], startIndexLeaf2, endIndexLeaf2,
                                  keysToMoveInLoneLeaf, poitnersToMoveInLoneLeaf, numToMoveInLoneLeaf);
    delete []keysToInsertAfterFullLeaves;
    delete []pointersToInsertAfterFullLeaves;
    delete []keysToMoveInBeginInter2;
    delete []poitnersToMoveInBeginInter2;
    delete []keysToMoveInLoneLeaf;
    delete []poitnersToMoveInLoneLeaf;
}

// move [start_, end_] before row des_
void DynamicArray::Move(int start_, int end_, int des_) {
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

    CutArray(start);
    //PrintTree();
    CutArray(end+1);
    //PrintTree();
    int toindex = -1;
    NodeDA * toleaf = CutForDesInMove(des, &toindex);
    //printf("after CutForDesInMove\n");
    //PrintDA();
    //toleaf, toindex := da.CutForDesInMove(des);
    int startIndexLeaf1, endIndexLeaf1;
    NodeDA ** inter1 = nullptr;
    int numLeaf1 = RangePosInLeafPointer(start, end, &inter1, &startIndexLeaf1, &endIndexLeaf1);
    //inter1, numLeaf1, startIndexLeaf1, endIndexLeaf1 := da.RangePosInLeafPointer(start, end);
    //printf("before RealMove, da\n");
    //PrintTree();
    RealMove(startIndexLeaf1, endIndexLeaf1, inter1, numLeaf1, toleaf, toindex);
    //printf("after realmove: \n");
    //PrintDA();
    if (des < start) {
        CheckMinArrayAfterSwap(des-1);
        CheckMinArrayAfterSwap(des);
        CheckMinArrayAfterSwap(des + end-start);
        CheckMinArrayAfterSwap(des + end-start + 1);
        CheckMinArrayAfterSwap(end);
        CheckMinArrayAfterSwap(end+1);
    } else {
        CheckMinArrayAfterSwap(start-1);
        CheckMinArrayAfterSwap(start);
        CheckMinArrayAfterSwap(des - (end-start+1)-1);
        CheckMinArrayAfterSwap(des - (end-start+1));
        CheckMinArrayAfterSwap(des-1);
        CheckMinArrayAfterSwap(des);
    }
}

void DynamicArray::RealMove(int startIndexLeaf1, int endIndexLeaf1, NodeDA ** inter1, int numLeaf1, NodeDA * toleaf, int toindex) {
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
            auto ** leaves = new NodeDA *[1];
            leaves[0] = toleaf;
            SwapRangesAdjacent(inter1, leaves, numLeaf1, 1, startIndexLeaf1, endIndexLeaf1, 0, toindex-1);
            delete []leaves;
        } else {
            auto ** leaves = new NodeDA *[2];
            leaves[0] = inter1[numLeaf1-1];
            leaves[1] = toleaf;
            swapHeadTailLen2Is2(inter1, leaves, numLeaf1, 2, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toindex-1);
            delete []leaves;
        }
    } else if (toleaf->Next == inter1[0]) {
        if (startIndexLeaf1==0) {
            auto ** leaves = new NodeDA *[1];
            leaves[0] = toleaf;
            SwapRangesAdjacent(leaves, inter1, 1, numLeaf1, toindex, toleaf->NumOfKeys-1, startIndexLeaf1, endIndexLeaf1);
            delete []leaves;
        } else {
            auto ** leaves = new NodeDA *[2];
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

void DynamicArray::MoveToLeafIsFirstLeaf(NodeDA ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA * toleaf, int toindex) {
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

void DynamicArray::MoveToLeafIsLastLeaf(NodeDA ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA * toleaf, int toindex) {
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

void DynamicArray::Move1(NodeDA ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA * toleaf, int toindex) {
    //NodeDA ** leafToMove = new NodeDA * [numLeaf1-2];
    int NumLeafToMove = numLeaf1-2;
    NodeDA ** leafToMove = &inter1[1]; // only move whole leaves

    NodeDA * fl = inter1[0];// findPrevious(leafToMove[0]);
    if (fl != nullptr) {
        fl->Next = leafToMove[NumLeafToMove-1]->Next;
        /*
        if (fl->Next != nullptr) {
            fl->Next->Previous = fl;
        }
         */
    }
    NodeDA * preLeaf = deleteMulEntry(leafToMove, NumLeafToMove);
    CallRedistributeMergeRecomputeKeysInAncestorsLeftPart(preLeaf, leafToMove, NumLeafToMove);

    int startIndex = getIndexInParent(toleaf)+1;
    NodeDA * toParent = toleaf->Parent;
    insertLeavesIntoLeafParent(toParent, startIndex, leafToMove, NumLeafToMove);

    int numKeysBefore = inter1[0]->NumOfKeys - startIndexLeaf1;
    int * KeysBefore = new int[numKeysBefore];
    auto ** PointersBefore = new NodeDA *[numKeysBefore];
    for (int i = 0; i < numKeysBefore; i ++) {
        KeysBefore[i] = inter1[0]->Keys[startIndexLeaf1+i];
        PointersBefore[i] = inter1[0]->Pointers[startIndexLeaf1+i];
    }

    int numKeysAfter = endIndexLeaf1+1 + toleaf->NumOfKeys - toindex;
    int * KeysAfter = new int[numKeysAfter];// make([]int, numKeysAfter);
    auto ** PointersAfter = new NodeDA *[numKeysAfter];
    int afterIdx = 0;
    for (int i = 0; i < endIndexLeaf1+1; i ++) {
        KeysAfter[i] = inter1[numLeaf1-1]->Keys[i];
        PointersAfter[i] = inter1[numLeaf1-1]->Pointers[i];
        afterIdx++;
    }
    for (int j = toindex; j < toleaf->NumOfKeys; ++j) {
        KeysAfter[afterIdx] = toleaf->Keys[j];
        PointersAfter[afterIdx] = toleaf->Pointers[j];
        afterIdx++;
    }

    insertKeysIntoLeaf(KeysAfter, PointersAfter, numKeysAfter, inter1[numLeaf1-2], inter1[numLeaf1-2]->NumOfKeys);
    insertKeysIntoLeaf(KeysBefore, PointersBefore, numKeysBefore, inter1[1], 0);
    toleaf->NumOfKeys = toindex;
    CompleteAdjustAncestorNodesAfterDelete(toleaf);
    DeleteMidPartOfTwoAdjacentLeaves(inter1[0], startIndexLeaf1, inter1[numLeaf1-1], endIndexLeaf1);
    delete []PointersAfter;
    delete []KeysAfter;
    delete []PointersBefore;
    delete []KeysBefore;
}


void DynamicArray::MoveWhenLenIs1(NodeDA ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA * toleaf, int toIndex) {
    NodeDA * fromLeaf = inter1[0];
    if (fromLeaf->Next == toleaf) {
        if (endIndexLeaf1 < fromLeaf->NumOfKeys-1) {
            swapHeadTail12(fromLeaf, toleaf, startIndexLeaf1, endIndexLeaf1, endIndexLeaf1+1, toIndex-1);
        } else {
            SWapRangesAdjacentWhenLensAreBoth1(fromLeaf, toleaf, startIndexLeaf1, endIndexLeaf1, 0, toIndex-1);
        }
        return;
    } else if (toleaf->Next == fromLeaf) {
        if (startIndexLeaf1 > 0) {
            swapHeadTail21(toleaf, fromLeaf, toIndex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        } else {
            SWapRangesAdjacentWhenLensAreBoth1(toleaf, fromLeaf, toIndex, toleaf->NumOfKeys-1, 0, endIndexLeaf1);
        }
        return;
    }

    if (fromLeaf != toleaf) {
        int NumKeys = endIndexLeaf1-startIndexLeaf1+1;
        int * Keys = new int[NumKeys];// make([]int, NumKeys)
        auto ** Pointers = new NodeDA * [NumKeys];
        //Pointers = make([]interface{}, NumKeys)
        for (int i = startIndexLeaf1; i <= endIndexLeaf1; i ++) {
            Keys[i-startIndexLeaf1] = fromLeaf->Keys[i];
            Pointers[i-startIndexLeaf1] = fromLeaf->Pointers[i];
        }
        insertKeysIntoLeaf(Keys, Pointers, NumKeys, toleaf, toIndex);
        movePointerInNodeForward(fromLeaf, endIndexLeaf1+1, fromLeaf->NumOfKeys-1, startIndexLeaf1);
        fromLeaf->NumOfKeys -= NumKeys;
        CompleteAdjustAncestorNodesAfterDelete(fromLeaf);
        delete []Pointers;
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


void DynamicArray::MoveWhenLenIs2(NodeDA ** inter1, int numLeaf1, int startIndexLeaf1, int endIndexLeaf1, NodeDA * toleaf, int toIndex) {
    NodeDA * fromLeaf1 = inter1[0];
    NodeDA * fromLeaf2 = inter1[1];
    if (toleaf->Next == fromLeaf1) {
        if (startIndexLeaf1 > 0) {
            swapHeadTail22(toleaf, fromLeaf1, fromLeaf2, toIndex, startIndexLeaf1-1, startIndexLeaf1, endIndexLeaf1);
        } else {
            SWapRangesAdjacentWhenLensAre1And2(toleaf, fromLeaf1, fromLeaf2, toIndex, toleaf->NumOfKeys-1, startIndexLeaf1, endIndexLeaf1);
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
        auto ** Pointers = new NodeDA *[NumKeys];
        for (int i = startIndexLeaf1; i < fromLeaf1->NumOfKeys; i ++) {
            Keys[ik] = fromLeaf1->Keys[i];
            Pointers[ik] = fromLeaf1->Pointers[i];
            ik++;
        }
        for (int i = 0; i <= endIndexLeaf1; i ++) {
            Keys[ik] = fromLeaf2->Keys[i];
            Pointers[ik] = fromLeaf2->Pointers[i];
            ik++;
        }
        insertKeysIntoLeaf(Keys, Pointers, NumKeys, toleaf, toIndex);
        DeleteMidPartOfTwoAdjacentLeaves(fromLeaf1, startIndexLeaf1, fromLeaf2, endIndexLeaf1);
        delete []Pointers;
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

NodeDA * DynamicArray::CutForDesInMove(int des, int* toIndex) {
    int IndexInLeaf, IndexInArray;
    NodeDA * leaf = findLeaf(des, &IndexInLeaf, &IndexInArray);
    if (des > NumItems) {
        *toIndex = IndexInLeaf + 1;
        return leaf;
    }
    if (IndexInArray == 0) {
        *toIndex = IndexInLeaf;
        return leaf;
    } else {
        int * tmp = reinterpret_cast<int *>(leaf->Pointers[IndexInLeaf]);
        int oldlen = leaf->Keys[IndexInLeaf];
        int leftl = IndexInArray;
        int rightl = oldlen-leftl;

        if (leftl >= Capacity && rightl >= Capacity) {
            leaf->Keys[IndexInLeaf] = leftl;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(tmp);
            int * righta = &tmp[IndexInArray];
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
                *toIndex = IndexInLeaf + 1;
                return leaf;
            } else {
                return insertOneIntoLeafAfterSplittingForMove(leaf, righta, rightl, IndexInLeaf+1, toIndex);
            }
        } else if (leftl >= Capacity) {
            int * righta = new int[Capacity];
            for (int i = 0; i < rightl; i ++) {
                righta[i] = tmp[IndexInArray+i];
            }
            leaf->Keys[IndexInLeaf] = leftl;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(tmp);
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
                *toIndex = IndexInLeaf + 1;
                return leaf;
            } else {
                return insertOneIntoLeafAfterSplittingForMove(leaf, righta, rightl, IndexInLeaf+1, toIndex);
            }
        } else if (rightl >= Capacity) {
            int * lefta = new int[Capacity];
            //lefta := make([]int, Capacity)
            for (int i = 0; i < leftl; i ++) {
                lefta[i] = tmp[i];
            }
            leaf->Keys[IndexInLeaf] = leftl;
            leaf->Pointers[IndexInLeaf] = reinterpret_cast<NodeDA *>(lefta);
            int * righta = &tmp[IndexInArray];
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
                *toIndex = IndexInLeaf + 1;
                return leaf;
            } else {
                return insertOneIntoLeafAfterSplittingForMove(leaf, righta, rightl, IndexInLeaf+1, toIndex);
            }
        } else {
            int * righta = new int[Capacity];
            //righta := make([]int, Capacity)
            for (int i = 0; i < rightl; i ++) {
                righta[i] = tmp[IndexInArray+i];
            }
            leaf->Keys[IndexInLeaf] = leftl;
            if (leaf->NumOfKeys < Capacity) {
                insertOneIntoLeafForCut(leaf, righta, rightl, IndexInLeaf+1);
                *toIndex = IndexInLeaf+1;
                return leaf;
            } else {
                return insertOneIntoLeafAfterSplittingForMove(leaf, righta, rightl, IndexInLeaf+1, toIndex);
            }
        }
    }
}

NodeDA * DynamicArray::insertOneIntoLeafAfterSplittingForMove(NodeDA * leaf, int * newID, int length, int indexInLeaf, int * toIndex) {
    NodeDA * new_leaf;
    int split= Min, i, newIndex = -1;
    int LeafNumPointers = 0, NewLeafNumPointers = 0;
    new_leaf = makeLeafonedp(Capacity);

    leaf->NumOfKeys = split;
    new_leaf->NumOfKeys = Capacity + 1 - split;

    if (indexInLeaf < split) { // new array is in leaf (old leaf)
        int j = Capacity - 1;
        for (i = new_leaf->NumOfKeys - 1; i >= 0; i--) {
            new_leaf->Keys[i] = leaf->Keys[j];
            leaf->Keys[j] = 0;
            new_leaf->Pointers[i] = leaf->Pointers[j];
            leaf->Pointers[j] = nullptr;
            j--;
            NewLeafNumPointers += new_leaf->Keys[i];
        }
        for (i = split - 1; i > indexInLeaf; i--) {
            leaf->Pointers[i] = leaf->Pointers[i-1];
            leaf->Keys[i] = leaf->Keys[i-1];
            LeafNumPointers += leaf->Keys[i];
        }
        LeafNumPointers += length;
        leaf->Keys[i] = length;
        leaf->Pointers[i] = reinterpret_cast<NodeDA *>(newID);
        newIndex = i;
        i--;
        for (; i >= 0; i--) {
            LeafNumPointers += leaf->Keys[i];
        }

        new_leaf->Parent = leaf->Parent;
        new_leaf->IsLeaf = true;
        new_leaf->Next = leaf->Next;
        /*
        if (new_leaf->Next != nullptr) {
            new_leaf->Next->Previous = new_leaf;
        }
        new_leaf->Previous = leaf;
         */
        leaf->Next = new_leaf;
        insertIntoParent(leaf, LeafNumPointers, new_leaf, NewLeafNumPointers);

        *toIndex = newIndex;
        return leaf;
    } else { // new array is in new_leaf
        indexInLeaf -= split;
        for (i = 0; i < new_leaf->NumOfKeys; i++) {
            if (i == indexInLeaf) {
                NewLeafNumPointers += length;
                new_leaf->Keys[i] = length;
                new_leaf->Pointers[i] = reinterpret_cast<NodeDA *>(newID);
                newIndex = i;
                break;
            } else {
                new_leaf->Keys[i] = leaf->Keys[i+split];
                leaf->Keys[i+split] = 0;
                new_leaf->Pointers[i] = leaf->Pointers[i+split];
                leaf->Pointers[i+split] = nullptr;
                NewLeafNumPointers += new_leaf->Keys[i];
            }
        }
        i++;
        for (; i < new_leaf->NumOfKeys; i++) {
            new_leaf->Keys[i] = leaf->Keys[i-1+split];
            leaf->Keys[i-1+split] = 0;
            new_leaf->Pointers[i] = leaf->Pointers[i-1+split];
            leaf->Pointers[i-1+split] = nullptr;
            NewLeafNumPointers += new_leaf->Keys[i];
        }
        for (int y = 0; y < leaf->NumOfKeys; y++) {
            LeafNumPointers += leaf->Keys[y];
        }
        new_leaf->Parent = leaf->Parent;
        new_leaf->IsLeaf = true;
        new_leaf->Next = leaf->Next;
        /*
        if (new_leaf->Next != nullptr) {
            new_leaf->Next->Previous = new_leaf;
        }
        new_leaf->Previous = leaf;
         */
        leaf->Next = new_leaf;
        insertIntoParent(leaf, LeafNumPointers, new_leaf, NewLeafNumPointers);
        *toIndex = newIndex;
        return new_leaf;
    }
}


