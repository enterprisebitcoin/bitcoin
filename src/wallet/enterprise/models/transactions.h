#ifndef ENTERPRISE_TRANSACTIONS_H
#define ENTERPRISE_TRANSACTIONS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include <boost/uuid/uuid.hpp>

#include "../version.h"

#pragma db object

class eTransactions {
public:
    eTransactions(
            const int block_index,
            const bool is_trusted,
            const unsigned int size,
            const int64_t time,
            const int64_t time_received,
            const std::string &txid,
            const boost::uuids::uuid &wallet_id
    )
            :
            block_index(block_index),
            is_trusted(is_trusted),
            size(size),
            time(time),
            time_received(time_received),
            txid(txid),
            wallet_id(wallet_id) {
    }

#pragma db id auto
    unsigned int id;

    int block_index;
    bool is_trusted;
    unsigned int size;
    int64_t time;
    int64_t time_received;

#pragma db unique
    std::string txid;

#pragma db type("UUID")
    boost::uuids::uuid wallet_id;

private:
    friend class odb::access;

    eTransactions() {}

};

#endif //ENTERPRISE_TRANSACTIONS_H
