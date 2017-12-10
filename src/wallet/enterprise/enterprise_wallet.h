#include <wallet/wallet.h>

#ifndef ENTERPRISEBITCOIN_WALLET_H
#define ENTERPRISEBITCOIN_WALLET_H

namespace enterprise_wallet
{
    /** Insert or update a wallet transaction **/
    void UpsertTx(const CWalletTx wtx);
}

#endif //ENTERPRISEBITCOIN_WALLET_H
