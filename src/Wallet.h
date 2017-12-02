#ifndef BLOCKSIM_WALLET_H
#define BLOCKSIM_WALLET_H

class Wallet
{
public:
    Wallet(unsigned char ID);

    unsigned char get_ID();
    double get_balance();

private:
    unsigned char ID;
    double balance;

    unsigned char *last_block_applied;
};

#endif //BLOCKSIM_WALLET_H