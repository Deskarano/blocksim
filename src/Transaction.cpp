#include "Transaction.h"
#include "crypto/SHA256.h"

#include <cstring>
#include <iostream>

Transaction::Transaction(unsigned int from, unsigned int to, double amount, double fee)
{
    printf("New tx: %p\n", this);
    this->from = from;
    this->to = to;

    this->amount = amount;
    this->fee = fee;

    this->left = nullptr;
    this->right = nullptr;

    update_hash();
}

void Transaction::update_hash()
{
    auto *wallets = new unsigned char[2 * sizeof(int)];
    memcpy(wallets, &to, sizeof(int));
    memcpy(wallets + sizeof(int), &from, sizeof(int));

    unsigned char *wallet_hash = SHA256(wallets, 2 * sizeof(int));
    delete wallets;

    auto *values = new unsigned char[2 * sizeof(double)];
    memcpy(values, &amount, sizeof(double));
    memcpy(values + sizeof(double), &fee, sizeof(double));

    unsigned char *value_hash = SHA256(values, 2 * sizeof(double));
    delete values;

    unsigned char *left_hash;
    if(left == nullptr)
    {
        left_hash = new unsigned char[32];
        for(int i = 0; i < 32; i++)
        {
            left_hash[i] = 0;
        }
    }
    else
    {
        left_hash = left->get_hash();
    }

    unsigned char *right_hash;
    if(right == nullptr)
    {
        right_hash = new unsigned char[32];
        for(int i = 0; i < 32; i++)
        {
            right_hash[i] = 0;
        }
    }
    else
    {
        right_hash = right->get_hash();
    }

    auto *concat_data = new unsigned char[128];

    for(int i = 0; i < 32; i++)
    {
        concat_data[i] = wallet_hash[i];
        concat_data[i + 32] = value_hash[i];

        concat_data[i + 64] = left_hash[i];
        concat_data[i + 96] = right_hash[i];
    }

    hash = SHA256(concat_data, 128);

    //TODO: fix this memory mess
    //delete concat_data;
    //delete wallet_hash;
    //delete value_hash;
    //delete left_hash;
    //delete right_hash;

    printf("New tx hash for %p: ", this);
    print_hash(hash);
}

unsigned char *Transaction::get_hash()
{
    if(!hash)
    {
        update_hash();
    }

    return hash;
}