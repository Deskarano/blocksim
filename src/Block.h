#ifndef BLOCKSIM_BLOCK_H
#define BLOCKSIM_BLOCK_H

#include "Transaction.h"

#include <vector>

class Block
{
public:
    Block(Transaction *tx_root, unsigned char *prev_hash);

    void add_tx(Transaction *tx);

    void update_hash();
    unsigned char *get_hash();

    unsigned int get_size()
    { return size; }

    std::vector<Transaction *> *get_tx_pointers()
    { return tx_pointers; }

private:
    std::vector<Transaction *> *tx_pointers;
    unsigned int size;

    unsigned char *hash;
    unsigned char *prev_hash;
};

#endif //BLOCKSIM_BLOCK_H