#ifndef UNION_FIND_H
#define UNION_FIND_H
#include <memory>

struct UFNode {
int id;
UFNode* parent;
int relativeHeight;
int nodeCount;
int absoluteHeight;
int treeHeight;
int column;
int purchaseCount;
};

class UnionFind {
private:
    int numberOfRecords;
    UFNode** nodesArray;

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