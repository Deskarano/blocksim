#include "Transaction.h"
#include "crypto/SHA256.h"

#include <cstring>

Transaction::Transaction(unsigned int to, unsigned int from, double amount, double fee)
{
    this->to = to;
    this->from = from;
    this->amount = amount;
    this->fee = fee;
}

void Transaction::update_hash()
{
    char *wallets = new char[2 * sizeof(int)];
    memcpy(wallets, &to, sizeof(int));
    memcpy(wallets + sizeof(int), &from, sizeof(int));

    hash_t wallet_hash = SHA256(wallets);
    delete wallets;

    char *values = new char[2 * sizeof(double)];
    memcpy(values, &amount, sizeof(double));
    memcpy(values + sizeof(double), &fee, sizeof(double));

    hash_t value_hash = SHA256(values);
    delete values;

    hash_t left_hash;
    if (left == nullptr)
    {
        left_hash = new unsigned char[32];
        for (int i = 0; i < 32; i++)
        {
            left_hash[i] = 0;
        }
    }
    else
    {
        left_hash = left->get_hash();
    }

    hash_t right_hash;
    if (right == nullptr)
    {
        right_hash = new unsigned char[32];
        for (int i = 0; i < 32; i++)
        {
            right_hash[i] = 0;
        }
    }
    else
    {
        right_hash = right->get_hash();
    }

    char *concat_data = new char[128];

    for (int i = 0; i < 32; i++)
    {
        concat_data[i] = wallet_hash[i];
        concat_data[i + 32] = value_hash[i];

        concat_data[i + 64] = left_hash[i];
        concat_data[i + 96] = right_hash[i];
    }

    delete concat_data;
    delete wallet_hash;
    delete value_hash;
    delete left_hash;
    delete right_hash;

    hash = SHA256(concat_data);
}

hash_t Transaction::get_hash()
{
    if(hash == nullptr)
    {
        update_hash();
    }

    return hash;
}

void Transaction::set_left_child(Transaction *left)
{
    this->left = left;
}

void Transaction::set_right_child(Transaction *right)
{
    this->right = right;
}