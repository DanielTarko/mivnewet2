//
// Created by Shirah Hassan on 6/7/23.
//

#include "recordsCompany.h"
#include <iostream>

RecordsCompany::RecordsCompany() : numOfRecords(0) {}

RecordsCompany::~RecordsCompany() = default;

StatusType RecordsCompany::newMonth(int *records_stocks, int number_of_records) {
    if (number_of_records < 0)
        return StatusType::INVALID_INPUT;

    try {
        if (month) {
            delete month;
        }
        month = new UnionFind(records_stocks, number_of_records);
    } catch (const bad_alloc &e) {
        return StatusType::ALLOCATION_ERROR;
    }
    numOfRecords = number_of_records;
    std::shared_ptr<Customer> *customersArray = new std::shared_ptr<Customer>[membersById.getSize()];
    membersById.populateArrayReverseOrder(customersArray);
    for (int i = 0; i < membersById.getSize(); i++) {
        customersArray[i]->resetExpenses();
    }
    delete[] customersArray;
    membersById.deleteExtras();
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::addCostumer(int c_id, int phone) {
    if (c_id < 0 || phone < 0) {
        return INVALID_INPUT;
    }

    if (customers.contains(c_id)) {
        return ALREADY_EXISTS;
    }

    std::shared_ptr<Customer> c = std::make_shared<Customer>(c_id, phone);

    try {
        customers.insert(c);
    } catch (const std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

Output_t<int> RecordsCompany::getPhone(int c_id) {//todo fix!
    if (c_id < 0) {
        return Output_t<int>(INVALID_INPUT);
    }

    if (!customers.contains(c_id)) {
        return Output_t<int>(DOESNT_EXISTS);
    }

    std::shared_ptr<Customer> c = customers.find(c_id);

    const int phoneNumber = c->GetPhoneNumber();
    return Output_t<int>(phoneNumber);
}

StatusType RecordsCompany::makeMember(int c_id) {
    if (c_id < 0) {
        return INVALID_INPUT;
    }

    if (!customers.contains(c_id)) {
        return DOESNT_EXISTS;
    }

    std::shared_ptr<Customer> c = customers.find(c_id);

    if (c->IsMember()) {
        return ALREADY_EXISTS;
    }

    c->setAsMember();
    try {
        membersById.insert(c_id, c);
    } catch (const std::bad_alloc &) {
        return ALLOCATION_ERROR;
    }

    return SUCCESS;
}

Output_t<bool> RecordsCompany::isMember(int c_id) {
    if (c_id < 0) {
        return Output_t<bool>(INVALID_INPUT);
    }


    if (!customers.contains(c_id)) {
        return Output_t<bool>(DOESNT_EXISTS);
    }

    std::shared_ptr<Customer> c = customers.find(c_id);

    if (c->IsMember()) {
        return Output_t<bool>(true);
    }

    return Output_t<bool>(false);

}

StatusType RecordsCompany::buyRecord(int c_id, int r_id) {
    if (c_id < 0 || r_id < 0) {
        return INVALID_INPUT;
    }

    if (!customers.contains(c_id)) {
        return DOESNT_EXISTS;
    }

    if (r_id >= numOfRecords) {
        return DOESNT_EXISTS;
    }
            
    int currentPurchaseCount = month->getPurchaseCount(r_id);

    std::shared_ptr<Customer> c = customers.find(c_id);

    if (c->IsMember()) {
        c->updateExpenses(currentPurchaseCount);
    }

    month->incrementPurchaseCount(r_id);
    return SUCCESS;
}

StatusType RecordsCompany::addPrize(int c_id1, int c_id2, double amount) {
    if (c_id1 < 0 || c_id2 < c_id1 || amount <= 0) {
        return INVALID_INPUT;
    }
    membersById.addExtra(c_id2 - 1, amount);
    membersById.addExtra(c_id1 - 1, -amount);

    return SUCCESS;

}

Output_t<double> RecordsCompany::getExpenses(int c_id) {
    if (c_id < 0) {
        return Output_t<double>(INVALID_INPUT);
    }

    if (!membersById.contains(c_id)) {
        return Output_t<double>(DOESNT_EXISTS);
    }

    std::shared_ptr<Customer> c = membersById.find(c_id);

    double totalExpenses = c->getMonthlyExpenses();
    double discount = membersById.getExtraSum(c_id);
    return Output_t<double>(totalExpenses - discount);

}

StatusType RecordsCompany::putOnTop(int r_id1, int r_id2) {
    if (r_id1 < 0 || r_id2 < 0)
        return StatusType::INVALID_INPUT;

    if (r_id1 >= numOfRecords || r_id2 >= numOfRecords)
        return StatusType::DOESNT_EXISTS;

    if (month->getRoot(r_id1) == month->getRoot(r_id2))
        return StatusType::FAILURE;

    month->unite(r_id1, r_id2);
    return StatusType::SUCCESS;
}

StatusType RecordsCompany::getPlace(int r_id, int *column, int *hight) {
    if (r_id < 0 || column == NULL || hight == NULL)
        return StatusType::INVALID_INPUT;

    if (r_id >= numOfRecords)
        return StatusType::DOESNT_EXISTS;

    *hight = month->find(r_id, column);
    return StatusType::SUCCESS;
}