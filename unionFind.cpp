#include "unionFind.h"
#include <iostream>

UnionFind::UnionFind(int* records_stocks, int number_of_records) {
    nodesArray = new std::shared_ptr<UFNode>[number_of_records*sizeof(std::shared_ptr<UFNode>)]();
    numberOfRecords = number_of_records;
    for (int i = 0; i < number_of_records; i++) {
        nodesArray[i] = std::make_shared<UFNode>();
        nodesArray[i]->id = i;
        nodesArray[i]->relativeHeight = 0;
        nodesArray[i]->nodeCount = 1;
        nodesArray[i]->absoluteHeight = 0;
        nodesArray[i]->treeHeight = records_stocks[i];
        nodesArray[i]->column = i;
        nodesArray[i]->next = nullptr;
        nodesArray[i]->purchaseCount = 0;
    }
}

UnionFind::~UnionFind() {
    delete[] nodesArray;
}

int UnionFind::find(int id, int* column) {
    int rootId;
    return findAux(id, &rootId, column) + nodesArray[rootId]->relativeHeight;
}

int UnionFind::findAux(int id, int* rootId, int* column) {
    std::shared_ptr<UFNode> node = nodesArray[id];
    if (!node->next) {
        *rootId = node->id;
        *column = node->column;
        return 0;
    }
    int heightFromRoot = findAux(node->next->id, rootId, column) + node->relativeHeight;
    node->relativeHeight = heightFromRoot;
    node->next = nodesArray[*rootId];
    return heightFromRoot;
}

int UnionFind::getRoot(int id) {
    return nodesArray[id]->next ? getRoot(nodesArray[id]->next->id) : id;
}

int UnionFind::getPurchaseCount(int id) {
    return nodesArray[id]->purchaseCount;
}

void UnionFind::incrementPurchaseCount(int id) {
    nodesArray[id]->purchaseCount++;
}

void UnionFind::unite(int id1, int id2) {
    std::shared_ptr<UFNode> root1 = nodesArray[getRoot(id1)];
    std::shared_ptr<UFNode> root2 = nodesArray[getRoot(id2)];

    root1->column = root2->column;
    root1->absoluteHeight += root2->treeHeight;

    std::shared_ptr<UFNode> newRoot = root1->nodeCount > root2->nodeCount ? root1 : root2;
    std::shared_ptr<UFNode> oldRoot = root1->nodeCount > root2->nodeCount ? root2 : root1;
    
    oldRoot->next = newRoot;
    newRoot->treeHeight += oldRoot->treeHeight;
    oldRoot->relativeHeight = oldRoot->absoluteHeight - newRoot->absoluteHeight;
    newRoot->relativeHeight = newRoot->absoluteHeight;
    newRoot->nodeCount += oldRoot->nodeCount;

}