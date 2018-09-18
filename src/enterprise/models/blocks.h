#ifndef ENTERPRISE_BLOCKS_H
#define ENTERPRISE_BLOCKS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"

/**
 *
 * Represents a CBlockIndex object
 *
 **/

#pragma db object
class eBlocks {
public:
    eBlocks() {}
    eBlocks(
            const std::string hash,
            const std::string merkle_root,
            const int64_t time,
            const int64_t height,
            const unsigned int transactions_count,
            const uint32_t version,
            const uint32_t status,
            const uint32_t bits,
            const uint64_t nonce
    )
            :
            hash(hash),
            merkle_root(merkle_root),
            time(time),
            height(height),
            transactions_count(transactions_count),
            version(version),
            status(status),
            bits(bits),
            nonce(nonce) {
    }

    #pragma db id auto
    unsigned int id;

    std::string hash;
    std::string merkle_root;
    int64_t time;
    int64_t height;
    unsigned int transactions_count;
    uint32_t version;
    uint32_t status;
    uint32_t bits;
    uint64_t nonce;

    int64_t segwit_spend_count = 0;
    int64_t outputs_count = 0;
    int64_t inputs_count = 0;
    int64_t total_output_value = 0;
    int64_t total_fees = 0;
    int64_t total_size = 0;
    int64_t total_vsize = 0;
    int64_t total_weight = 0;
    std::string fee_data;

private:
    friend class odb::access;

};

#pragma db view object(eBlocks)
struct block_hash
{
  std::string hash;
};

#pragma db view
struct missing_blocks
{
  int height;
};

#endif //ENTERPRISE_BLOCKS_H
