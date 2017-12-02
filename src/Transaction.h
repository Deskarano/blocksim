#ifndef BLOCKSIM_TRANSACTION_H
#define BLOCKSIM_TRANSACTION_H

#include "Wallet.h"

class Transaction
{
public:
    Transaction(Wallet *to, Wallet *from, double amount, double fee);

    void update_hash();
    unsigned char *get_hash();
    void set_left_child(Transaction *left);
    void set_right_child(Transaction *right);

private:
    Wallet *to;
    Wallet *from;
    double amount;
    double fee;

    Transaction *left;
    Transaction *right;

    unsigned char *hash;
};


#endif //BLOCKSIM_TRANSACTION_H