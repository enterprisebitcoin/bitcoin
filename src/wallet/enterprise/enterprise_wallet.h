#include <boost/uuid/uuid.hpp>

#include <wallet/wallet.h>

#ifndef ENTERPRISE_WALLET_H
#define ENTERPRISE_WALLET_H

namespace enterprise_wallet
{
    boost::uuids::uuid GetWalletID();
    void ImportWatchOnlyAddresses();
    void UpsertWallet();
    void TopUpAddressPool();
    void UpsertAddress(const std::string &p2pkh_address,
                       const std::string &sw_bech32_address,
                       const std::string &sw_p2sh_address,
                       const std::string &name,
                       const std::string &purpose);
    void UpsertTx(const CWalletTx &wtx);
    void DeleteTx(uint256 &hash);
}

#endif //ENTERPRISE_WALLET_H
