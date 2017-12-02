#include <cstring>
#include <iostream>
#include "Block.h"
#include "crypto/SHA256.h"

Block::Block(unsigned char *prev_hash)
{
    printf("New block %p with prev_hash ", this);
    print_hash(prev_hash);
    this->tx_pointers = new std::vector<Transaction *>;

    this->size = 0;
    this->prev_hash = prev_hash;
    this->nonce = 0;
}

void Block::add_tx(Transaction *tx)
{
    printf("Adding tx %p to block %p\n", tx, this);
    tx_pointers->push_back(tx);

    if(size > 0)
    {
        unsigned int index = (size - 1) / 2;

        if (2 * index + 1 == size)
        {
            tx_pointers->at(index)->set_left_child(tx_pointers->at(size));
        }
        else
        {
            tx_pointers->at(index)->set_right_child(tx_pointers->at(size));
        }

        while (index != 0)
        {
            tx_pointers->at(index)->update_hash();
            index = (index - 1) / 2;
        }

        tx_pointers->at(0)->update_hash();
    }

    size++;
    update_hash();
}

void Block::update_hash()
{
    delete hash;
    char *hash_data = new char[96];
    std::memcpy(hash_data, prev_hash, 32);
    std::memcpy(hash_data + 32, tx_pointers->at(0)->get_hash(), 32);
    std::memcpy(hash_data + 64, &nonce, 32);

    hash = SHA256(hash_data, 96);

    printf("New block hash for %p: ", this);
    print_hash(hash);

    delete hash_data;
}

unsigned char *Block::get_hash()
{
    if (hash == nullptr)
    {
        update_hash();
    }

    return hash;
}