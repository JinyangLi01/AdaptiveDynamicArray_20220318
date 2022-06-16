//
//  CountedBtree.cpp
//  ADADebug
//
//  Created by Jinyang Li on 2/20/22.
//

#include "CountedBtree.h"
#include <iostream>
#include <list>
using namespace std;

NodeForCBT::NodeForCBT(int capacity, int numkeys, NodeForCBT * parent, bool isLeaf) {
    Pointers = new NodeForCBT * [capacity];
    Keys = new int[capacity];
    KeysNumInChild = new int[capacity];
    for (int i = 0; i < capacity; i++) {
        Pointers[i] = nullptr;
        Keys[i] = 0;
        KeysNumInChild[i] = 0;
    }
    Parent = parent;
    NumOfKeys = numkeys;
    IsLeaf = isLeaf;
}

void CountedBtree::PrintTree() const {
    printf("\n");
    int numInLevel = 0;
    int curNumInLevel = 0;
    int nextNumInLevel = 1;
    list<NodeForCBT*> lst;
//    lst.resize(NumKeys/Capacity * 2);
    int i;
    if (Root == nullptr) {
        printf("Empty tree.\n");
        return;
    }
    lst.push_back(Root);
    int order = Capacity;
    while (!lst.empty()) {
        NodeForCBT * ele = lst.front();
        if (ele != nullptr) {
            lst.pop_front();
            //n := ele->Value.(*NodeForCBT);
            if (curNumInLevel == numInLevel) {
                curNumInLevel = 1;
                numInLevel = nextNumInLevel;
                nextNumInLevel = 0;
                printf("\n");
            } else {
                curNumInLevel++;
            }
            for (i = 0; i < ele->NumOfKeys; i++) {
                printf("%d ", ele->Keys[i]);
            }
            for (int j = ele->NumOfKeys; j < order - 1; j++) {
                printf("0 ");
            }
            printf("(");
            for (int k = 0; k < order; k++) {
                printf("%d ", ele->KeysNumInChild[k]);
            }
            printf(")");
            if (!ele->IsLeaf) {
                for (i = 0; i < ele->NumOfKeys+1; i++) {
                    if (ele->Pointers[i] != nullptr) {
                        NodeForCBT * c = ele->Pointers[i];
                        for (int k = 0; k < c->NumOfKeys; k++) {
                            printf("%d   ", c->Keys[k]);
                        }
                        printf("\n");
                        lst.push_back(c);
                        nextNumInLevel++;
                    }
                }
            }
            printf(" | ");
        }
    }
    printf("\n\n");
}

CountedBtree* NewCBTreeForArray(int order, int num) {
    auto* t = new CountedBtree(order);
    for (int i = 1; i <= num; i++) {
        t->Insert(i, i);
        //t->PrintTree();
    }
    return t;
}


CountedBtree::~CountedBtree() {
    std::list<NodeForCBT *> toDelete;
    std::list<NodeForCBT *> toFindChild;
    while (!toFindChild.empty()) {
        NodeForCBT * ele = toFindChild.front();
        toFindChild.pop_front();
        if (ele->IsLeaf) {
            // std::cout<<"delete ele"<<endl;
            // delete ele;
        } else {
            for (int i = 0; i < ele->NumOfKeys; ++i) {
                toFindChild.push_back(ele->Pointers[i]);
            }
            toDelete.push_back(ele);
        }
    }
    while (!toDelete.empty()) {
        // NodeForCBT * ele = toDelete.front();
        toDelete.pop_front();
        // delete ele;
    }
}


void CountedBtree::Insert(int rowID, int pos) {
    NumKeys++;
    if (Root == nullptr) {
        startNewTree(rowID);
        return;
    }
    int NodeForCBTNum = NumKeys;
    if (pos > NodeForCBTNum+1) {
        pos = NodeForCBTNum + 1;
    }
    int IndexToInsertInLeaf;
    NodeForCBT * leaf = findLeaf(pos, &IndexToInsertInLeaf);

    if (leaf->NumOfKeys < Capacity-1) {
        insertIntoLeaf(leaf, rowID, IndexToInsertInLeaf);
        return;
    }
    insertIntoLeafAfterSplitting(leaf, rowID, IndexToInsertInLeaf);
}



NodeForCBT * CountedBtree::FindLastKeyInLeaf(NodeForCBT * n, int* indexInLeaf) {
    NodeForCBT * c = n;
    while (!c->IsLeaf) {
        c = c->Pointers[c->NumOfKeys];
    }
    * indexInLeaf = c->NumOfKeys;
    return c ;
}

