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

    void change_unconfirmed_balance(double d_unconfirmed_balance)
    { this->unconfirmed_balance += d_unconfirmed_balance; }

    double get_unconfirmed_balance()
    { return unconfirmed_balance; }

private:
    double balance;
    double unconfirmed_balance;
};

#endif //BLOCKSIM_INTERNALWALLET_H