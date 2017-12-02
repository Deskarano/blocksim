#include <cstddef>
#include "Blockchain.h"

void apply_block_to_wallets(Block *block)
{
    std::vector<Transaction *> *tx_pointers = block->get_tx_pointers();
    Transaction *current_tx;

    for(int i = 0; i < block->get_size(); i++)
    {
        current_tx = tx_pointers->at(i);

        unsigned int to = current_tx->get_to();
        unsigned int from = current_tx->get_from();
        double amount = current_tx->get_amount();
        double fee = current_tx->get_fee();

        if(i == 0)
        {

        }
    }
}

Block *gen_next_block(Block *prev_block, unsigned int miner_wallet)
{
    Transaction *reward_tx = new Transaction(miner_wallet, NULL, 25, 0);
    Block *result = new Block(reward_tx, prev_block->get_hash());

    return result;
}

Blockchain::Blockchain(unsigned int miner_wallet)
{
    this->blocks = new std::vector<Block *>;
    this->wallets = new std::vector<Wallet *>;

    Transaction *genesis_tx = new Transaction(miner_wallet, NULL, 25, 0);
    Block *genesis_block = new Block(genesis_tx, nullptr);

    blocks->push_back(genesis_block);

    current_block = gen_next_block(genesis_block, miner_wallet);
}

int Blockchain::receive_tx(Transaction *tx)
{
    //TODO: have to decide how to check available funds
}