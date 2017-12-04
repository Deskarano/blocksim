#include "Blockchain.h"
#include "../crypto/SHA256.h"

#include <cstddef>
#include <iostream>
#include <random>
#include <cstring>

Blockchain::Blockchain(unsigned int miner_wallet)
{
    this->block_pointers = new std::vector<Block *>;
    this->wallet_pointers = new std::unordered_map<unsigned int, InternalWallet *>;
    this->miner_wallet = miner_wallet;
    this->miner_data = nullptr;

    auto *zero_hash = new unsigned char[32];
    for(int i = 0; i < 32; i++)
    {
        zero_hash[i] = 0;
    }

    auto *genesis_block = new Block(zero_hash);

    current_block = genesis_block;
    block_pointers->push_back(current_block);

    auto *genesis_tx = new Transaction(miner_wallet, miner_wallet, 25, 0);
    receive_tx(genesis_tx);
}

void Blockchain::add_wallet(unsigned int ID)
{
    auto wallet = new InternalWallet();

    auto wallet_pair = new std::pair<unsigned int, InternalWallet *>;
    wallet_pair->first = ID;
    wallet_pair->second = wallet;
    wallet_pointers->insert(*wallet_pair);
}

void Blockchain::receive_tx(Transaction *tx)
{
    if(wallet_pointers->find(tx->get_from()) == wallet_pointers->end())
    {
        add_wallet(tx->get_from());
    }

    if(wallet_pointers->find(tx->get_to()) == wallet_pointers->end())
    {
        add_wallet(tx->get_to());
    }

    if(current_block->get_tx_pointers()->empty())
    {
        wallet_pointers->at(tx->get_to())->change_unconfirmed_balance(tx->get_amount());
        wallet_pointers->at(miner_wallet)->change_unconfirmed_balance(tx->get_fee());

        current_block->add_tx(tx);
    }
    else
    {
        if(tx->get_amount() + tx->get_fee() < wallet_pointers->at(tx->get_from())->get_uncomfirmed_balance())
        {
            wallet_pointers->at(tx->get_to())->change_unconfirmed_balance(tx->get_amount());
            wallet_pointers->at(miner_wallet)->change_unconfirmed_balance(tx->get_fee());

            wallet_pointers->at(tx->get_from())->change_unconfirmed_balance(-1 * tx->get_amount());
            wallet_pointers->at(tx->get_from())->change_unconfirmed_balance(-1 * tx->get_fee());

            current_block->add_tx(tx);
        }
        else
        {
            std::cout << "--blockchain_error\tInsufficient confirmed funds in 'from' wallet. Deleting transaction\n";
            delete tx;
        }
    }
}

unsigned int get_max_difficulty(const unsigned char *hash)
{
    for(unsigned int i = 0; i < 256; i++)
    {
        if((hash[i / 8] >> (7 - (i % 8)) & 0b00000001) == 1)
        {
            return i;
        }
    }

    return 256;
}

void *miner_thread(void *data)
{
    Block *block = ((miner_data_t *) data)->block;
    time_t block_timestamp = block->get_timestamp();

    unsigned int difficulty = ((miner_data_t *) data)->difficulty;

    unsigned char *last_tx_hash = block->get_tx_pointers()->at(0)->get_hash();

    auto *hash_data = new unsigned char[64 + sizeof(unsigned int) + sizeof(time_t)];
    std::memcpy(hash_data, block->get_prev_hash(), 32);
    std::memcpy(hash_data + 32, last_tx_hash, 32);
    //no nonce-ents
    std::memcpy(hash_data + 64 + sizeof(unsigned int), &block_timestamp, sizeof(time_t));

    std::random_device *rand = new std::random_device;

    auto *test_hash = new unsigned char[32];
    unsigned int nonce;

    while(((miner_data_t *) data)->running)
    {
        if(((miner_data_t *) data)->time_ended != time(nullptr))
        {
            ((miner_data_t *) data)->time_ended = time(nullptr);
        }

        if(last_tx_hash != block->get_tx_pointers()->at(0)->get_hash())
        {
            std::cout << "--blockchain_miner\tDetected new transaction in block. Resetting miner_data\n";
            ((miner_data_t *) data)->time_started = time(nullptr);
            ((miner_data_t *) data)->num_hashes = 0;
            ((miner_data_t *) data)->best_difficulty = 0;

            last_tx_hash = block->get_tx_pointers()->at(0)->get_hash();
            std::memcpy(hash_data + 32, last_tx_hash, 32);
        }

        ++((miner_data_t *) data)->num_hashes;

        nonce = (unsigned) rand->operator()();
        std::memcpy(hash_data + 64, &nonce, sizeof(unsigned int));

        delete test_hash;
        test_hash = SHA256(hash_data, 64 + sizeof(unsigned int) + sizeof(time_t));

        unsigned int test_difficulty = get_max_difficulty(test_hash);

        if(test_difficulty > ((miner_data_t *) data)->best_difficulty)
        {
            ((miner_data_t *) data)->best_difficulty = test_difficulty;
        }

        if(test_difficulty > difficulty)
        {
            std::cout << "\n\n--blockchain_miner\tFound nonce for block " << block << "\n";
            std::cout << "--blockchain_miner\tRun 'confirm' to confirm block\n";

            ((miner_data_t *) data)->time_ended = time(nullptr);
            ((miner_data_t *) data)->result_found = true;
            ((miner_data_t *) data)->result = nonce;
        }
    }

    delete rand;
    return nullptr;
}

