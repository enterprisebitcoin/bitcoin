#ifndef ENTERPRISE_TRANSACTIONS_H
#define ENTERPRISE_TRANSACTIONS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include <boost/uuid/uuid.hpp>

#include "../version.h"

#pragma db object

/**
 *
 * Based on the CTransaction class
 *
 **/
class eTransactions {
public:
    eTransactions(
            const unsigned int block_id,
            const unsigned int total_size,
            const unsigned int inputs_count,
            const unsigned int outputs_count,
            const int64_t value_out,
            const int32_t n_lock_time,
            const int32_t n_version,
            const std::string hash,
            const std::string witness_hash,
            const bool is_coinbase,
            const bool has_witness,
            const boost::uuids::uuid wallet_id
    )
            :
            block_id(block_id),
            total_size(total_size),
            inputs_count(inputs_count),
            outputs_count(outputs_count),
            value_out(value_out),
            n_lock_time(n_lock_time),
            n_version(n_version),
            hash(hash),
            witness_hash(witness_hash),
            is_coinbase(is_coinbase),
            has_witness(has_witness),
            wallet_id(wallet_id) {
    }

#pragma db id auto
    unsigned int id;

    const unsigned int block_id;
    const unsigned int total_size;
    const unsigned int inputs_count;
    const unsigned int outputs_count;
    const int64_t value_out;
    const int32_t n_lock_time;
    const int32_t n_version;
    const std::string hash;
    const std::string witness_hash;
    const bool is_coinbase;
    const bool has_witness;

#pragma db type("UUID") not_null
    boost::uuids::uuid wallet_id;

private:
    friend class odb::access;

    eTransactions() {}

};

#endif //ENTERPRISE_TRANSACTIONS_H
