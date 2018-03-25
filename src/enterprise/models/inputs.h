#ifndef ENTERPRISE_INPUTS_H
#define ENTERPRISE_INPUTS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

/**
 *
 * A CTxIn
 *
 **/

class eInputs {
public:
    eInputs(
            std::string output_transaction_hash,
            unsigned int output_vector,

            std::string block_hash,
            unsigned int transaction_block_index,
            std::string transaction_hash,
            unsigned int vector,
            std::string unlocking_script_id,
            uint32_t sequence
    ) : output_transaction_hash(output_transaction_hash),
        output_vector(output_vector),

        block_hash(block_hash),
        transaction_block_index(transaction_block_index),
        vector(vector),
        unlocking_script_id(unlocking_script_id),
        sequence(sequence) {};

#pragma db id auto
    unsigned int id;

//    The output that is being spent
    std::string output_transaction_hash;
    unsigned int output_vector;

    std::string block_hash;
    unsigned int transaction_block_index;
    std::string transaction_hash;
    unsigned int vector;
    std::string unlocking_script_id;
    uint32_t sequence;

private:
    friend class odb::access;

    eInputs() {}

};

#endif //ENTERPRISE_INPUTS_H