void *miner_thread_controller(void *data)
{
    auto miner_data = (miner_data_t *) data;

    for(unsigned int i = 0; i < miner_data->num_threads; i++)
    {
        auto thread = new pthread_t;
        pthread_create(thread, nullptr, miner_thread, miner_data);
        delete thread;
    }

    //idle while running
    while(miner_data->running && !miner_data->result_found);

    if(miner_data->result_found)
    {
        miner_data->block->set_nonce(miner_data->result);
        miner_data->block->update_hash();

        miner_data->running = false;
        std::cout << "\nblocksim> ";
        std::cout.flush();
    }

    return nullptr;
}

void Blockchain::miner_start(unsigned int difficulty, unsigned int num_threads)
{
    if(miner_data == nullptr)
    {
        miner_data = new miner_data_t;
    }

    //reset miner_data
    miner_data->time_started = time(nullptr);
    miner_data->running = true;
    miner_data->result_found = false;

    miner_data->block = current_block;
    miner_data->difficulty = difficulty;
    miner_data->num_threads = num_threads;

    miner_data->num_hashes = 0;
    miner_data->best_difficulty = 0;
    miner_data->result = 0;

    auto handle = new pthread_t;
    pthread_create(handle, nullptr, miner_thread_controller, miner_data);

    std::cout << "--blockchain_miner\tStarted mining on " << num_threads << " threads\n";
}

void Blockchain::miner_stop()
{
    miner_data->running = false;

    std::cout << "--blockchain_miner\tKilled mining threads\n";
}

void Blockchain::confirm_next_block(unsigned int difficulty)
{
    if(get_max_difficulty(current_block->get_block_hash()) > difficulty)
    {
        confirm_internal_wallets();
        gen_next_block();
    }
    else
    {
        std::cout << "--blockchain_error\tHash does not satisfy difficulty requirement. Block not confirmed\n";
    }
}

void Blockchain::confirm_internal_wallets()
{
    std::vector<Transaction *> *tx_pointers = current_block->get_tx_pointers();
    Transaction *current_tx = tx_pointers->at(0);

    unsigned int to = current_tx->get_to();
    unsigned int from;
    double amount = current_tx->get_amount();
    double fee = current_tx->get_fee();

    wallet_pointers->at(to)->change_balance(amount);
    wallet_pointers->at(miner_wallet)->change_balance(fee);

    for(unsigned int i = 1; i < current_block->get_size(); i++)
    {
        current_tx = tx_pointers->at(i);

        to = current_tx->get_to();
        from = current_tx->get_from();
        amount = current_tx->get_amount();
        fee = current_tx->get_fee();

        wallet_pointers->at(to)->change_balance(amount);
        wallet_pointers->at(miner_wallet)->change_balance(fee);

        wallet_pointers->at(from)->change_balance(-amount);
        wallet_pointers->at(from)->change_balance(-fee);
    }

    for(auto &wallet_pointer : *wallet_pointers)
    {
        if(wallet_pointer.second->get_balance() != wallet_pointer.second->get_uncomfirmed_balance())
        {
            std::cout << "--blockchain_error\tInvalid transactions in block " << current_block << "\n";
        }
    }
}

void Blockchain::gen_next_block()
{
    current_block = new Block(current_block->get_block_hash());
    auto *reward = new Transaction(miner_wallet, miner_wallet, 25, 0);

    block_pointers->push_back(current_block);

    receive_tx(reward);
}