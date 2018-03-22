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

            std::string input_block_hash,
            unsigned int input_transaction_index,
            std::string input_transaction_hash,
            unsigned int input_vector,
            std::string unlocking_script_id,
            uint32_t sequence
    ) : output_transaction_hash(output_transaction_hash),
        output_vector(output_vector),

        input_block_hash(input_block_hash),
        input_transaction_index(input_transaction_index),
        input_vector(input_vector),
        unlocking_script_id(unlocking_script_id),
        sequence(sequence) {};

#pragma db id auto
    unsigned int id;

    std::string output_transaction_hash;
    unsigned int output_vector;

    std::string input_block_hash;
    unsigned int input_transaction_index;
    std::string input_transaction_hash;
    unsigned int input_vector;
    std::string unlocking_script_id;
    uint32_t sequence;

private:
    friend class odb::access;

    eInputs() {}

};

#endif //ENTERPRISE_INPUTS_H
