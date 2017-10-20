#ifndef ENTERPRISE_ADDRESSES_H
#define ENTERPRISE_ADDRESSES_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "version.h"

#pragma db object

class eAddresses {
public:
    eAddresses(
            std::string address,
            std::string name,
            std::string purpose
    ) : address(address),
        name(name),
        purpose(purpose) {};

    std::string name;
    std::string purpose;

#pragma db id auto
    unsigned int id;

#pragma db unique
    std::string address;

private:
    friend class odb::access;

    eAddresses() {}

};

#endif //ENTERPRISE_ADDRESSES_H
