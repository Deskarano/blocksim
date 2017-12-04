#ifndef BLOCKSIM_BLOCKCHAIN_H
#define BLOCKSIM_BLOCKCHAIN_H

#include "../Block.h"
#include "InternalWallet.h"

#include <vector>
#include <unordered_map>

typedef struct
{
    time_t time_started;

    bool running;
    bool result_found;

    Block *block;
    unsigned int difficulty;
    unsigned int num_threads;

    unsigned int num_hashes;
    unsigned int result;
} miner_data_t;

class Blockchain
{
public:
    Blockchain(unsigned int miner_wallet);

    int receive_tx(Transaction *tx);

    void miner_start(unsigned int difficulty, unsigned int num_treads);
    void miner_stop();

    void confirm_next_block(unsigned int difficulty);

    std::vector<Block *> *get_block_pointers()
    { return block_pointers; }

    std::unordered_map<unsigned int, InternalWallet *> *get_wallet_pointers()
    { return wallet_pointers; }

    miner_data_t *get_miner_data()
    { return miner_data; }

private:
    void gen_next_block();

    void confirm_internal_wallets();

    void add_wallet(unsigned int ID);

    Block *current_block;
    std::vector<Block *> *block_pointers;
    std::unordered_map<unsigned int, InternalWallet *> *wallet_pointers;

    unsigned int miner_wallet;
    miner_data_t *miner_data;
};

#endif //BLOCKSIM_BLOCKCHAIN_H