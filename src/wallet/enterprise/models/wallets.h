#ifndef ENTERPRISE_WALLETS_H
#define ENTERPRISE_WALLETS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include <boost/uuid/uuid.hpp>

#include "enterprise/version.h"

#pragma db object

class eWallets {
public:
    eWallets(
            const boost::uuids::uuid &wallet_id,
            const std::string &name,
            const std::string &description
    )
            :
            wallet_id(wallet_id),
            name(name),
            description(description) {
    }

#pragma db id auto
    unsigned int id;

#pragma db unique type("UUID") not_null
    boost::uuids::uuid wallet_id;

    std::string name;
    std::string description;


private:
    friend class odb::access;

    eWallets() {}

};

#endif //ENTERPRISE_WALLETS_H
