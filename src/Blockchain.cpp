#include "Blockchain.h"

Blockchain::Blockchain(unsigned int num_wallets, unsigned int miner_wallet, unsigned int max_tx)
{
    this->wallets = new std::vector<Wallet *>;

    for(int i = 0; i < num_wallets; i++)
    {
        wallets->push_back(new Wallet(i));
    }

    this->blocks = new std::vector<Block *>;

    Transaction *genesis_tx = new Transaction(wallets->at(miner_wallet), nullptr, 25, 0);
    Block *genesis_block = new Block(genesis_tx, max_tx);

    blocks->push_back(genesis_block);
}