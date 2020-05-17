#include "Transaction.h"
#include "../crypto/SHA256.h"

#include <cstring>
#include <iostream>

Transaction::Transaction(unsigned int from, unsigned int to, double amount, double fee)
{
    timestamp = time(nullptr);

    std::cout << "--blockchain_tx\t\tNew tx " << this << "\n";
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
    unsigned char wallets[2 * sizeof(int)];
    memcpy(wallets, &to, sizeof(int));
    memcpy(wallets + sizeof(int), &from, sizeof(int));
    unsigned char wallet_hash[32];
    SHA256(wallets, 2 * sizeof(int), wallet_hash);

    unsigned char values[2 * sizeof(double)];
    memcpy(values, &amount, sizeof(double));
    memcpy(values + sizeof(double), &fee, sizeof(double));
    unsigned char value_hash[32];
    SHA256(values, 2 * sizeof(double), value_hash);

    unsigned char left_hash[32];
    if(left == nullptr)
    {
        for(int i = 0; i < 32; i++)
        {
            left_hash[i] = 0;
        }
    }
    else
    {
        for(int i = 0; i < 32; i++)
        {
            left_hash[i] = left->get_hash()[i];
        }
    }

    unsigned char right_hash[32];
    if(right == nullptr)
    {
        for(int i = 0; i < 32; i++)
        {
            right_hash[i] = 0;
        }
    }
    else
    {
        for(int i = 0; i < 32; i++)
        {
            right_hash[i] = right->get_hash()[i];
        }
    }

    auto *concat_data = new unsigned char[128 + sizeof(timestamp)];

    for(int i = 0; i < 32; i++)
    {
        concat_data[i] = wallet_hash[i];
        concat_data[i + 32] = value_hash[i];
        concat_data[i + 64] = left_hash[i];
        concat_data[i + 96] = right_hash[i];
    }
    memcpy(concat_data + 128, &timestamp, sizeof(time_t));

    SHA256(concat_data, 128 + sizeof(time_t), hash);
    delete concat_data;

    std::cout << "--blockchain_tx\t\tNew tx hash for " << this << ": ";
    print_hash(hash);
}

unsigned char *Transaction::get_hash()
{
    return hash;
}