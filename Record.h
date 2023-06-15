//
// Created by Shirah Hassan on 6/7/23.
//

#ifndef MIVNEWET2_RECORD_H
#define MIVNEWET2_RECORD_H


class Record {
private:
    int id;
    int purchaseCount;
    int copyCount;

public:
    Record(int id, int copyCount);
    Record(const Record& other) = default;
    ~Record() = default;
    Record& operator=(const Record& other) = default;
    int GetPurchaseCount() const;
    void increasePurchaseCount();

};


#endif //MIVNEWET2_RECORD_H
