#ifndef ENTERPRISE_OUTPUT_ENTRIES_H
#define ENTERPRISE_OUTPUT_ENTRIES_H

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

class eOutputEntries {
public:
    eOutputEntries(
            unsigned int output_etransaction_id,
            unsigned int output_vector,
            uint8_t is_output_mine,
            int64_t n_value,
            std::string destination,
            std::string script_pub_key,

            unsigned int input_etransaction_id,
            unsigned int input_vector,
            std::string script_sig,
            std::string script_witness,
            std::string n_sequence
    ) : output_etransaction_id(output_etransaction_id),
        output_vector(output_vector),
        is_output_mine(is_output_mine),
        n_value(n_value),
        destination(destination),
        script_pub_key(script_pub_key),

        input_etransaction_id(input_etransaction_id),
        input_vector(input_vector),
        script_sig(script_sig),
        script_witness(script_witness),
        n_sequence(n_sequence) {};

    #pragma db id auto
    unsigned int id;

    /**
     *  CTxOut
     **/
    unsigned int output_etransaction_id;
    unsigned int output_vector;
    uint8_t is_output_mine;
    int64_t n_value;
    std::string destination;
    std::string script_pub_key;

    /**
     *  CTxIn
     **/
    #pragma db null
    unsigned int input_etransaction_id;
    #pragma db null
    unsigned int input_vector;
    #pragma db null
    std::string script_sig;
    #pragma db null
    std::string script_witness;
    #pragma db null
    std::string n_sequence;

private:
    friend class odb::access;

    eOutputEntries() {}

};

#endif //ENTERPRISE_OUTPUT_ENTRIES_H
