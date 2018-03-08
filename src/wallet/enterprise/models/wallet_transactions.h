#ifndef ENTERPRISE_WALLET_TRANSACTIONS_H
#define ENTERPRISE_WALLET_TRANSACTIONS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include <boost/uuid/uuid.hpp>

#include "../version.h"

#pragma db object

/**
 *
 * Based on the CWalletTx class
 *
 **/
class eWalletTransactions {
public:
    eWalletTransactions(
            const unsigned int etransaction_id,
            const bool is_trusted,
            const int64_t n_time_smart,
            const int64_t n_time_received,
            const int64_t debit,
            const int64_t credit,
            const boost::uuids::uuid wallet_id
    )
            :
            etransaction_id(etransaction_id),
            is_trusted(is_trusted),
            n_time_smart(n_time_smart),
            n_time_received(n_time_received),
            debit(debit),
            credit(credit),
            wallet_id(wallet_id) {
    }

#pragma db id auto
    unsigned int id;

    unsigned int etransaction_id;
    bool is_trusted;
    int64_t n_time_smart;
    int64_t n_time_received;
    int64_t debit;
    int64_t credit;

#pragma db type("UUID") not_null
    boost::uuids::uuid wallet_id;

private:
    friend class odb::access;

    eWalletTransactions() {}

};

#endif //ENTERPRISE_WALLET_TRANSACTIONS_H
