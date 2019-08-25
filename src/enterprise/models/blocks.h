#ifndef ENTERPRISE_BLOCKS_H
#define ENTERPRISE_BLOCKS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

/**
 *
 * Represents a CBlockIndex object
 *
 **/

class eBlocks
{
public:
    eBlocks() {}
    eBlocks(
        const std::string hash,
        const std::string merkle_root,
        const int64_t time,
        const int64_t median_time,
        const int64_t height,
        const int64_t subsidy,
        const unsigned int transactions_count,
        const uint32_t version,
        const uint32_t status,
        const uint32_t bits,
        const uint64_t nonce,
        const double difficulty,
        const std::string chain_work)
        : hash(hash),
          merkle_root(merkle_root),
          time(time),
          median_time(median_time),
          height(height),
          subsidy(subsidy),
          transactions_count(transactions_count),
          version(version),
          status(status),
          bits(bits),
          nonce(nonce),
          difficulty(difficulty),
          chain_work(chain_work)
    {
    }

#pragma db id auto
    unsigned int id;

    std::string hash;
    std::string merkle_root;
    int64_t time;
    int64_t median_time;
    int64_t height;
    int64_t subsidy;
    unsigned int transactions_count;
    uint32_t version;
    uint32_t status;
    uint32_t bits;
    uint64_t nonce;
    double difficulty;
    std::string chain_work;
    std::string fee_data;

    int64_t segwit_spend_count = 0;
    int64_t outputs_count = 0;
    int64_t inputs_count = 0;
    int64_t total_output_value = 0;
    int64_t total_fees = 0;
    int64_t total_size = 0;
    int64_t total_vsize = 0;
    int64_t total_weight = 0;


private:
    friend class odb::access;
};

#endif //ENTERPRISE_BLOCKS_H
