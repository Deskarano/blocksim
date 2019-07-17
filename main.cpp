#include <iostream>

#include "src/blockchain/Blockchain.h"
#include "src/crypto/SHA256.h"

int main()
{
    Blockchain *main_chain = nullptr;

    std::string input;
    while(true)
    {
        std::cout << "blocksim> ";
        std::cin >> input;

        if(input == "exit")
        {
            return 0;
        }
        else if(input == "help")
        {
            std::cout << "\nblocksim commands:\n";
            std::cout << "\tBlockchain:\n";
            std::cout << "\t\tnew_chain [miner_wallet]: creates a new blockchain, with all mined coins and fees deposited to wallet [miner_wallet]\n";
            std::cout << "\t\tnew_block [difficulty]: confirms the current block, adding it to the chain\n";
            std::cout << "\t\tnew_tx [from] [to] [amount] [fee]: creates the specified transaction on the blockchain, if valid\n";
            std::cout << "\t\tnew_wallet [ID]: creates a new wallet with ID [ID]\n";
            std::cout << "\tInformation:\n";
            std::cout << "\t\tblocks: lists all blocks in the current blockchain\n";
            std::cout << "\t\ttxs [block]: lists all transactions in the specified block\n";
            std::cout << "\t\twallets: lists all wallets in the current blockchain\n";
            std::cout << "\tMining:\n";
            std::cout << "\t\tminer_start [difficulty] [num_threads]: starts the blockchain miner\n";
            std::cout << "\t\tminer stop: stops the blockchain miner\n";
            std::cout << "\t\tminer_status: shows the current status of the blockchain miner";
            std::cout << "\n";
        }
        else if(input == "new_chain")
        {
            unsigned int miner_wallet;

            std::cout << "\t[miner_wallet] = ";
            std::cin >> miner_wallet;

            std::cout << "\n";
            main_chain = new Blockchain(miner_wallet);
        }
        else if(input == "new_block")
        {
            unsigned int difficulty;

            std::cout << "\t[difficulty] = ";
            std::cin >> difficulty;

            std::cout << "\n";

            main_chain->confirm_next_block(difficulty);
        }
        else if(input == "new_tx")
        {
            unsigned int from;
            unsigned int to;
            double amount;
            double fee;

            std::cout << "\t[from] = ";
            std::cin >> from;

            std::cout << "\t[to] = ";
            std::cin >> to;

            std::cout << "\t[amount] = ";
            std::cin >> amount;

            std::cout << "\t[fee] = ";
            std::cin >> fee;

            std::cout << "\n";

            auto tx = new Transaction(from, to, amount, fee);
            main_chain->receive_tx(tx);
        }
        else if(input == "new_wallet")
        {
            unsigned int ID;

            std::cout << "ID: ";
            std::cin >> ID;

            main_chain->add_wallet(ID);
        }
        else if(input == "blocks")
        {
            std::cout << "\n";

            std::vector<Block *> *block_pointers = main_chain->get_block_pointers();
            for(unsigned int i = 0; i < block_pointers->size(); i++)
            {
                std::cout << "Block " << i << " (" << block_pointers->at(i) << "):\n";

                std::cout << "\tHashes:\n";
                std::cout << "\t\tprev_hash: ";
                print_hash(block_pointers->at(i)->get_prev_hash());
                std::cout << "\t\ttx_hash: ";
                print_hash(block_pointers->at(i)->get_tx_pointers()->at(0)->get_hash());
                std::cout << "\t\tblock_hash: ";
                print_hash(block_pointers->at(i)->get_block_hash());

                std::cout << "\tInfo:\n";
                std::cout << "\t\ttime: " << block_pointers->at(i)->get_timestamp() << "\n";
                std::cout << "\t\tnonce: " << block_pointers->at(i)->get_nonce() << "\n";
                std::cout << "\t\tsize: " << block_pointers->at(i)->get_size() << "\n";
            }
        }
        else if(input == "txs")
        {
            unsigned int block;

            std::cout << "\t[block] = ";
            std::cin >> block;

            std::cout << "\n";

            std::vector<Transaction *> *tx_pointers = main_chain->get_block_pointers()->at(block)->get_tx_pointers();
            for(unsigned int i = 0; i < tx_pointers->size(); i++)
            {
                std::cout << "Transaction " << i << " (" << tx_pointers->at(i) << "):\n";
                std::cout << "\tDetails:\n";
                std::cout << "\t\tfrom: " << tx_pointers->at(i)->get_from() << "\n";
                std::cout << "\t\tto: " << tx_pointers->at(i)->get_to() << "\n";
                std::cout << "\t\tamount: " << tx_pointers->at(i)->get_amount() << "\n";
                std::cout << "\t\tfee: " << tx_pointers->at(i)->get_fee() << "\n";

                std::cout << "\tMetadata:\n";
                std::cout << "\t\ttime: " << tx_pointers->at(i)->get_timestamp() << "\n";
                std::cout << "\t\thash: ";
                print_hash(tx_pointers->at(i)->get_hash());
            }
        }
        else if(input == "wallets")
        {
            std::cout << "\n";
            std::cout << "Wallets in blockchain:\n";

            for(auto &wallet_pointer : *main_chain->get_wallet_pointers())
            {
                std::cout << "\tWallet " << wallet_pointer.first << ":\n";
                std::cout << "\t\tconfirmed: " << wallet_pointer.second->get_balance() << "\n";
                std::cout << "\t\tunconfirmed: " << wallet_pointer.second->get_unconfirmed_balance() << "\n";
            }
        }
        else if(input == "miner_start")
        {
            unsigned int difficulty;
            unsigned int num_threads;

            std::cout << "\t[difficulty] = ";
            std::cin >> difficulty;

            std::cout << "\t[num_threads] = ";
            std::cin >> num_threads;

            std::cout << "\n";
            main_chain->miner_start(difficulty, num_threads);
        }
        else if(input == "miner_stop")
        {
            std::cout << "\n";

            main_chain->miner_stop();
        }
        else if(input == "miner_status")
        {
            miner_data_t *miner_data = main_chain->get_miner_data();

            std::cout << "\n";
            std::cout << "miner_data:\n";

            std::cout << "\tStatus:\n";
            std::cout << "\t\trunning: " << miner_data->running << "\n";
            std::cout << "\t\tresult_found: " << miner_data->result_found << "\n";

            std::cout << "\tConfig:\n";
            std::cout << "\t\tblock: " << miner_data->block << "\n";
            std::cout << "\t\tdifficulty: " << miner_data->difficulty << "\n";
            std::cout << "\t\tnum_threads: " << miner_data->num_threads << "\n";

            std::cout << "\tAnalysis:\n";

            time_t elapsed = miner_data->time_ended - miner_data->time_started;
            std::cout << "\t\tnum_hashes: " << miner_data->num_hashes << "\n";
            std::cout << "\t\ttime_elapsed: " << elapsed << "s\n";

            double hash_per_sec = (double) miner_data->num_hashes / elapsed;
            std::cout << "\t\thash/sec: " << hash_per_sec << "\n";
            std::cout << "\t\thash/sec/thread: " << hash_per_sec / miner_data->num_threads << "\n";

            std::cout << "\tResults:\n";
            std::cout << "\t\tbest_difficulty: " << miner_data->best_difficulty << "\n";
            std::cout << "\t\tresult: " << miner_data->result << "\n";
        }
        else
        {
            std::cout << "\n--blockchain_error\tUnrecognized command: " << input << "\n";
        }

        std::cout << "\n";
    }
}
