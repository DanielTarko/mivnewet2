#ifndef HASH_TABLE
#define HASH_TABLE
#include <memory>
#include "Customer.h"
#include "AVLTree.h"



const float MULTIPLICATION_RESIZE_FACTOR = 2;
const int INITIAL_SIZE = 11;
const float LOAD_FACTOR_THRESHOLD = 0.75;

class HashTable {
private:
    AVLTree<int, std::shared_ptr<Customer>>* table;
    int size;
    int elementCount;
    int hash(int key) const;
    void resize(int updatedSize);
public:
    HashTable();

    ~HashTable();

    void insert(std::shared_ptr<Customer> customer);

    std::shared_ptr<Customer> find(int id) const; //assumes that we know for sure that there is a user with this key so we should check before calling this

    bool contains(int id) const;

};

#endif //HASH_TABLE