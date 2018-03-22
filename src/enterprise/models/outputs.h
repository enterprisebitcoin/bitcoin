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
            std::string output_block_hash,
            unsigned int output_transaction_index,
            std::string output_transaction_hash,
            unsigned int output_vector,
            int64_t value,
            std::string locking_script_id,
            int required_signatures
    ) : output_block_hash(output_block_hash),
        output_transaction_index(output_transaction_index),
        output_transaction_hash(output_transaction_hash),
        output_vector(output_vector),
        value(value),
        locking_script_id(locking_script_id),
        required_signatures(required_signatures) {};

#pragma db id auto
    unsigned int id;

    /**
     *  CTxOut
     **/
    std::string output_block_hash;
    unsigned int output_transaction_index;
    std::string output_transaction_hash;
    unsigned int output_vector;
    int64_t value;
    std::string locking_script_id;
    int required_signatures;


private:
    friend class odb::access;

    eOutputs() {}

};

#endif //ENTERPRISE_OUTPUTS_H
