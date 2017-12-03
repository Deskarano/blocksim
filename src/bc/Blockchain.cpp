#include "Blockchain.h"
#include "../crypto/SHA256.h"

#include <cstddef>
#include <iostream>
#include <random>
#include <cstring>

Blockchain::Blockchain(unsigned int miner_wallet)
{
    this->blocks = new std::vector<Block *>;
    this->wallets = new std::unordered_map<unsigned int, InternalWallet *>;
    this->miner_wallet = miner_wallet;

    auto *zero_hash = new unsigned char[32];
    for(int i = 0; i < 32; i++)
    {
        zero_hash[i] = 0;
    }

    auto *genesis_block = new Block(zero_hash);
    delete zero_hash;

    current_block = genesis_block;

    auto *genesis_tx = new Transaction(0, miner_wallet, 25, 0);
    receive_tx(genesis_tx);

    confirm_internal_wallets();
    gen_next_block();
}

void Blockchain::add_wallet(unsigned int ID)
{
    auto *wallet = new InternalWallet();

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

bool test_hash_for_difficulty(unsigned char *hash, unsigned int difficulty)
{
    for(int i = 0; i < difficulty; i++)
    {
        if(i % 2 == 0)
        {
            //top nibble
            if(((hash[i / 2] >> 4) & 0xF) != 0)
            {
                return false;
            }
        }
        else
        {
            //bottom nibble
            if((hash[i / 2] & 0xF) != 0)
            {
                return false;
            }
        }
    }

    return true;
}

void *miner_thread(void *data)
{
    unsigned char *prev_hash = ((miner_data_t *) data)->block->get_prev_hash();
    unsigned char *tx_hash = ((miner_data_t *) data)->block->get_tx_pointers()->at(0)->get_hash();

    unsigned int difficulty = ((miner_data_t *) data)->difficulty;

    auto *hash_data = new unsigned char[64 + sizeof(unsigned int)];
    std::memcpy(hash_data, prev_hash, 32);
    std::memcpy(hash_data + 32, tx_hash, 32);

    auto *test_hash = new unsigned char[32];
    unsigned int nonce;

    std::random_device *rand = new std::random_device;

    while(((miner_data_t *) data)->run)
    {
        ++((miner_data_t *) data)->num_hashes_tried;

        nonce = (unsigned) rand->operator()();
        std::memcpy(hash_data + 64, &nonce, sizeof(unsigned int));

        delete test_hash;
        test_hash = SHA256(hash_data, 64 + sizeof(unsigned int));

        if(test_hash_for_difficulty(test_hash, difficulty))
        {
            printf("success!\n");

            ((miner_data_t *) data)->run = false;
            ((miner_data_t *) data)->result = nonce;
        }
    }

    delete rand;
    return nullptr;
}

void *miner_thread_controller(void *data)
{
    miner_data_t *miner_data = (miner_data_t *) data;

    for(int i = 0; i < miner_data->num_threads; i++)
    {
        auto thread = new pthread_t;
        pthread_create(thread, nullptr, miner_thread, miner_data);
        delete thread;
    }

    //idle while running
    while(miner_data->run);

    if(miner_data->result != 0)
    {
        miner_data->block->set_nonce(miner_data->result);
        miner_data->block->update_hash();
    }
}

void Blockchain::miner_start(unsigned int difficulty, unsigned int num_threads)
{
    delete miner_data;

    miner_data = new miner_data_t;
    miner_data->run = true;
    miner_data->block = current_block;
    miner_data->difficulty = difficulty;
    miner_data->num_threads = num_threads;
    miner_data->num_hashes_tried = 0;
    miner_data->result = 0;

    auto handle = new pthread_t;
    pthread_create(handle, nullptr, miner_thread_controller, nullptr);
}

void Blockchain::miner_stop()
{
    miner_data->run = false;
}

void Blockchain::confirm_next_block(unsigned int difficulty)
{
    if(test_hash_for_difficulty(current_block->get_hash(), difficulty))
    {
        confirm_internal_wallets();
        gen_next_block();
    }
}

void Blockchain::gen_next_block()
{
    current_block = new Block(current_block->get_hash());
    auto *reward = new Transaction(0, miner_wallet, 25, 0);

    receive_tx(reward);
}


void Blockchain::confirm_internal_wallets()
{
    std::vector<Transaction *> *tx_pointers = current_block->get_tx_pointers();
    Transaction *current_tx = nullptr;

    for(unsigned int i = 0; i < current_block->get_size(); i++)
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
