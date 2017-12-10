#include <wallet/wallet.h>

#ifndef ENTERPRISE_WALLET_H
#define ENTERPRISE_WALLET_H

namespace enterprise_wallet
{
    void TopUpAddressPool();
    void InsertAddress(const std::string &address);
    void UpdateAddress(const std::string &address, const std::string &name, const std::string &purpose);
    void UpsertTx(const CWalletTx wtx);
    void DeleteTx(uint256 hash);
}

#endif //ENTERPRISE_WALLET_H
