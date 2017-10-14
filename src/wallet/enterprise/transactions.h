#ifndef ENTERPRISE_TRANSACTIONS_H
#define ENTERPRISE_TRANSACTIONS_H

#include <odb/core.hxx>
#include <string>

#include "version.h"

#pragma db object

class etransactions {
public:
    etransactions(const std::string& txid,
                  unsigned int time);

    const std::string&
    txid () const
    {
        return txid_;
    }

    void
    time (std::string& txid)
    {
        txid_ = txid;
    }

    unsigned int
    time () const
    {
        return time_;
    }

    void
    time (unsigned int time)
    {
        time_ = time;
    }


private:
    friend class odb::access;

    etransactions() {}

#pragma db id auto
    unsigned int id_;

    std::string txid_;
    unsigned int time_;
};

#endif //ENTERPRISE_TRANSACTIONS_H
