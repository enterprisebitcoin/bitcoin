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
    void UpsertAddressBook(const std::map<CTxDestination, CAddressBookData>& address_book);
//    unsigned int UpsertBlock(const uint256 &binary_hash);
//    unsigned int GetBlockID(uint256& tx_hash);
//    void UpsertOutputs(const std::vector<CTxOut> vout, const unsigned int output_etransaction_id);
//    void UpsertInputs(const std::vector<CTxIn> vin, const unsigned int input_etransaction_id);
//    unsigned int UpsertTransaction(const CTransactionRef tx, const bool upsert_inputs);
    void UpsertWalletTransactions(const std::map<uint256, CWalletTx> wallet_transactions);
    void UpsertWalletTransaction(const CWalletTx& wallet_transaction);
//    void DeleteTx(uint256 &hash);
}

#endif //ENTERPRISE_WALLET_H
