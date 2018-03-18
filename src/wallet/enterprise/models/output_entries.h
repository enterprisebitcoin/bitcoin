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
            unsigned int etransaction_id,
            unsigned int vector,
            int64_t amount,
            std::string category,
            std::string destination
    ) : etransaction_id(etransaction_id),
        vector(vector),
        amount(amount),
        category(category),
        destination(destination) {};

    unsigned int etransaction_id;
    unsigned int vector;
    int64_t amount;
    std::string category;
    std::string destination;

#pragma db id auto
    unsigned int id;

private:
    friend class odb::access;

    eOutputEntries() {}

};

#endif //ENTERPRISE_OUTPUT_ENTRIES_H
