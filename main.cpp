#include <iostream>
#include "src/bc/Blockchain.h"

using namespace std;

int main()
{
    auto *chain = new Blockchain(1);

    for(int i = 2; i <= 100; i++)
    {
        Transaction *tx = new Transaction(1, (unsigned) i, .1, .1);
        chain->receive_tx(tx);
    }

    chain->miner_start(6, 3);
    miner_data_t *chain_data = chain->get_miner_data();

    while(chain_data->run);
    for(volatile int i = 0; i < 1000000; i++);

    /*
    string input;
    while (input != "exit")
    {
        cout << "blocksim> ";
        cin >> input;

        if (input == "help")
        {
            cout << "blocksim supported commands:\n";
            cout << "\tgen [num_wallets] [miner_wallet]: creates a new blockchain\n";
            cout << "\tsave [path]: saves the current blockchain\n";
            cout << "\tload [path]: loads the specified blockchain\n";
            cout << "\tset [attribute] [value]: changes attributes of current blockchain. See 'set help'\n";
            cout << "\n";
            cout
                    << "\ttx [from_wallet] [to_wallet] [amount] [fee]: creates the specified transaction on the blockchain, if valid\n";
            cout << "\n";
            cout << "\tminer [option]: change the blockchain miner state. See 'miner help'\n";
        }
    }
     */
}