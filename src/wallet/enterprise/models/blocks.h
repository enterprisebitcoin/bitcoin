#ifndef ENTERPRISE_BLOCKS_H
#define ENTERPRISE_BLOCKS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include <boost/uuid/uuid.hpp>

#include "../version.h"

#pragma db object

class eBlocks {
public:
    eBlocks(
            const std::string &hash,
            const int64_t time,
            const int64_t height
    )
            :
            hash(hash),
            time(time),
            height(height) {
    }

#pragma db id auto
    unsigned int id;

#pragma db unique
    std::string hash;

    int64_t time;
    int64_t height;

private:
    friend class odb::access;

    eBlocks() {}

};

#endif //ENTERPRISE_BLOCKS_H
