#include <cstring>
#include "Block.h"
#include "crypto/SHA256.h"

Block::Block(Transaction *tx_root, unsigned char *prev_hash)
{
    this->tx_pointers = new std::vector<Transaction *>;
    tx_pointers->push_back(tx_root);

    this->prev_hash = prev_hash;
}

void Block::add_tx(Transaction *tx)
{
    tx->update_hash();
    tx_pointers->push_back(tx);

    unsigned int index = (size - 1) / 2;

    if(2 * index + 1 == size)
    {
        tx_pointers->at(index)->set_left_child(tx_pointers->at(size));
    }
    else
    {
        tx_pointers->at(index)->set_right_child(tx_pointers->at(size));
    }

    while(index != 0)
    {
        tx_pointers->at(index)->update_hash();
        index = (index - 1) / 2;
    }

    size++;
    update_hash();
}

void Block::update_hash()
{
    char *hash_data = new char[64];
    std::memcpy(hash_data, prev_hash, 32);
    std::memcpy(hash_data + 32, tx_pointers->at(0)->get_hash(), 32);

    hash = SHA256(hash_data);
    delete hash_data;
}

unsigned char *Block::get_hash()
{
    if(hash == nullptr)
    {
        update_hash();
    }

    return hash;
}