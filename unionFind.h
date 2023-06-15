#ifndef UNION_FIND_H
#define UNION_FIND_H
#include <memory>
#include "Record.h"

struct UFNode {
int id;
std::shared_ptr<UFNode> next;
int relativeHeight;
int nodeCount;
int absoluteHeight;
int treeHeight;
int column;
int purchaseCount;
};

class UnionFind {
private:
    std::shared_ptr<UFNode>* nodesArray;
    int numberOfRecords;

public:
    UnionFind(int* records_stocks, int number_of_records);
    UnionFind() = default;
    ~UnionFind();
    int find(int id, int* column);
    int findAux(int id, int* rootId, int* column);

    void unite(int id1, int id2);
    int getRoot(int id);
    int getPurchaseCount(int id);
    void incrementPurchaseCount(int id);
    };

#endif //UNION_FIND_H