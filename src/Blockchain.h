#ifndef BLOCKSIM_BLOCKCHAIN_H
#define BLOCKSIM_BLOCKCHAIN_H

#include "Block.h"
#include "Wallet.h"
#include <vector>

class Blockchain
{
public:
    Blockchain(unsigned int num_wallets, unsigned int miner_wallet, unsigned int max_tx);
    ~Blockchain();

    int save(char *path);

private:
    std::vector<Block *> *blocks;
    std::vector<Wallet *> *wallets;
};


#endif //BLOCKSIM_BLOCKCHAIN_H