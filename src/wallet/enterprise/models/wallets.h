#ifndef ENTERPRISE_WALLETS_H
#define ENTERPRISE_WALLETS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "../version.h"

#pragma db object

class eWallets {
public:
    eWallets(
            const std::string &wallet_id,
            const std::string &name
    )
            :
            wallet_id(wallet_id),
            name(name) {
    }

#pragma db id auto
    unsigned int id;

#pragma db unique
    std::string wallet_id;

    std::string name;


private:
    friend class odb::access;

    eWallets() {}

};

#endif //ENTERPRISE_WALLETS_H
