#ifndef BLOCK_TO_SQL_H
#define BLOCK_TO_SQL_H

#include <chain.h>

#include "enterprise/models/addresses.h"
#include "enterprise/models/blocks.h"
#include "enterprise/models/inputs.h"
#include "enterprise/models/outputs.h"
#include "enterprise/models/scripts.h"
#include "enterprise/models/transactions.h"

static constexpr size_t PER_UTXO_OVERHEAD = sizeof(COutPoint) + sizeof(uint32_t) + sizeof(bool);

template<typename T>
void Insert(const std::vector <T> &records);

struct FeeData {
    unsigned int fee;
    unsigned int size;
    unsigned int vsize;
    unsigned int weight;
};

class BlockToSql {

    const CBlockIndex m_block_index;
    const CBlock m_block;

    const std::string m_block_header_hash;

    eBlocks m_block_record;
    std::vector <eBlocks> m_block_records;
    std::vector <eAddresses> m_address_records;
    std::vector <eInputs> m_input_records;
    std::vector <eOutputs> m_output_records;
    std::vector <eScripts> m_script_records;
    std::vector <eTransactions> m_transaction_records;
    std::vector <FeeData> m_fee_data;

    bool GetBlockRecord();

    void GetTransactionRecord(const int &transaction_index, const CTransactionRef &transaction);

    void GetOutputRecord(const int &output_transaction_index,
                         const std::string &output_transaction_hash,
                         const int &output_vector,
                         const CTxOut &txout_data,
                         CAmount &total_output_value,
                         int64_t &utxo_size_inc);

    void GetInputRecord(const int &input_transaction_index,
                        const std::string &input_transaction_hash,
                        const int &input_vector,
                        const CTxIn &txin_data,
                        const bool is_coinbase,
                        CAmount &total_input_value,
                        int64_t &utxo_size_inc);

public:
    BlockToSql(const CBlockIndex block_index, const CBlock block);

    void InsertBlock(const bool insert_transactions = false);
};

#endif //BLOCK_TO_SQL_H
