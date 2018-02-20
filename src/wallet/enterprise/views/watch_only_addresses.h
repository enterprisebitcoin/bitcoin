#ifndef ENTERPRISE_WATCH_ONLY_ADDRESSES_H
#define ENTERPRISE_WATCH_ONLY_ADDRESSES_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include <boost/uuid/uuid.hpp>

#include "../version.h"

#pragma db view query("SELECT wallet_id, address, source " \
                        "FROM wallet.watch_only_addresses " \
                                "WHERE watch_only_address_id IS NULL")

struct watch_only_addresses {
    boost::uuids::uuid wallet_id;
    std::string address;
    std::string source;
    unsigned int watch_only_address_id;
};


#endif //ENTERPRISE_WATCH_ONLY_ADDRESSES_H
