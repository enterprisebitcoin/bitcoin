#ifndef ENTERPRISE_ADDRESSES_H
#define ENTERPRISE_ADDRESSES_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

class eAddresses {
public:
    eAddresses(
            std::string address,
            std::string name,
            std::string purpose,
            int64_t time,
            bool is_used
    ) : address(address),
        name(name),
        purpose(purpose),
        time(time),
        is_used(is_used) {};

    std::string name;
    std::string purpose;
    int64_t time;

#pragma db default(false)
    bool is_used;

#pragma db id auto
    unsigned int id;

#pragma db unique
    std::string address;

private:
    friend class odb::access;

    eAddresses() {}

};

#pragma db view object(eAddresses)
struct address_stats
{
#pragma db column("count(" + eAddresses::id + ")")
    std::size_t count;
};


#endif //ENTERPRISE_ADDRESSES_H
