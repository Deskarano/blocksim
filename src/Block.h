#ifndef BLOCKSIM_BLOCK_H
#define BLOCKSIM_BLOCK_H

#include "Transaction.h"

#include <vector>

class Block
{
public:
    Block(Transaction *tx_root, unsigned int tx_max);

    int add_tx(Transaction *tx);

    void update_hash();
    unsigned char *get_hash();

private:
    std::vector<Transaction *> *tx_pointers;
    unsigned int tx_max;
    unsigned int size;

    unsigned char *hash;
    unsigned char *prev_hash;
};

#endif //BLOCKSIM_BLOCK_H