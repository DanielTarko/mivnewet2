#include "unionFind.h"
#include <iostream>

UnionFind::UnionFind(int* records_stocks, int number_of_records) {
    nodesArray = new UFNode*[number_of_records]();
    numberOfRecords = number_of_records;
    for (int i = 0; i < number_of_records; i++) {
        nodesArray[i] = new UFNode();
        nodesArray[i]->id = i;
        nodesArray[i]->relativeHeight = 0;
        nodesArray[i]->nodeCount = 1;
        nodesArray[i]->absoluteHeight = 0;
        nodesArray[i]->treeHeight = records_stocks[i];
        nodesArray[i]->column = i;
        nodesArray[i]->parent = nullptr;
        nodesArray[i]->purchaseCount = 0;
    }
}

UnionFind::~UnionFind() {
    for (int i = 0; i < numberOfRecords; i++) {
        delete nodesArray[i];
    }
    delete[] nodesArray;
}

int UnionFind::find(int id, int* column) {
    int rootId;
    return findAux(id, &rootId, column) + nodesArray[rootId]->relativeHeight;
}

int UnionFind::findAux(int id, int* rootId, int* column) {
    UFNode* node = nodesArray[id];
    if (!node->parent) {
        *rootId = node->id;
        *column = node->column;
        return 0;
    }
    int heightFromRoot = findAux(node->parent->id, rootId, column) + node->relativeHeight;
    node->relativeHeight = heightFromRoot;
    node->parent = nodesArray[*rootId];
    return heightFromRoot;
}

int UnionFind::getRoot(int id) {
    return nodesArray[id]->parent ? getRoot(nodesArray[id]->parent->id) : id;
}

int UnionFind::getPurchaseCount(int id) {
    return nodesArray[id]->purchaseCount;
}

void UnionFind::incrementPurchaseCount(int id) {
    nodesArray[id]->purchaseCount++;
}

void UnionFind::unite(int id1, int id2) {
    UFNode* root1 = nodesArray[getRoot(id1)];
    UFNode* root2 = nodesArray[getRoot(id2)];

    root1->column = root2->column;
    root1->absoluteHeight += root2->treeHeight;

    UFNode* newRoot = root1->nodeCount > root2->nodeCount ? root1 : root2;
    UFNode* oldRoot = root1->nodeCount > root2->nodeCount ? root2 : root1;
    
    oldRoot->parent = newRoot;
    newRoot->treeHeight += oldRoot->treeHeight;
    oldRoot->relativeHeight = oldRoot->absoluteHeight - newRoot->absoluteHeight;
    newRoot->relativeHeight = newRoot->absoluteHeight;
    newRoot->nodeCount += oldRoot->nodeCount;

}