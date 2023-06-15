//
// Created by Shirah Hassan on 6/7/23.
//

#include "Customer.h"
#include <iostream>

Customer::Customer(int id, int phone): id(id), phone(phone), isMember(false), monthlyExpenses(0) {}

const int Customer::GetPhoneNumber() const {
    return phone;
}

bool Customer::IsMember() const {
    return isMember;
}

int Customer::GetId() const {
    return id;
}

void Customer::setAsMember() {
    isMember = true;
}

void Customer::updateExpenses(int recordPurchases) {
    monthlyExpenses += (100 + recordPurchases);
}

double Customer::getMonthlyExpenses() const {
    return monthlyExpenses;
}

void Customer::resetExpenses() {
    monthlyExpenses = 0;
}

int Customer::getId() const {
    return id;
}