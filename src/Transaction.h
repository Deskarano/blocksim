#ifndef BLOCKSIM_TRANSACTION_H
#define BLOCKSIM_TRANSACTION_H

#include <ctime>

class Transaction
{
public:
    Transaction(unsigned int from, unsigned int to, double amount, double fee);

    void update_hash();
    unsigned char *get_hash();

    void set_left_child(Transaction *left)
    { this->left = left; }

    void set_right_child(Transaction *right)
    { this->right = right; }

    time_t get_timestamp()
    { return timestamp; }

    unsigned int get_from()
    { return from; }

    unsigned int get_to()
    { return to; }

    double get_amount()
    { return amount; }

    double get_fee()
    { return fee; }

private:
    time_t timestamp;

    unsigned int from;
    unsigned int to;

    double amount;
    double fee;

    Transaction *left;
    Transaction *right;

    unsigned char *hash;
};


#endif //BLOCKSIM_TRANSACTION_H