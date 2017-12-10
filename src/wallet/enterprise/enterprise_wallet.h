#include <wallet/wallet.h>

#ifndef ENTERPRISEBITCOIN_WALLET_H
#define ENTERPRISEBITCOIN_WALLET_H

namespace enterprise_wallet
{
    void InsertAddress(const std::string &address);
    void UpdateAddress(const std::string &address, const std::string &name, const std::string &purpose);
    void UpsertTx(const CWalletTx wtx);
    void DeleteTx(uint256 hash);
}

#endif //ENTERPRISEBITCOIN_WALLET_H
