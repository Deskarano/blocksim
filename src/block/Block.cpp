#include "Block.h"
#include "../crypto/SHA256.h"

#include <cstring>
#include <iostream>

Block::Block(unsigned char *prev_hash)
{
    timestamp = time(nullptr);

    std::cout << "--blockchain_block\tNew block " << this << "\n";
    this->tx_pointers = new std::vector<Transaction *>;

    this->size = 0;
    this->prev_hash = prev_hash;
    this->nonce = 0;
}

void Block::add_tx(Transaction *tx)
{
    std::cout << "--blockchain_block\tAdding tx " << tx << "to block " << this << "\n";
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
    auto hash_data = new unsigned char[64 + sizeof(unsigned int) + sizeof(time_t)];

    std::memcpy(hash_data, prev_hash, 32);
    std::memcpy(hash_data + 32, tx_pointers->at(0)->get_hash(), 32);
    std::memcpy(hash_data + 64, &nonce, sizeof(unsigned int));
    std::memcpy(hash_data + 64 + sizeof(unsigned int), &timestamp, sizeof(time_t));

    block_hash = SHA256(hash_data, 64 + sizeof(unsigned int) + sizeof(time_t));

    std::cout << "--blockchain_block\tNew block hash for " << this << ": ";
    print_hash(block_hash);

    delete hash_data;
}