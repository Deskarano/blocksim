#include "Blockchain.h"

#include <cstddef>
#include <iostream>

#include "InternalWallet.h"

Block *gen_next_block(Block *prev_block, unsigned int miner_wallet)
{
    Transaction *reward_tx = new Transaction(miner_wallet, NULL, 25, 0);
    Block *result = new Block(reward_tx, prev_block->get_hash());

    return result;
}

Blockchain::Blockchain(unsigned int miner_wallet)
{
    this->blocks = new std::vector<Block *>;
    this->wallets = new std::unordered_map<unsigned int, InternalWallet *>;

    Transaction *genesis_tx = new Transaction(miner_wallet, NULL, 25, 0);
    Block *genesis_block = new Block(genesis_tx, nullptr);
    blocks->push_back(genesis_block);

    current_block = gen_next_block(genesis_block, miner_wallet);

    InternalWallet *wallet = new InternalWallet();

    std::pair<unsigned int, InternalWallet *> wallet_pair;
    wallet_pair.first = miner_wallet;
    wallet_pair.second = wallet;
    wallets->insert(wallet_pair);

    this->miner_wallet = miner_wallet;
}

int Blockchain::receive_tx(Transaction *tx)
{
    if(wallets->find(tx->get_to()) == wallets->end())
    {
        InternalWallet *wallet = new InternalWallet();

        std::pair<unsigned int, InternalWallet *> wallet_pair;
        wallet_pair.first = tx->get_to();
        wallet_pair.second = wallet;
        wallets->insert(wallet_pair);
    }

    if(wallets->find(tx->get_from()) == wallets->end())
    {
        InternalWallet *wallet = new InternalWallet();

        std::pair<unsigned int, InternalWallet *> wallet_pair;
        wallet_pair.first = tx->get_from();
        wallet_pair.second = wallet;
        wallets->insert(wallet_pair);
    }

    if (tx->get_amount() + tx->get_fee() < wallets->at(tx->get_from())->get_balance())
    {
        wallets->at(tx->get_to())->change_unconfirmed_balance(tx->get_amount());
        wallets->at(tx->get_from())->change_unconfirmed_balance(-1 * tx->get_amount());

        wallets->at(miner_wallet)->change_unconfirmed_balance(tx->get_fee());
        wallets->at(tx->get_from())->change_unconfirmed_balance(-1 * tx->get_fee());

        current_block->add_tx(tx);
        return 0;
    }
    else
    {
        return -1;
    }
}

/*
void Blockchain::confirm_internal_wallets(Block *block)
{
    std::vector<Transaction *> *tx_pointers = block->get_tx_pointers();
    Transaction *current_tx;

    unsigned int miner_wallet;

    for(int i = 0; i < block->get_size(); i++)
    {
        current_tx = tx_pointers->at(i);

        unsigned int to = current_tx->get_to();
        unsigned int from = current_tx->get_from();
        double amount = current_tx->get_amount();
        double fee = current_tx->get_fee();

        if(i == 0)
        {
            wallets->at(to)->change_balance(amount);
            miner_wallet = to;
        }
        else
        {
            wallets->at(to)->change_balance(amount);
            wallets->at(from)->change_balance(-amount);

            wallets->at(miner_wallet)->change_balance(fee);
            wallets->at(from)->change_balance(-fee);
        }
    }
}
 */