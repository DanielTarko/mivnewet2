#include <iostream>
#include "hashTable.h"
#include "utilesWet2.h"


HashTable::HashTable() { //maybe we need a try and catch here
    size = INITIAL_SIZE;
    table = new AVLTree<int, std::shared_ptr<Customer>>[size*sizeof(AVLTree<int, std::shared_ptr<Customer>>)]();
    elementCount = 0;
}

HashTable::~HashTable() {
    delete[] table;
}

int HashTable::hash(int key) const {
    return key % size;
}

void HashTable::resize(int updatedSize) {
    int oldSize = size;
    size = updatedSize;
    AVLTree<int, std::shared_ptr<Customer>>* newTable = new AVLTree<int, std::shared_ptr<Customer>>[updatedSize*sizeof(AVLTree<int, std::shared_ptr<Customer>>)];

    for (int i = 0; i < oldSize; i++) {
        std::shared_ptr<Customer> *sortedCustomersArray = new std::shared_ptr<Customer>[table[i].getSize()*sizeof(std::shared_ptr<Customer>)];
        table[i].populateArrayReverseOrder(sortedCustomersArray);

        for (int j = 0; j < table[i].getSize(); j++) {
            int newIndex = hash(sortedCustomersArray[j]->getId());
            newTable[newIndex].insert(sortedCustomersArray[j]->getId(), sortedCustomersArray[j]);
        }

        delete[] sortedCustomersArray;
    }

    delete[] table;
    table = newTable;
}

void HashTable::insert(std::shared_ptr<Customer> customer) {
    if (static_cast<float>(elementCount) / size >= LOAD_FACTOR_THRESHOLD) {
        int updatedSize = size * MULTIPLICATION_RESIZE_FACTOR;
        resize(updatedSize);
    }

    int index = hash(customer->getId());
    table[index].insert(customer->getId(), customer);
    elementCount++;
}

std::shared_ptr<Customer> HashTable::find(int id) const { //assumes that we know for sure that there is a user with this key so we should check before calling this
    int index = hash(id);
    return table[index].find(id);
}

bool HashTable::contains(int id) const {
    int index = hash(id);
    return table[index].contains(id);
}