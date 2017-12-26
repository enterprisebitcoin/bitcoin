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
            std::string p2pkh_address,
            std::string sw_bech32_address,
            std::string sw_p2sh_address,
            std::string name,
            std::string purpose,
            int64_t time,
            bool is_used
    ) : p2pkh_address(p2pkh_address),
        sw_bech32_address(sw_bech32_address),
        sw_p2sh_address(sw_p2sh_address),
        name(name),
        purpose(purpose),
        time(time),
        is_used(is_used) {};

    std::string name;
    std::string purpose;
    int64_t time;

    std::string sw_bech32_address;
    std::string sw_p2sh_address;

#pragma db default(false)
    bool is_used;

#pragma db id auto
    unsigned int id;

//    traditional base58 address
#pragma db unique
    std::string p2pkh_address;

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
