#include "Wallet.h"

Wallet::Wallet(unsigned char ID)
{
    this->ID = ID;
}

unsigned char Wallet::get_ID()
{
    return this->ID;
}

double Wallet::get_balance()
{
    return this->balance;
}