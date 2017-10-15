#ifndef ENTERPRISE_OUTPUT_ENTRIES_H
#define ENTERPRISE_OUTPUT_ENTRIES_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "version.h"

#pragma db object

class eOutputEntries {
public:
    eOutputEntries(
            const unsigned int etransaction_id,
            const unsigned int vector,
            const int64_t amount,
            const std::string category,
            const std::string destination
    )
            :
            etransaction_id_(etransaction_id),
            vector_(vector),
            amount_(amount),
            category_(category),
            destination_(destination) {
    }

    const unsigned int
    etransaction_id() const {
        return etransaction_id_;
    }

    void
    etransaction_id(int etransaction_id) {
        etransaction_id_ = etransaction_id;
    }

    const unsigned int
    vector() const {
        return vector_;
    }

    void
    vector(unsigned int vector) {
        vector_ = vector;
    }

    const int64_t
    amount() const {
        return amount_;
    }

    void
    amount(int64_t amount) {
        amount_ = amount;
    }

    const std::string &
    category() const {
        return category_;
    }

    void
    category(std::string category) {
        category_ = category;
    }

    const std::string &
    destination() const {
        return destination_;
    }

    void
    destination(std::string destination) {
        destination_ = destination;
    }

    const unsigned int
    id() const {
        return id_;
    }

private:
    friend class odb::access;

    eOutputEntries() {}

#pragma db id auto
    unsigned int id_;

    unsigned int etransaction_id_;
    unsigned int vector_;
    int64_t amount_;
    std::string category_;
    std::string destination_;

#pragma db unique
    std::string txid_;

};

#endif //ENTERPRISE_OUTPUT_ENTRIES_H