NodeForCBT * CountedBtree::findLeaf(int pos, int * IndexToInsertInLeaf) const {
    NodeForCBT * c = Root;
    int p = pos;
    if (c == nullptr) {
        *IndexToInsertInLeaf = 0;
        return c;
    }
    bool flag = false;
    while (!c->IsLeaf) {
        flag = false;
        int i = 0;
        for (; i < c->NumOfKeys; i++) {
            if (p <= c->KeysNumInChild[i]) {
                c = c->Pointers[i];
                flag = true;
                break;
            } else if (p == c->KeysNumInChild[i]+1) {
                return FindLastKeyInLeaf(c->Pointers[i], IndexToInsertInLeaf);
            }
            p -= c->KeysNumInChild[i] + 1;
        }
        if (!flag) {
            c = c->Pointers[c->NumOfKeys];
        }
    }
    *IndexToInsertInLeaf = p-1;
    return c;
}


int CountedBtree::getLeftIndex(NodeForCBT * parent, NodeForCBT * left) const {
    int left_index = 0;
    while (left_index < Capacity && parent->Pointers[left_index] != left) {
        left_index += 1;
    }
    return left_index;
}

void CountedBtree::insertIntoLeaf(NodeForCBT * leaf, int key, int indexInLeaf) const {
    int keyToStore = key;
    leaf->NumOfKeys++;
    int tmpKey;
    for (int i = indexInLeaf; i < Capacity-1; i++) {
        if (leaf->Keys[i] != 0) {
            tmpKey = leaf->Keys[i];
            leaf->Keys[i] = keyToStore;
            keyToStore = tmpKey;
        } else {
            leaf->Keys[i] = keyToStore;
            break;
        }
    }

    if (leaf == Root) {
        return;
    }
    NodeForCBT * c = leaf;
    NodeForCBT * parent = c->Parent;
    while (parent != nullptr) {
        for (int i = 0; i < Capacity; i++) {
            if (parent->Pointers[i] == c) {
                parent->KeysNumInChild[i]++;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

void CountedBtree::insertIntoLeafAfterSplitting(NodeForCBT * leaf, int key, int indexInLeaf) {
    NodeForCBT *  new_leaf;
    int insertion_index, i, j;
    int leftNumKeys, rightNumKeys;
    new_leaf = new NodeForCBT(Capacity, 0, nullptr, true);
    int * temp_keys = new int[Capacity * 2];
    //temp_keys := make([]int, Capacity*2)

    insertion_index = indexInLeaf;
    j = 0;
    for (i = 0; i < Capacity-1; i++) {
        if (j == insertion_index) {
            j += 1;
        }
        if (leaf->Keys[i] != 0) {
            temp_keys[j] = leaf->Keys[i];
            leaf->Keys[i] = 0;

            j += 1;
        } else {
            break;
        }
    }

    temp_keys[insertion_index] = key;
    int upKeyIndex = Capacity/2; // flooring
    leftNumKeys = upKeyIndex;

    for (i = 0; i < leftNumKeys; i++) {
        leaf->Keys[i] = temp_keys[i];
    }
    int KeyToLift = temp_keys[upKeyIndex];
    j = 0;
    for (i = leftNumKeys + 1; i < Capacity; i++) {
        new_leaf->Keys[j] = temp_keys[i];
        j += 1;
    }
    rightNumKeys = j;
    new_leaf->Parent = leaf->Parent;
    new_leaf->IsLeaf = true;

    leaf->NumOfKeys = leftNumKeys;
    new_leaf->NumOfKeys = rightNumKeys;

    insertIntoParent(leaf, leftNumKeys, new_leaf, rightNumKeys, KeyToLift);
    delete []temp_keys;
}

void CountedBtree::insertIntoNodeForCBT(NodeForCBT * n, int left_index, int sumKeysRight, NodeForCBT * right, int KeyToLift) const {
    n->NumOfKeys++;
    int KeyToStore = KeyToLift;
    int i;
    int keynum_to_store = sumKeysRight;
    NodeForCBT * pointer_to_store = right;
    for (i = left_index + 1; i < Capacity; i++) {
        if (n->KeysNumInChild[i] != 0) {
            int tmp_keynum = n->KeysNumInChild[i];
            NodeForCBT * tmp_pointer = n->Pointers[i];
            int tmp_key = n->Keys[i-1];
            n->Keys[i-1] = KeyToStore;
            n->KeysNumInChild[i] = keynum_to_store;
            n->Pointers[i] = pointer_to_store;
            keynum_to_store = tmp_keynum;
            pointer_to_store = tmp_pointer;
            KeyToStore = tmp_key;
        } else {
            n->KeysNumInChild[i] = keynum_to_store;
            n->Pointers[i] = pointer_to_store;
            n->Keys[i-1] = KeyToStore;
            break;
        }
    }
    NodeForCBT * c = n;
    NodeForCBT * parent = c->Parent;
    while (parent != nullptr) {
        for (i = 0; i < Capacity; i++) {
            if (parent->Pointers[i] == c) {
                parent->KeysNumInChild[i]++;
                c = parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

void CountedBtree::insertIntoNodeForCBTAfterSplitting(NodeForCBT * parent, int left_index, int sumKeysRight, NodeForCBT * right, int newKey) {
    int i, j, LeftSumKeys = 0, RightSumKeys = 0;
    NodeForCBT * new_NodeForCBT, * child;
    int * temp_keynum = new int[Capacity+1];
    int *  temp_keys = new int[Capacity];
    auto ** temp_pointers = new NodeForCBT* [Capacity+1];

    j = 0;
    for (i = 0; i < Capacity-1; i++) {
        if (j == left_index) {
            j += 1;
        }
        if (parent->KeysNumInChild[i] != 0) {
            temp_keys[j] = parent->Keys[i];
            parent->Keys[i] = 0;
            j++;
        } else {
            break;
        }
    }

    j = 0;
    for (i = 0; i < Capacity; i++) {
        if (j == left_index+1) {
            j += 1;
        }
        if (parent->KeysNumInChild[i] != 0) {
            temp_pointers[j] = parent->Pointers[i];
            temp_keynum[j] = parent->KeysNumInChild[i];
            parent->KeysNumInChild[i] = 0;
            parent->Pointers[i] = nullptr;
            j++;
        } else {
            break;
        }
    }
    temp_pointers[left_index+1] = right;
    temp_keynum[left_index+1] = sumKeysRight;
    temp_keys[left_index] = newKey;

    int UpKeyIndex = Capacity/2;
    new_NodeForCBT = new NodeForCBT(Capacity);
    for (i = 0; i < UpKeyIndex; i++) {
        LeftSumKeys += temp_keynum[i] + 1;
        parent->Pointers[i] = temp_pointers[i];
        parent->KeysNumInChild[i] = temp_keynum[i];
        parent->Keys[i] = temp_keys[i];
    }
    LeftSumKeys += temp_keynum[i];
    parent->Pointers[i] = temp_pointers[i];
    parent->KeysNumInChild[i] = temp_keynum[i];
    int KeyToLift = temp_keys[i];
    i++;

    parent->NumOfKeys = UpKeyIndex;
    j = 0;
    for (; i < Capacity; i++) {
        RightSumKeys += temp_keynum[i] + 1;
        new_NodeForCBT->Pointers[j] = temp_pointers[i];
        new_NodeForCBT->KeysNumInChild[j] = temp_keynum[i];
        new_NodeForCBT->Keys[j] = temp_keys[i];
        child = new_NodeForCBT->Pointers[j];
        child->Parent = new_NodeForCBT;
        j += 1;
    }
    RightSumKeys += temp_keynum[i];
    new_NodeForCBT->Pointers[j] = temp_pointers[i];
    child = new_NodeForCBT->Pointers[j];
    child->Parent = new_NodeForCBT;
    new_NodeForCBT->KeysNumInChild[j] = temp_keynum[i];
    new_NodeForCBT->NumOfKeys = j;
    j++;
    new_NodeForCBT->Parent = parent->Parent;
    insertIntoParent(parent, LeftSumKeys, new_NodeForCBT, RightSumKeys, KeyToLift);
    delete []temp_keys;
    delete []temp_keynum;
    delete []temp_pointers;
}

void CountedBtree::insertIntoParent(NodeForCBT * left, int sumKeysLeft, NodeForCBT * right, int sumKeysRight, int KeyToLift) {
    NodeForCBT * parent = left->Parent;

    if (parent == nullptr) {
        // insert left, right into a new root
        insertIntoNewRoot(left, sumKeysLeft, right, sumKeysRight, KeyToLift);
        return;
    }

    int left_index = getLeftIndex(parent, left); //index of leaf, starting from 0
    parent->KeysNumInChild[left_index] = sumKeysLeft;

    if (parent->NumOfKeys < Capacity-1) {
        // insert NodeForCBT "right" into parent, index is left_index + 1
        insertIntoNodeForCBT(parent, left_index, sumKeysRight, right, KeyToLift); // right key and keys upwards modified
        return;
    }

    // split parent, and insert NodeForCBT "right" into parent, index is left_index + 1
    insertIntoNodeForCBTAfterSplitting(parent, left_index, sumKeysRight, right, KeyToLift);
}

void CountedBtree::insertIntoNewRoot(NodeForCBT * left, int sumKeysLeft, NodeForCBT * right, int sumKeysRight, int newkey) {
    Root = new NodeForCBT(Capacity);
    Root->KeysNumInChild[0] = sumKeysLeft;
    Root->Pointers[0] = left;
    Root->KeysNumInChild[1] = sumKeysRight;
    Root->Pointers[1] = right;
    Root->Keys[0] = newkey;
    Root->Parent = nullptr;
    left->Parent = Root;
    right->Parent = Root;
    Root->NumOfKeys = 1;
}

void CountedBtree::startNewTree(int rowID) {
    Root = new NodeForCBT(Capacity);
    Root->IsLeaf = true;
    NumKeys = 1;
    Root->Keys[0] = rowID;
    Root->NumOfKeys = 1;
}

int CountedBtree::getLeftNeighborIndex(NodeForCBT * n) {
    NodeForCBT * parent = n->Parent;
    for (int i = 0; i < parent->NumOfKeys+1; i++) {
        if (parent->Pointers[i] == n) {
            return i - 1;
        }
    }
    return -1;
}

int CountedBtree::getRightNeighborIndex(NodeForCBT * n) {
    int i;
    NodeForCBT * parent = n->Parent;
    for (i = 0; i < parent->NumOfKeys; i++) {
        if (parent->Pointers[i] == n) {
            return i + 1;
        }
    }
    return -1;
}

//NodeForCBT * CountedBtree::removeEntryFromNodeForCBT(NodeForCBT * n, int indexInNodeForCBT) const {
//    n->KeysNumInChild[indexInNodeForCBT] = 0;
//    if (n->Pointers[indexInNodeForCBT] != nullptr) {
//        NodeForCBT * deleted = n->Pointers[indexInNodeForCBT];
//        deleted->Parent = nullptr;
//        n->Pointers[indexInNodeForCBT] = nullptr;
//    }
//    int j = indexInNodeForCBT;
//    for (int i = j + 1; i < Capacity; i++) {
//        if (n->KeysNumInChild[i] != 0) {
//            n->KeysNumInChild[j] = n->KeysNumInChild[i];
//            n->Pointers[j] = n->Pointers[i];
//            n->KeysNumInChild[i] = 0;
//            n->Pointers[i] = nullptr;
//            j++;
//        } else {
//            break;
//        }
//    }
//    return n;
//}
/*
void CountedBtree::adjustRoot(){
    NodeForCBT *  new_root = nullptr;

    if (Root->NumOfKeys > 1) {
        return;
    }

    if (!Root->IsLeaf) {
        for (int i = 0; i < Capacity; i++) { // don't need this loop
            if (Root->Pointers[i] != nullptr) {
                new_root = Root->Pointers[i];
                break;
            }
        }
        new_root->Parent = nullptr;
        Root = new_root;
    }
}
*/


void CountedBtree::coalesceLeafForCBTs(NodeForCBT * n, NodeForCBT * neighbour, int neighbour_index, int flag) {
    int i, j;
    NodeForCBT * tmp;

    if (flag == 1) {
        tmp = n;
        n = neighbour;
        neighbour = tmp;
        neighbour_index--;
    }
    NodeForCBT * parent = n->Parent;
    i = neighbour->NumOfKeys;
    neighbour->Keys[i] = parent->Keys[neighbour_index];
    i++;
    for (j = 0; j < n->NumOfKeys; j++) {
        neighbour->Keys[i] = n->Keys[j];
        i += 1;
    }
    neighbour->NumOfKeys = neighbour->NumOfKeys + n->NumOfKeys + 1;
    parent->KeysNumInChild[neighbour_index] = ComputeNumItems(neighbour); //neighbour->NumOfKeys

    DeleteFromNodeForCBT(n->Parent, neighbour_index);
}

void CountedBtree::coalesceNodeForCBTs(NodeForCBT * n, NodeForCBT * neighbour, int neighbour_index, int flag) {
    int i, j;
    NodeForCBT * tmp;

    if (flag == 1) {
        tmp = n;
        n = neighbour;
        neighbour = tmp;
        neighbour_index--;
    }
    NodeForCBT * parent = n->Parent;
    i = neighbour->NumOfKeys;
    neighbour->Keys[i] = parent->Keys[neighbour_index];
    i++;
    for (j = 0; j < n->NumOfKeys; j++) {
        neighbour->Keys[i] = n->Keys[j];
        neighbour->KeysNumInChild[i] = n->KeysNumInChild[j];
        neighbour->Pointers[i] = n->Pointers[j];
        tmp = neighbour->Pointers[i];
        if (tmp != nullptr) {
            tmp->Parent = neighbour;
        }
        i += 1;
    }
    neighbour->KeysNumInChild[i] = n->KeysNumInChild[j];
    neighbour->Pointers[i] = n->Pointers[j];
    tmp = neighbour->Pointers[i];
    if (tmp != nullptr) {
        tmp->Parent = neighbour;
    }
    neighbour->NumOfKeys = neighbour->NumOfKeys + n->NumOfKeys + 1;
    parent->KeysNumInChild[neighbour_index] = ComputeNumItems(neighbour); //neighbour->NumOfKeys

    DeleteFromNodeForCBT(n->Parent, neighbour_index);
}

int CountedBtree::getIndexInParent(NodeForCBT * n) {
    NodeForCBT * parent = n->Parent;
    for (int i = 0; i <= parent->NumOfKeys; i++) {
        if (parent->Pointers[i] == n) {
            return i;
        }
    }
    return -1;
}


void CountedBtree::redistributeLeafForCBTs(NodeForCBT * n, NodeForCBT * neighbour, int neighbour_index, int flag) const {
    int i, leftIndex, rightIndex;
    //    parent := n->Parent
    int n_idxInParent = getIndexInParent(n);

    if (flag == -1) { // neighbor (more keys) is on the left
        leftIndex = neighbour_index;
        NodeForCBT * parent = n->Parent;
        for (int j = n->NumOfKeys; j >= 1; j--) {
            n->Keys[j] = n->Keys[j-1];
        }
        n->Keys[0] = n->Parent->Keys[n_idxInParent-1];
        parent->Keys[leftIndex] = neighbour->Keys[neighbour->NumOfKeys-1];
        n->NumOfKeys++;

        neighbour->KeysNumInChild[neighbour->NumOfKeys] = 0;
        neighbour->NumOfKeys--;
        //n->NumOfKeys++

        parent = n->Parent;
        parent->KeysNumInChild[leftIndex]--;
        //parent->KeysNumInChild[leftIndex+1]++;

    } else { // neighbor is on the right (more keys) (neighbor_index == -1)
        rightIndex = neighbour_index;
        n->Keys[n->NumOfKeys] = n->Parent->Keys[neighbour_index-1];
        n->NumOfKeys++;

        NodeForCBT * parent = n->Parent;
        parent->Keys[neighbour_index-1] = neighbour->Keys[0];

        i = 0;
        for (; i < neighbour->NumOfKeys-1; i++) {
            neighbour->Keys[i] = neighbour->Keys[i+1];
        }
        i++;
        neighbour->NumOfKeys--;

        parent = n->Parent;
        //parent->KeysNumInChild[rightIndex]--;
        parent->KeysNumInChild[rightIndex-1]++;
    }

    NodeForCBT * c = n->Parent;
    NodeForCBT * parent = c->Parent;
    while (parent != nullptr) {
        for (i = 0; i < Capacity; i++) {
            if (parent->Pointers[i] == c) {
                parent->KeysNumInChild[i]--;
                c = c->Parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

void CountedBtree::redistributeNodeForCBTs(NodeForCBT * n, NodeForCBT * neighbour, int neighbour_index, int flag) const {
    int i, leftIndex, rightIndex;
    //    parent := n->Parent
    int n_idxInParent = getIndexInParent(n);

    if (flag == -1) { // neighbor (more keys) is on the left
        leftIndex = neighbour_index;
        NodeForCBT * parent = n->Parent;
        for (int j = n->NumOfKeys; j >= 1; j--) {
            n->Keys[j] = n->Keys[j-1];
        }
        n->Keys[0] = n->Parent->Keys[n_idxInParent-1];
        parent->Keys[leftIndex] = neighbour->Keys[neighbour->NumOfKeys-1];
        n->NumOfKeys++;
        for (int k = n->NumOfKeys; k >= 1; k--) {
            n->Pointers[k] = n->Pointers[k - 1];
            n->KeysNumInChild[k] = n->KeysNumInChild[k - 1];
        }
        n->Pointers[0] = neighbour->Pointers[neighbour->NumOfKeys];
        if (n->Pointers[0] != nullptr) {
            n->Pointers[0]->Parent = n;
        }
        n->KeysNumInChild[0] = neighbour->KeysNumInChild[neighbour->NumOfKeys];
        neighbour->Pointers[neighbour->NumOfKeys] = nullptr;
        neighbour->KeysNumInChild[neighbour->NumOfKeys] = 0;
        neighbour->NumOfKeys--;
        //n->NumOfKeys++

        parent = n->Parent;
        parent->KeysNumInChild[leftIndex] = ComputeNumItems(neighbour);
        parent->KeysNumInChild[leftIndex+1] = ComputeNumItems(n);

    } else { // neighbor is on the right (more keys) (neighbor_index == -1)
        rightIndex = neighbour_index;
        n->Keys[n->NumOfKeys] = n->Parent->Keys[neighbour_index-1];
        n->Pointers[n->NumOfKeys+1] = neighbour->Pointers[0];
        n->KeysNumInChild[n->NumOfKeys+1] = neighbour->KeysNumInChild[0];
        n->NumOfKeys++;
        if (n->Pointers[n->NumOfKeys] != nullptr) {
            n->Pointers[n->NumOfKeys]->Parent = n;
        }
        NodeForCBT * parent = n->Parent;
        parent->Keys[neighbour_index-1] = neighbour->Keys[0];

        i = 0;
        for (; i < neighbour->NumOfKeys-1; i++) {
            neighbour->Pointers[i] = neighbour->Pointers[i+1];
            neighbour->Keys[i] = neighbour->Keys[i+1];
            neighbour->KeysNumInChild[i] = neighbour->KeysNumInChild[i+1];
        }
        neighbour->Pointers[i] = neighbour->Pointers[i+1];
        neighbour->KeysNumInChild[i] = neighbour->KeysNumInChild[i+1];
        i++;
        neighbour->Pointers[i] = nullptr;
        neighbour->KeysNumInChild[i] = 0;
        neighbour->NumOfKeys--;

        parent = n->Parent;
        parent->KeysNumInChild[rightIndex] = ComputeNumItems(neighbour);
        parent->KeysNumInChild[rightIndex-1] = ComputeNumItems(n);
    }

    NodeForCBT * c = n->Parent;
    NodeForCBT * parent = c->Parent;
    while (parent != nullptr) {
        for (i = 0; i < Capacity; i++) {
            if (parent->Pointers[i] == c) {
                parent->KeysNumInChild[i]--;
                c = c->Parent;
                parent = c->Parent;
                break;
            }
        }
    }
}

int CountedBtree::ComputeNumItems(NodeForCBT * n) {
    int ans = n->NumOfKeys;
    for (int i = 0; i < n->NumOfKeys+1; i++) {
        ans += n->KeysNumInChild[i];
    }
    return ans;
}

void CountedBtree::Replace(int pos, int newKey) const {
    bool flag;
    NodeForCBT * c = Root;
    while (!c->IsLeaf) {
        flag = false;
        int i = 0;
        for (; i < c->NumOfKeys; i++) {
            if (pos <= c->KeysNumInChild[i]) {
                c = c->Pointers[i];
                flag = true;
                break;
            } else if (pos == c->KeysNumInChild[i]+1) {
                c->Keys[i] = newKey;
                return;
            } else {
                pos = pos - 1 - c->KeysNumInChild[i];
            }
        }
        if (!flag) {
            c = c->Pointers[c->NumOfKeys];
        }
    }
    c->Keys[pos-1] = newKey;
}

void CountedBtree::Reorder(int start, int end, int * newIDs) const {
    for (int i = start; i <= end; i++) {
        Replace(i, newIDs[i-start]);
    }
}


void CountedBtree::Swap(int start1, int end1, int start2, int end2) {
    //std::cout << "Here is CBT::Swap" << std::endl;
    int len1 = end1 - start1 + 1;
    int j = end2 + 1;
    for (int i = start1; i <= end1; i++) {
        int s = Query(start1);
        Insert(s, j);
        Delete(start1);
    }
    int k = start1;
    int q = start2 - len1;
    for (int i = start2 - len1; i <= end2-len1; i++) {
        int s = Query(q);
        Insert(s, k);
        Delete(q + 1);
        k++;
        q++;
    }
}

int * CountedBtree::RangeQuery(int start, int end, int * lenOfAns) const {
    int dastart = 1;
    int daend = NumKeys;
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
    int * ans = new int[realnum];
    for (int i = 0; i < realnum; ++i) {
        ans[i] = Query(realstart + i);
    }
    return ans;
}

int CountedBtree::Query(int pos) const {
    // printf("Here is CBT query\n");
    if (pos > NumKeys) {
        pos = NumKeys;
    }
    NodeForCBT * c = Root;
    int p = pos;
    if (c == nullptr) {
        return 0;
    }
    int num_while = 0;
    // printf("CBT query, before while\n");
    while (!c->IsLeaf) {
        // printf("num_while = %d\n", num_while);
        num_while ++;
        bool flag = false;
        int i = 0;
        for (; i < c->NumOfKeys; i++) {
            if (p <= c->KeysNumInChild[i]) {
                c = c->Pointers[i];
                flag = true;
                break;
            }
            if (p == c->KeysNumInChild[i]+1) {
                return c->Keys[i];
            }
            p = p - c->KeysNumInChild[i] - 1;
        }
        if (!flag) {
            c = c->Pointers[c->NumOfKeys];
        }
        if (c == nullptr) {
            printf("c is null\n")       ;
        }
    }
    return c->Keys[p-1];
}

NodeForCBT * CountedBtree::FindNodeForCBT(int pos, int * index) const {
    if (pos > NumKeys) {
        pos = NumKeys;
    }
    NodeForCBT * c = Root;
    int p = pos;
    if (c == nullptr) {
        *index = 0;
        return nullptr;
    }

    while (!c->IsLeaf) {
        bool flag = false;
        int i = 0;
        for (; i < c->NumOfKeys; i++) {
            if (p <= c->KeysNumInChild[i]) {
                c = c->Pointers[i];
                flag = true;
                break;
            }
            if (p == c->KeysNumInChild[i]+1) {
                * index = i;
                return c;
            }
            p = p - c->KeysNumInChild[i] - 1;
        }
        if (!flag) {
            c = c->Pointers[c->NumOfKeys];
        }
    }
    *index = p-1;
    return c;
}

void CountedBtree::Exchange(int a, int b) const {
    int idx1 = -1, idx2 = -1;
    NodeForCBT * n1 = FindNodeForCBT(a, &idx1);
    NodeForCBT * n2 = FindNodeForCBT(b, &idx2);
    int tmp = n1->Keys[idx1];
    n1->Keys[idx1] = n2->Keys[idx2];
    n2->Keys[idx2] = tmp;
}

void CountedBtree::Move(int start, int end, int des) {
    if (des > NumKeys) {
        des = NumKeys;
    }
    if (des < start) {
        int j = des;
        for (int i = start; i <= end; i++) {
            int s = Query(i);
            Insert(s, j);
            j++;
            Delete(i+1);
        }
        return;
    } else if (des >= start && des <= end+1) {
        return;
    } else {
        for (int i = start; i <= end; i ++) {
            int s = Query(start);
            Insert(s, des);
            Delete(start);
        }
        return;
    }
}

void CountedBtree::Delete(int rank) {
    int NodeForCBTNum = NumKeys;
    if (rank > NodeForCBTNum) {
        rank = NodeForCBTNum;
    }
    NumKeys--;

    NodeForCBT * c = Root;
    int p = rank;

    bool flag = false;
    while (!c->IsLeaf) {
        flag = false;
        int i = 0;
        for (; i < c->NumOfKeys; i++) {
            if (p <= c->KeysNumInChild[i]) {
                c = c->Pointers[i];
                flag = true;
                break;
            } else if (p == c->KeysNumInChild[i]+1) {
                int idx = -1;
                NodeForCBT * leaf = FindLastKeyInLeaf(c->Pointers[i], &idx);
                idx--;
                c->Keys[i] = leaf->Keys[idx];
                DeleteFromNodeForCBT(leaf, idx);
                return;
            }
            p -= c->KeysNumInChild[i] + 1;
        }
        if (!flag) {
            c = c->Pointers[c->NumOfKeys];
        }
    }

    DeleteFromLeafForCBT(c, p-1);
}

void CountedBtree::DeleteFromLeafForCBT(NodeForCBT * n, int idx) {
    if (!n->IsLeaf) {
        throw std::invalid_argument("in DeleteFromLeafForCBT, not a leaf\n");
    }
    for (int i = idx; i < n->NumOfKeys-1; i++) {
        n->Keys[i] = n->Keys[i+1];
    }
    n->Keys[n->NumOfKeys-1] = 0;
    n->NumOfKeys--;
    int min_keys = Capacity/2 - 1;
    if (n == Root) {
        adjustRootInDeletion();
        return;
    }
    if (n->NumOfKeys >= min_keys) {
        NodeForCBT * tmp = n;
        NodeForCBT * parent = tmp->Parent;
        while (parent != nullptr) {
            for (int i = 0; i <= parent->NumOfKeys; i++) {
                NodeForCBT * tp = parent->Pointers[i];
                if (tp == tmp) {
                    parent->KeysNumInChild[i]--;
                    tmp = parent;
                    parent = tmp->Parent;
                    break;
                }
            }
        }
        return;
    }

    int left_neighbour_index = getLeftNeighborIndex(n);
    int right_neighbour_index = getRightNeighborIndex(n);
    NodeForCBT * LeftNeighbour = nullptr, * RightNeighbour = nullptr;

    if (left_neighbour_index != -1) {
        LeftNeighbour = n->Parent->Pointers[left_neighbour_index];
    }
    if (right_neighbour_index != -1) {
        RightNeighbour = n->Parent->Pointers[right_neighbour_index];
    }

    if (LeftNeighbour == nullptr) {
        if (RightNeighbour->NumOfKeys > min_keys) {
            redistributeLeafForCBTs(n, RightNeighbour, right_neighbour_index, 1);
            return;
        } else {
            coalesceLeafForCBTs(n, RightNeighbour, right_neighbour_index, 1);
            return;
        }
    } else if (RightNeighbour == nullptr) {
        if (LeftNeighbour->NumOfKeys > min_keys) {
            redistributeLeafForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
            return;
        } else {
            coalesceLeafForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
            return;
        }
    } else if (LeftNeighbour->NumOfKeys > min_keys) {
        redistributeLeafForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
        return;
    } else if (RightNeighbour->NumOfKeys > min_keys) {
        redistributeLeafForCBTs(n, RightNeighbour, right_neighbour_index, 1);
        return;
    } else {
        coalesceLeafForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
    }
}



void CountedBtree::DeleteFromNodeForCBT(NodeForCBT * n, int idx) {
    if (!n->IsLeaf) {
        // delete n->Pointers[idx+1];
    }
    for (int i = idx; i < n->NumOfKeys-1; i++) {
        n->Keys[i] = n->Keys[i+1];
        n->Pointers[i+1] = n->Pointers[i+2];
        n->KeysNumInChild[i+1] = n->KeysNumInChild[i+2];
    }
    n->Keys[n->NumOfKeys-1] = 0;
    n->Pointers[n->NumOfKeys] = nullptr;
    n->KeysNumInChild[n->NumOfKeys] = 0;
    n->NumOfKeys--;
    int min_keys = Capacity/2 - 1;
    if (n == Root) {
        adjustRootInDeletion();
        return;
    }
    if (n->NumOfKeys >= min_keys) {
        NodeForCBT * tmp = n;
        NodeForCBT * parent = tmp->Parent;
        while (parent != nullptr) {
            for (int i = 0; i <= parent->NumOfKeys; i++) {
                NodeForCBT * tp = parent->Pointers[i];
                if (tp == tmp) {
                    parent->KeysNumInChild[i]--;
                    tmp = parent;
                    parent = tmp->Parent;
                    break;
                }
            }
        }
        return;
    }

    int left_neighbour_index = getLeftNeighborIndex(n);
    int right_neighbour_index = getRightNeighborIndex(n);
    NodeForCBT * LeftNeighbour = nullptr, * RightNeighbour = nullptr;

    if (left_neighbour_index != -1) {
        LeftNeighbour = n->Parent->Pointers[left_neighbour_index];
    }
    if (right_neighbour_index != -1) {
        RightNeighbour = n->Parent->Pointers[right_neighbour_index];
    }

    if (LeftNeighbour == nullptr) {
        if (RightNeighbour->NumOfKeys > min_keys) {
            redistributeNodeForCBTs(n, RightNeighbour, right_neighbour_index, 1);
            return;
        } else {
            coalesceNodeForCBTs(n, RightNeighbour, right_neighbour_index, 1);
            return;
        }
    } else if (RightNeighbour == nullptr) {
        if (LeftNeighbour->NumOfKeys > min_keys) {
            redistributeNodeForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
            return;
        } else {
            coalesceNodeForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
            return;
        }
    } else if (LeftNeighbour->NumOfKeys > min_keys) {
        redistributeNodeForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
        return;
    } else if (RightNeighbour->NumOfKeys > min_keys) {
        redistributeNodeForCBTs(n, RightNeighbour, right_neighbour_index, 1);
        return;
    } else {
        coalesceNodeForCBTs(n, LeftNeighbour, left_neighbour_index, -1);
    }
}

void CountedBtree::adjustRootInDeletion() {
    NodeForCBT *  new_root = nullptr;

    if (Root->NumOfKeys >= 1) {
        return;
    }
    //Root->NumOfKeys = 0
    if (!Root->IsLeaf) {
        new_root = Root->Pointers[0];
    }
    if (new_root == nullptr) {
        Root = nullptr;
        return;
    }
    // delete Root;
    new_root->Parent = nullptr;
    Root = new_root;
}
