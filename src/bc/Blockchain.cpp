#include "Blockchain.h"

#include <cstddef>
#include <iostream>
#include <random>

void Blockchain::gen_next_block()
{
    current_block = new Block(current_block->get_hash());
    Transaction *reward = new Transaction(0, miner_wallet, 25, 0);

    receive_tx(reward);
}

Blockchain::Blockchain(unsigned int miner_wallet)
{
    this->blocks = new std::vector<Block *>;
    this->wallets = new std::unordered_map<unsigned int, InternalWallet *>;
    this->miner_wallet = miner_wallet;

    unsigned char *zero_hash = new unsigned char[32];
    for(int i = 0; i < 32; i++)
    {
        zero_hash[i] = 0;
    }

    Block *genesis_block = new Block(zero_hash);
    current_block = genesis_block;

    Transaction *genesis_tx = new Transaction(0, miner_wallet, 25, 0);
    receive_tx(genesis_tx);

    confirm_internal_wallets(genesis_block);
    gen_next_block();
}

void Blockchain::add_wallet(unsigned int ID)
{
    InternalWallet *wallet = new InternalWallet();

    auto *wallet_pair = new std::pair<unsigned int, InternalWallet *>;
    wallet_pair->first = ID;
    wallet_pair->second = wallet;
    wallets->insert(*wallet_pair);
}

int Blockchain::receive_tx(Transaction *tx)
{
    if(wallets->find(tx->get_from()) == wallets->end())
    {
        add_wallet(tx->get_from());
    }

    if(wallets->find(tx->get_to()) == wallets->end())
    {
        add_wallet(tx->get_to());
    }

    if(tx->get_from() == 0)
    {
        wallets->at(tx->get_to())->change_unconfirmed_balance(tx->get_amount());
        wallets->at(miner_wallet)->change_unconfirmed_balance(tx->get_fee());

        current_block->add_tx(tx);

        return 0;
    }
    else
    {
        if(tx->get_amount() + tx->get_fee() < wallets->at(tx->get_from())->get_uncomfirmed_balance())
        {
            wallets->at(tx->get_to())->change_unconfirmed_balance(tx->get_amount());
            wallets->at(miner_wallet)->change_unconfirmed_balance(tx->get_fee());

            wallets->at(tx->get_from())->change_unconfirmed_balance(-1 * tx->get_amount());
            wallets->at(tx->get_from())->change_unconfirmed_balance(-1 * tx->get_fee());

            current_block->add_tx(tx);
            return 0;
        }
        else
        {
            return -1;
        }
    }
}

void Blockchain::miner_start(unsigned int difficulty)
{
    bool mining = true;
    auto *rand = new std::linear_congruential_engine<std::uint_fast32_t, 48271, 0, 2147483647>();

    while(mining)
    {
        unsigned int nonce = (unsigned) rand->operator()();

        current_block->set_nonce(nonce);
        current_block->update_hash();

        unsigned char *hash = current_block->get_hash();
    }
}

void Blockchain::confirm_internal_wallets(Block *block)
{
    std::vector<Transaction *> *tx_pointers = block->get_tx_pointers();
    Transaction *current_tx = nullptr;

    for(unsigned int i = 0; i < block->get_size(); i++)
    {
        current_tx = tx_pointers->at(i);

        unsigned int to = current_tx->get_to();
        unsigned int from = current_tx->get_from();
        double amount = current_tx->get_amount();
        double fee = current_tx->get_fee();

        wallets->at(to)->change_balance(amount);
        wallets->at(miner_wallet)->change_balance(fee);

        if(from != 0)
        {
            wallets->at(from)->change_balance(-amount);
            wallets->at(from)->change_balance(-fee);
        }
    }
}
