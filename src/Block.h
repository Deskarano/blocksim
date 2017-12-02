#ifndef BLOCKSIM_BLOCK_H
#define BLOCKSIM_BLOCK_H

#include <vector>
#include "Transaction.h"

class Block
{
public:
    Block();

    int add_tx(Transaction *tx);

private:
    std::vector<Transaction> *tx;
};

#endif //BLOCKSIM_BLOCK_H