#ifndef ENTERPRISE_OUTPUT_ENTRIES_H
#define ENTERPRISE_OUTPUT_ENTRIES_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

class eOutputEntries {
public:
    eOutputEntries(
            unsigned int output_etransaction_id,
            unsigned int output_vector,
            uint8_t is_output_mine,
            unsigned int input_etransaction_id,
            unsigned int input_vector,
            int64_t amount,
            std::string destination
    ) : output_etransaction_id(output_etransaction_id),
        output_vector(output_vector),
        output_is_mine(output_is_mine),
        input_etransaction_id(input_etransaction_id),
        input_vector(input_vector),
        amount(amount),
        destination(destination) {};

    unsigned int output_etransaction_id;
    unsigned int output_vector;
    uint8_t output_is_mine;
    unsigned int input_etransaction_id;
    unsigned int input_vector;
    int64_t amount;
    std::string destination;

#pragma db id auto
    unsigned int id;

private:
    friend class odb::access;

    eOutputEntries() {}

};

#endif //ENTERPRISE_OUTPUT_ENTRIES_H
