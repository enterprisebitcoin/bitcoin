#ifndef BLOCK_DATABASE_H
#define BLOCK_DATABASE_H

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

struct FeeData
{
    unsigned int fee;
    unsigned int size;
    unsigned int vsize;
    unsigned int weight;
};

struct BlockData
{
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
};


struct TransactionData
{
    const int &m_transaction_index;
    const CTransactionRef &m_transaction;

    CAmount total_output_value = 0;
    CAmount total_input_value = 0;
    CAmount fees = 0;
    int64_t utxo_size_inc = 0;
    unsigned int weight;
    unsigned int vsize;
    bool is_segwit_out_spend;

    std::string transaction_hash;
    bool is_coinbase;

    TransactionData(const int &transaction_index, const CTransactionRef &transaction);

    CAmount GetFee()
    {
        return is_coinbase ? 0 : total_input_value - total_output_value;
    };
};

struct BlockDatabaseConfig
{
    const bool use_addresses_table;
    const bool use_inputs_table;
    const bool use_outputs_table;
    const bool use_scripts_table;
    const bool use_transactions_table;
};
BlockDatabaseConfig default_db_config{
        false, // use_addresses_table
        false, // use_inputs_table
        false, // use_outputs_table
        false, // use_scripts_table
        false  // use_transactions_table

};

class BlockDatabase
{
    bool GetBlockRecord(BlockData &block_data);

    void GetTransactionRecord(BlockData &block_data, const int &transaction_index);

    void GetOutputRecord(BlockData &block_data, TransactionData &transaction_data, const int &output_vector);

    void GetInputRecord(BlockData &block_data, TransactionData &transaction_data, const int &input_vector);

public:
    BlockDatabaseConfig m_block_db_config;

    BlockDatabase(BlockDatabaseConfig block_db_config = default_db_config) : m_block_db_config(block_db_config)
    {};

    void InsertBlock(const CBlockIndex block_index, const CBlock block, const bool insert_transactions = false);
};

#endif //BLOCK_DATABASE_H
