#include <spice/HD_Wallet.hpp>

HD_Wallet::HD_Wallet()
{
    entropy = bc::data_chunk(16);
    bc::pseudo_random_fill(entropy);
    mnemonic = bc::wallet::create_mnemonic(entropy);
    seed = bc::to_chunk(bc::wallet::decode_mnemonic(mnemonic));
    //testnet
    privateKey = bc::wallet::hd_private(seed, bc::wallet::hd_private::testnet);
    publicKey = privateKey.to_public();
}

HD_Wallet::HD_Wallet(const bc::data_chunk Userentropy)
{
    entropy = Userentropy;
    mnemonic = bc::wallet::create_mnemonic(entropy);
    seed = bc::to_chunk(bc::wallet::decode_mnemonic(mnemonic));
    //testnet
    privateKey = bc::wallet::hd_private(seed, bc::wallet::hd_private::testnet);
    publicKey = privateKey.to_public();
}
HD_Wallet::HD_Wallet(const bc::wallet::word_list mnemonicSeed)
{
    seed = bc::to_chunk(bc::wallet::decode_mnemonic(mnemonicSeed));
    mnemonic = mnemonicSeed;
    //testnet
    privateKey = bc::wallet::hd_private(seed, bc::wallet::hd_private::testnet);
    publicKey = privateKey.to_public();
}
bc::wallet::hd_private HD_Wallet::childPrivateKey(int index)
{
    return privateKey.derive_private(index);
}
bc::wallet::hd_public HD_Wallet::childPublicKey(int index)
{
    return publicKey.derive_public(index);
}
bc::wallet::payment_address HD_Wallet::childAddress(int index)
{
    //mainnet
    //return wallet::ec_public(childPublicKey(index).point()).to_payment_address();
    //testnet
    return bc::wallet::payment_address(bc::wallet::ec_public(childPublicKey(index).point()), 0x6f);
}

void HD_Wallet::displayPrivateKey()
{
    std::cout << "\nPrivate Key:" << privateKey.encoded() << std::endl;
}
void HD_Wallet::displayChildPrivateKey(int index)
{
    std::cout << "\nChild Key: " << childPrivateKey(index).encoded() << std::endl;
}
std::string HD_Wallet::displayAddress(int index)
{
    return childAddress(index).encoded();
}
void HD_Wallet::addressRange(int start, int end)
{
    while(start != end)
    {
        displayAddress(start++);
    }
}
std::string HD_Wallet::displayMnemonic()
{
    if(bc::wallet::validate_mnemonic(mnemonic)){
        std::string mnemonicString = bc::join(mnemonic);
        return mnemonicString;
    } else {
        return "not valid!";
    }
}
void HD_Wallet::dumpKeys()
{
    displayMnemonic();
    displayPrivateKey();
    displayChildPrivateKey(1);
    displayAddress(1);
}
HD_Wallet randomGen()
{
    bc::data_chunk entropyChunk = bc::data_chunk(16);
    bc::pseudo_random_fill(entropyChunk);
    return HD_Wallet(entropyChunk);
}
