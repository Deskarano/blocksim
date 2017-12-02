#ifndef BLOCKSIM_BLOCKCHAIN_H
#define BLOCKSIM_BLOCKCHAIN_H

#include "Block.h"
#include "Wallet.h"
#include <vector>

class Blockchain
{
public:
    Blockchain(unsigned int miner_wallet);

    int receive_tx(Transaction *tx);

    void miner_start(unsigned int num_threads);
    void miner_stop();

    int save(char *path);

private:
    void update_internal_wallets();

    Block *current_block;
    std::vector<Block *> *blocks;
    std::vector<Wallet *> *wallets;
};


#endif //BLOCKSIM_BLOCKCHAIN_H