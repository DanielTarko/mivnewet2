//
// Created by Shirah Hassan on 6/7/23.
//

#ifndef MIVNEWET2_CUSTOMER_H
#define MIVNEWET2_CUSTOMER_H


class Customer {
private:
    const int id;
    const int phone;
    bool isMember;
    double monthlyExpenses;

public:
    Customer(int id, int phone);
    Customer(const Customer& other) = default;
    ~Customer() = default;
    const int GetPhoneNumber() const;
    bool IsMember() const;
    int GetId() const;
    void setAsMember();
    void updateExpenses(int recordPurchases);
    double getMonthlyExpenses() const;
    void resetExpenses();
    int getId() const;

};


#endif //MIVNEWET2_CUSTOMER_H
