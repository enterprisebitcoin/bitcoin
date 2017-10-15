#ifndef ENTERPRISE_TRANSACTIONS_H
#define ENTERPRISE_TRANSACTIONS_H

#include <stdint.h>
#include <string>

#include <odb/core.hxx>

#include "version.h"

#pragma db object

class etransactions {
public:
    etransactions(
            const int block_index,
            const int64_t time,
            const std::string &txid
    )
            :
            block_index_(block_index),
            time_(time),
            txid_(txid) {
    }

    const int
    block_index() const {
        return block_index_;
    }

    void
    block_index(int block_index) {
        block_index_ = block_index;
    }

    const int64_t
    time() const {
        return time_;
    }

    void
    time(int64_t time) {
        time_ = time;
    }

    const std::string &
    txid() const {
        return txid_;
    }

    void
    txid(std::string &txid) {
        txid_ = txid;
    }


private:
    friend class odb::access;

    etransactions() {}

#pragma db id auto
    unsigned int id_;

    int block_index_;
    int64_t time_;

#pragma db unique
    std::string txid_;

};

#endif //ENTERPRISE_TRANSACTIONS_H
