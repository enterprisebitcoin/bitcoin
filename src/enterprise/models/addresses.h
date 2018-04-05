#ifndef ENTERPRISE_ADDRESSES_H
#define ENTERPRISE_ADDRESSES_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

/**
 *
 * A CTxDestination
 *
 **/

class eAddresses {
public:
    eAddresses(
            std::string script_id,
            std::string address,
            std::string source_type,
            std::string hex,
            unsigned int version,
            unsigned int length,
            std::string program
    ) : script_id(script_id),
        address(address),
        source_type(source_type),
        hex(hex),
        version(version),
        length(length),
        program(program) {};

#pragma db id auto
    unsigned int id;

    std::string script_id;
    std::string address;
    std::string source_type;
    std::string hex;
    unsigned int version;
    unsigned int length;
    std::string program;

private:
    friend class odb::access;

    eAddresses() {}

};

#endif //ENTERPRISE_ADDRESSES_H
