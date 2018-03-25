#ifndef ENTERPRISE_OUTPUTS_H
#define ENTERPRISE_OUTPUTS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

/**
 *
 * A CTxOut
 *
 **/

class eOutputs {
public:
    eOutputs(
            std::string block_hash,
            unsigned int transaction_block_index,
            std::string transaction_hash,
            unsigned int vector,
            int64_t value,
            std::string locking_script_id,
            int required_signatures
    ) : block_hash(block_hash),
        transaction_block_index(transaction_block_index),
        transaction_hash(transaction_hash),
        vector(vector),
        value(value),
        locking_script_id(locking_script_id),
        required_signatures(required_signatures) {};

#pragma db id auto
    unsigned int id;

    /**
     *  CTxOut
     **/
    std::string block_hash;
    unsigned int transaction_block_index;
    std::string transaction_hash;
    unsigned int vector;
    int64_t value;
    std::string locking_script_id;
    int required_signatures;


private:
    friend class odb::access;

    eOutputs() {}

};

#endif //ENTERPRISE_OUTPUTS_H
