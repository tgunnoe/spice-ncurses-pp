#ifndef HDWALLET_HPP
#define HDWALLET_HPP

#include <bitcoin/bitcoin.hpp>
#include <string.h>
#include <iostream>
#include <string>


class HD_Wallet
{
    bc::data_chunk entropy;
    bc::data_chunk seed;
    bc::wallet::word_list mnemonic;
    bc::wallet::hd_private privateKey;
    bc::wallet::hd_public publicKey;
  public:
    HD_Wallet();

    HD_Wallet(const bc::data_chunk Userentropy);

    HD_Wallet(const bc::wallet::word_list mnemonicSeed);

    bc::wallet::hd_private childPrivateKey(int index);

    bc::wallet::hd_public childPublicKey(int index);

    bc::wallet::payment_address childAddress(int index);


    void displayPrivateKey();

    void displayChildPrivateKey(int index);

    std::string displayAddress(int index);

    void addressRange(int start, int end);

    std::string displayMnemonic();

    void dumpKeys();

};

HD_Wallet randomGen();

#endif
