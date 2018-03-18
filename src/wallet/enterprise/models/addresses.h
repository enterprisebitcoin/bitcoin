#ifndef ENTERPRISE_ADDRESSES_H
#define ENTERPRISE_ADDRESSES_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include <boost/uuid/uuid.hpp>

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
            bool is_used,
            boost::uuids::uuid wallet_id
    ) : p2pkh_address(p2pkh_address),
        sw_bech32_address(sw_bech32_address),
        sw_p2sh_address(sw_p2sh_address),
        name(name),
        purpose(purpose),
        time(time),
        is_used(is_used),
        wallet_id(wallet_id) {};


//    traditional base58 address
#pragma db unique
    std::string p2pkh_address;

    std::string sw_bech32_address;

    std::string sw_p2sh_address;

    std::string name;

    std::string purpose;

    int64_t time;

#pragma db default(false)
    bool is_used;

#pragma db type("UUID") not_null
    boost::uuids::uuid wallet_id;

#pragma db id auto
    unsigned int id;

private:
    friend class odb::access;

    eAddresses() {}

};

#pragma db view object(eAddresses)
struct address_stats {
#pragma db column("count(" + eAddresses::id + ")")
    std::size_t count;
};


#endif //ENTERPRISE_ADDRESSES_H
