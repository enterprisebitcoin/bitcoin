#ifndef ENTERPRISE_TRANSACTIONS_H
#define ENTERPRISE_TRANSACTIONS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

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
            const std::string block_hash,
            const unsigned int index,
            const unsigned int size,
            const unsigned int vsize,
            const unsigned int weight,
            const unsigned int inputs_count,
            const unsigned int outputs_count,
            const int64_t value_out,
            const int32_t lock_time,
            const int32_t version,
            const std::string hash,
            const std::string witness_hash,
            const bool is_coinbase,
            const bool has_witness
    )
            :
            block_hash(block_hash),
            index(index),
            size(size),
            vsize(vsize),
            weight(weight),
            inputs_count(inputs_count),
            outputs_count(outputs_count),
            value_out(value_out),
            lock_time(lock_time),
            version(version),
            hash(hash),
            witness_hash(witness_hash),
            is_coinbase(is_coinbase),
            has_witness(has_witness) {
    }

#pragma db id auto
    unsigned int id;

    std::string block_hash;
    unsigned int index;
    unsigned int size;
    unsigned int vsize;
    unsigned int weight;
    unsigned int inputs_count;
    unsigned int outputs_count;
    int64_t value_out;
    int32_t lock_time;
    int32_t version;
    std::string hash;
    std::string witness_hash;
    bool is_coinbase;
    bool has_witness;

private:
    friend class odb::access;

    eTransactions() {}

};

#endif //ENTERPRISE_TRANSACTIONS_H
