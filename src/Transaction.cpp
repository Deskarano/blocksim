#include "Transaction.h"
#include "SHA256.h"

#include <cstring>

Transaction::Transaction(Wallet *to, Wallet *from, double amount, double fee)
{
    this->to = to;
    this->from = from;
    this->amount = amount;
    this->fee = fee;
}

hash_t Transaction::get_hash()
{
    char *wallets = new char[2];
    wallets[0] = to->get_ID();
    wallets[1] = to->get_ID();

    hash_t wallet_hash = SHA256(wallets);
    delete wallets;

    char *values = new char[2 * sizeof(double)];
    memcpy(values, &amount, sizeof(double));
    memcpy(values + sizeof(double), &fee, sizeof(double));

    hash_t value_hash = SHA256(values);
    delete values;

    hash_t left_hash;
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

    hash_t right_hash;
    if(right == nullptr)
    {
        right_hash = new unsigned char[32];
        for(int i =
                0; i < 32; i++)
        {
            right_hash[i] = 0;
        }
    }
    else
    {
        right_hash = right->get_hash();
    }

    char *concat_data = new char[128];

    for(int i = 0; i < 32; i++)
    {
        concat_data[i] = wallet_hash[i];
        concat_data[i + 32] = value_hash[i];

        concat_data[i + 64] = left_hash[i];
        concat_data[i + 96] = right_hash[i];
    }

    hash_t final = SHA256(concat_data);
    delete concat_data;

    delete wallet_hash;
    delete value_hash;
    delete left_hash;
    delete right_hash;

    return final;
}

void Transaction::set_children(Transaction *left, Transaction *right)
{
    this->left = left;
    this->right = right;
}