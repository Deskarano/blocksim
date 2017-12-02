#include "Blockchain.h"

Blockchain::Blockchain(unsigned int num_wallets, unsigned int miner_wallet)
{
    this->blocks = new std::vector<Block>;

    Block *genesis_block = new Block();
    Transaction *genesis_tx = new Transaction(nullptr, nullptr, 0, 25);

    genesis_block->add_tx(genesis_tx);
}