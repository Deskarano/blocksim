#ifndef BLOCKSIM_INTERNALWALLET_H
#define BLOCKSIM_INTERNALWALLET_H

class InternalWallet
{
public:
    InternalWallet()
    {
        balance = 0;
        unconfirmed_balance = 0;
    }

    void change_balance(double d_balance)
    { this->balance += d_balance; }

    double get_balance()
    { return balance; }

    void change_unconfirmed_balance(double d_uncomfirmed_balance)
    { this->unconfirmed_balance += d_uncomfirmed_balance; }

    double get_uncomfirmed_balance()
    { return unconfirmed_balance; }

    void confirm(unsigned char *block_hash)
    {
        balance = unconfirmed_balance;
        last_confirmed_block;
    }

private:
    double balance;
    double unconfirmed_balance;

    unsigned char *last_confirmed_block;
};

#endif //BLOCKSIM_INTERNALWALLET_H
