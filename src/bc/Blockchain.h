#ifndef BLOCKSIM_BLOCKCHAIN_H
#define BLOCKSIM_BLOCKCHAIN_H

#include "../Block.h"
#include "InternalWallet.h"

#include <vector>
#include <unordered_map>
#include <thread>

class Blockchain
{
public:
    Blockchain(unsigned int miner_wallet);

    void add_wallet(unsigned int ID);
    int receive_tx(Transaction *tx);

    void miner_start(unsigned int num_threads);
    void miner_stop();

    int save(char *path);

private:
    void gen_next_block();
    void confirm_internal_wallets(Block *block);

    Block *current_block;
    std::vector<Block *> *blocks;
    std::unordered_map<unsigned int, InternalWallet *> *wallets;

    unsigned int miner_wallet;

    std::vector<std::thread *> *threads;
};


#endif //BLOCKSIM_BLOCKCHAIN_H