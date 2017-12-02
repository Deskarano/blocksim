#ifndef BLOCKSIM_BLOCK_H
#define BLOCKSIM_BLOCK_H

#include "Transaction.h"

#include <vector>

class Block
{
public:
    Block(unsigned char *prev_hash);

    void add_tx(Transaction *tx);

    void update_hash();
    unsigned char *get_hash();

    std::vector<Transaction *> *get_tx_pointers()
    { return tx_pointers; }

    unsigned int get_size()
    { return size; }

    void set_nonce(unsigned int nonce)
    { this->nonce = nonce; }

private:
    std::vector<Transaction *> *tx_pointers;
    unsigned int size;

    unsigned char *hash;
    unsigned char *prev_hash;
    unsigned int nonce;
};

#endif //BLOCKSIM_BLOCK_H