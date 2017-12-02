#ifndef BLOCKSIM_TRANSACTION_H
#define BLOCKSIM_TRANSACTION_H

#include "Wallet.h"

class Transaction
{
public:
    Transaction(Wallet *to, Wallet *from, double amount, double fee);

    unsigned char *get_hash();
    void set_children(Transaction *left, Transaction *right);

private:
    Wallet *to;
    Wallet *from;
    double amount;
    double fee;

    Transaction *left;
    Transaction *right;
};


#endif //BLOCKSIM_TRANSACTION_H