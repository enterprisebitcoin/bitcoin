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
            const bool is_trusted,
            const unsigned int size,
            const int64_t time,
            const int64_t time_received,
            const std::string &txid
    )
            :
            block_index_(block_index),
            is_trusted_(is_trusted),
            size_(size),
            time_(time),
            time_received_(time_received),
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

    const bool
    is_trusted() const {
        return is_trusted_;
    }

    void
    is_trusted(bool is_trusted) {
        is_trusted_ = is_trusted;
    }

    const unsigned int
    size() const {
        return size_;
    }

    void
    size(unsigned int size) {
        size_ = size;
    }

    const int64_t
    time() const {
        return time_;
    }

    void
    time(int64_t time) {
        time_ = time;
    }

    const int64_t
    time_received() const {
        return time_received_;
    }

    void
    time_received(int64_t time_received) {
        time_received_ = time_received;
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
    bool is_trusted_;
    unsigned int size_;
    int64_t time_;
    int64_t time_received_;

#pragma db unique
    std::string txid_;

};

#endif //ENTERPRISE_TRANSACTIONS_H
