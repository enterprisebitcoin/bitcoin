#ifndef ENTERPRISE_BLOCKS_H
#define ENTERPRISE_BLOCKS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

class eBlocks {
public:
    eBlocks(
            const std::string hash,
            const std::string merkle_root,
            const int64_t time,
            const int64_t height,
            const unsigned int transactions_count,
            const uint32_t version,
            const uint32_t status,
            const uint32_t bits,
            const uint32_t nonce,


    )
            :
            hash(hash),
            time(time),
            height(height) {
    }

#pragma db id
    std::string hash;

    std::string merkle_root;
    int64_t time;
    int64_t height;

private:
    friend class odb::access;

    eBlocks() {}

};

#endif //ENTERPRISE_BLOCKS_H
