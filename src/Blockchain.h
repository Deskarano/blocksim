#ifndef BLOCKSIM_BLOCKCHAIN_H
#define BLOCKSIM_BLOCKCHAIN_H

#include "Block.h"
#include "Wallet.h"
#include <vector>

class Blockchain
{
public:
    Blockchain(unsigned int num_wallets, unsigned int miner_wallet);
    ~Blockchain();

    int save(char *path);

private:
    std::vector<Block> *blocks;
    Wallet wallets[];
};


#endif //BLOCKSIM_BLOCKCHAIN_H