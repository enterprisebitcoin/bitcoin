#ifndef ENTERPRISE_OUTPUTS_H
#define ENTERPRISE_OUTPUTS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

/**
 *
 * A combination of a CTxOut and the CTxIn that spends it, if it exists
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

            std::string input_block_hash,
            unsigned int input_transaction_index,
            std::string input_transaction_hash,
            unsigned int input_vector,
            std::string unlocking_script_id,
            uint32_t sequence
    ) : output_block_hash(output_block_hash),
        output_transaction_index(output_transaction_index),
        output_vector(output_vector),
        value(value),
        locking_script_id(locking_script_id),

        input_block_hash(input_block_hash),
        input_transaction_index(input_transaction_index),
        input_vector(input_vector),
        unlocking_script_id(unlocking_script_id),
        sequence(sequence) {};

    #pragma db id auto
    unsigned int id;

    /**
     *  CTxOut
     **/
    #pragma db null
    std::string output_block_hash;
    #pragma db null
    unsigned int output_transaction_index;
    #pragma db null
    std::string output_transaction_hash;
    unsigned int output_vector;
    #pragma db null
    int64_t value;
    #pragma db null
    std::string locking_script_id;

    /**
     *  CTxIn
     **/
    #pragma db null
    std::string input_block_hash;
    #pragma db null
    unsigned int input_transaction_index;
    #pragma db null
    std::string input_transaction_hash;
    #pragma db null
    unsigned int input_vector;
    #pragma db null
    std::string unlocking_script_id;
    #pragma db null
    uint32_t sequence;

private:
    friend class odb::access;

    eOutputs() {}

};

#endif //ENTERPRISE_OUTPUTS_H
