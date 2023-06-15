//
// Created by Shirah Hassan on 6/7/23.
//

#include "Record.h"
#include <iostream>

Record::Record(int id, int copyCount): id(id), purchaseCount(0), copyCount(copyCount){}

int Record::GetPurchaseCount() const {
    return purchaseCount;
}

void Record::increasePurchaseCount() {
    this->purchaseCount++;
}