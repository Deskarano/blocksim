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

    time_t get_timestamp()
    { return timestamp; }

    unsigned char *get_block_hash()
    { return block_hash; }

    unsigned char *get_prev_hash()
    { return prev_hash; }

    std::vector<Transaction *> *get_tx_pointers()
    { return tx_pointers; }

    unsigned int get_nonce()
    { return nonce; }

    unsigned int get_size()
    { return size; }

    void set_nonce(unsigned int nonce)
    { this->nonce = nonce; }

private:
    time_t timestamp;

    std::vector<Transaction *> *tx_pointers;
    unsigned int size;

    unsigned char *block_hash;
    unsigned char *prev_hash;
    unsigned int nonce;
};

#endif //BLOCKSIM_BLOCK_H