#ifndef BLOCKSIM_BLOCKCHAIN_H
#define BLOCKSIM_BLOCKCHAIN_H

#include "../Block.h"
#include "InternalWallet.h"

#include <vector>
#include <unordered_map>

typedef struct
{
    bool run;

    Block *block;
    unsigned int difficulty;
    unsigned int num_threads;

    unsigned int num_hashes_tried;
    unsigned int result;
} miner_data_t;

class Blockchain
{
public:
    Blockchain(unsigned int miner_wallet);

    void add_wallet(unsigned int ID);
    int receive_tx(Transaction *tx);

    void miner_start(unsigned int difficulty, unsigned int num_treads);
    void miner_stop();

    void confirm_next_block(unsigned int difficulty);

    miner_data_t *get_miner_data()
    { return miner_data; }

    int save(char *path);

private:
    void gen_next_block();
    void confirm_internal_wallets();


    Block *current_block;
    std::vector<Block *> *blocks;
    std::unordered_map<unsigned int, InternalWallet *> *wallets;

    unsigned int miner_wallet;

    miner_data_t *miner_data;

};


#endif //BLOCKSIM_BLOCKCHAIN_H