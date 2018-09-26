#include <sstream>

#include <core_io.h>
#include <chain.h>
#include <chainparams.h>
#include <consensus/validation.h>
#include <pubkey.h>
#include <script/standard.h>
#include <serialize.h>
#include <validation.h>

#include <enterprise/block_database.h>
#include <enterprise/database.h>
#include <enterprise/utilities.h>

#include "enterprise/models/addresses.h"
#include "enterprise/models/blocks.h"
#include "enterprise/models/inputs.h"
#include "enterprise/models/outputs.h"
#include "enterprise/models/scripts.h"
#include "enterprise/models/transactions.h"

#include "enterprise/models/addresses-odb.hxx"
#include "enterprise/models/blocks-odb.hxx"
#include "enterprise/models/inputs-odb.hxx"
#include "enterprise/models/outputs-odb.hxx"
#include "enterprise/models/scripts-odb.hxx"
#include "enterprise/models/transactions-odb.hxx"

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>

#include <rpc/blockchain.h>

CCriticalSection cs_tx_map;
std::map<uint256, CTransactionRef> g_tx_map GUARDED_BY(cs_tx_map);

template<typename T>
void Insert(const std::vector <T> &records) {
    std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
    odb::transaction t(enterprise_database->begin(), false);
    odb::transaction::current(t);

    for (T record: records) {
        enterprise_database->persist(record);
    }
    t.commit();
}

void BlockDatabase::InsertBlock(const CBlockIndex block_index, const CBlock block, const bool insert_transactions)
{

    BlockData block_data = BlockData {block_index, block, block.GetBlockHeader().GetHash().GetHex()};
    bool block_already_processed = GetBlockRecord(block_data);
    if (block_already_processed) return;

    std::ostringstream oss;
    oss << "[['size'" << "," << "'vsize'" << "," << "'weight'" << "," << "'fee']," << "\n";
    block_data.m_block_record.fee_data += oss.str();


    for (std::size_t transaction_index = 0; transaction_index < block_data.m_block.vtx.size(); ++transaction_index)
    {
         GetTransactionRecord(block_data, transaction_index);
    }

    LOCK(cs_tx_map);
    LogPrintf("Current g_tx_map size: %i\n", g_tx_map.size());

    if (insert_transactions) {
        int thread_count = 4;
        std::vector <std::vector<eTransactions>> work(thread_count);
        std::vector <std::thread> threads;
        for (size_t i = 0; i < block_data.m_transaction_records.size(); ++i) {
            int thread_index = i % thread_count;
            work[thread_index].push_back(block_data.m_transaction_records[i]);
        }

        for (int i = 0; i < thread_count; ++i) {
            std::thread work_thread(Insert<eTransactions>, work[i]);
            threads.push_back(std::move(work_thread));
        }

        for (std::thread &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        threads.clear();
    }

    std::ostringstream oss_2;
    oss_2 << "]\n";
    block_data.m_block_record.fee_data += oss_2.str();

    block_data.m_block_records.push_back(block_data.m_block_record);
    std::thread t6(Insert<eBlocks>, block_data.m_block_records);
    t6.join();
}

bool BlockDatabase::GetBlockRecord(BlockData &block_data) {
    bool block_already_processed = false;
    std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
    {
        typedef odb::query <eBlocks> query;
        odb::transaction t(enterprise_database->begin());
        std::auto_ptr <eBlocks> r(enterprise_database->query_one<eBlocks>(query::hash == block_data.m_block_header_hash));
        if (r.get() != 0) {
            block_already_processed = true;
        }
        t.commit();
    }
    if (block_already_processed) return true;

    eBlocks block_record(
            block_data.m_block_header_hash, // hash
            block_data.m_block_index.hashMerkleRoot.GetHex(), // merkle_root
            block_data.m_block_index.GetBlockTime(), // time
            block_data.m_block_index.GetMedianTimePast(), // median_time
            block_data.m_block_index.nHeight, // height
            GetBlockSubsidy(block_data.m_block_index.nHeight, Params().GetConsensus()), // subsidy
            block_data.m_block_index.nTx, // transactions_count
            block_data.m_block_index.nVersion, // version
            block_data.m_block_index.nStatus, // status
            block_data.m_block_index.nBits, // bits
            block_data.m_block_index.nNonce, // nonce
            GetDifficulty(&block_data.m_block_index),
            block_data.m_block_index.nChainWork.GetHex() // chain_work
    );
    block_data.m_block_record = block_record;
    return false;
}

void BlockDatabase::GetTransactionRecord(BlockData &block_data, const int &transaction_index) {
    TransactionData transaction_data = TransactionData {transaction_index, block_data.m_block.vtx[transaction_index]};

    // Outputs
    for (std::size_t output_vector = 0; output_vector < transaction_data.m_transaction->vout.size(); ++output_vector) {
        GetOutputRecord(block_data, transaction_data, output_vector);
    }

    // Inputs
    for (std::size_t input_vector = 0; input_vector < transaction_data.m_transaction->vin.size(); ++input_vector) {
        GetInputRecord(block_data, transaction_data, input_vector);
    }

    block_data.m_block_record.segwit_spend_count += transaction_data.is_segwit_out_spend;
    block_data.m_block_record.outputs_count += transaction_data.m_transaction->vout.size();
    block_data.m_block_record.inputs_count += transaction_data.m_transaction->vin.size();
    block_data.m_block_record.total_output_value += transaction_data.total_output_value;
    block_data.m_block_record.total_fees += transaction_data.GetFee();
    block_data.m_block_record.total_size += transaction_data.m_transaction->GetTotalSize();
    block_data.m_block_record.total_vsize += transaction_data.vsize;
    block_data.m_block_record.total_weight += transaction_data.weight;

    std::ostringstream oss;
    oss << "[";
    oss << transaction_data.m_transaction->GetTotalSize() << "," ;
    oss << transaction_data.vsize << ",";
    oss << transaction_data.weight << ",";
    oss << transaction_data.GetFee();
    oss << "]," << "\n";
    block_data.m_block_record.fee_data += oss.str();

    eTransactions transaction_record(
            block_data.m_block_header_hash, // block_hash
            transaction_data.m_transaction_index, // index
            transaction_data.m_transaction->GetTotalSize(), // size
            transaction_data.vsize, // vsize
            transaction_data.weight, // weight
            transaction_data.m_transaction->vin.size(), // inputs_count
            transaction_data.m_transaction->vout.size(), // outputs_count
            transaction_data.m_transaction->nLockTime, // lock_time
            transaction_data.m_transaction->nVersion, // version
            transaction_data.m_transaction->GetHash().GetHex(), // hash
            transaction_data.m_transaction->GetWitnessHash().GetHex(), // witness_hash
            transaction_data.is_coinbase, // is_coinbase
            transaction_data.m_transaction->HasWitness(), // has_witness
            transaction_data.is_segwit_out_spend, // is_segwit_out_spend
            transaction_data.total_output_value, // total_output_value
            transaction_data.total_input_value, // total_input_value
            transaction_data.fees, // fees
            static_cast<float>(transaction_data.fees) / transaction_data.m_transaction->GetTotalSize(), // fee_per_byte
            static_cast<float>(transaction_data.fees) / transaction_data.vsize, // fee_per_vbyte
            static_cast<float>(transaction_data.fees) / transaction_data.weight // fee_per_weight_unit
    );
    block_data.m_transaction_records.push_back(transaction_record);

}

void BlockDatabase::GetOutputRecord(BlockData &block_data, TransactionData &transaction_data, const int &output_vector)
{
    const CTxOut &txout_data = transaction_data.m_transaction->vout[output_vector];

    transaction_data.total_output_value += txout_data.nValue;
    transaction_data.utxo_size_inc += GetSerializeSize(txout_data, PROTOCOL_VERSION) + PER_UTXO_OVERHEAD;

    txnouttype output_script_type;
    std::vector <CTxDestination> destinations;
    int nRequired;

    if (m_block_db_config.use_outputs_table or m_block_db_config.use_scripts_table or m_block_db_config.use_addresses_table)
    {
        ExtractDestinations(txout_data.scriptPubKey, output_script_type, destinations, nRequired);
    }
    if (m_block_db_config.use_outputs_table)
    {

        eOutputs output_record(
                block_data.m_block_header_hash, // output_block_hash
                transaction_data.m_transaction_index, // output_transaction_index
                transaction_data.transaction_hash, // output_transaction_hash
                output_vector, // output_vector
                txout_data.nValue, // value
                CScriptID(txout_data.scriptPubKey).GetHex(), // locking_script_id
                nRequired // required_signatures
        );
        block_data.m_output_records.push_back(output_record);
    }

    if (m_block_db_config.use_scripts_table)
    {
        eScripts locking_script_record(
                CScriptID(txout_data.scriptPubKey).GetHex(), // id
                ScriptToAsmStr(txout_data.scriptPubKey), // script
                GetTxnOutputType(output_script_type) // type
        );
        block_data.m_script_records.push_back(locking_script_record);
    }

    if (m_block_db_config.use_addresses_table)
    {
        for (CTxDestination &destination: destinations) {
            EnterpriseDestination enterprise_destination = boost::apply_visitor(EnterpriseDestinationEncoder(Params()),
                                                                                destination);
            eAddresses address_record(
                    CScriptID(txout_data.scriptPubKey).GetHex(),
                    enterprise_destination.address,
                    enterprise_destination.source_type,
                    enterprise_destination.hex,
                    enterprise_destination.version,
                    enterprise_destination.length,
                    enterprise_destination.program
            );
            block_data.m_address_records.push_back(address_record);
        }
    }
}

void BlockDatabase::GetInputRecord(BlockData &block_data, TransactionData &transaction_data, const int &input_vector)
{
    const CTxIn &txin_data = transaction_data.m_transaction->vin[input_vector];

    if (m_block_db_config.use_inputs_table) {
        eInputs input_record(
                transaction_data.is_coinbase ? "" : txin_data.prevout.hash.GetHex(), // output_transaction_hash
                transaction_data.is_coinbase ? -1 : txin_data.prevout.n, // output_vector
                block_data.m_block_header_hash, // input_block_hash
                transaction_data.m_transaction_index, // input_transaction_index
                transaction_data.transaction_hash, // input_transaction_hash
                input_vector, // input_vector
                CScriptID(txin_data.scriptSig).GetHex(), // unlocking_script_id
                txin_data.nSequence == 0xffffffff ? -1 : txin_data.nSequence // sequence
        );
        block_data.m_input_records.push_back(input_record);
    }

    if (m_block_db_config.use_scripts_table) {
        eScripts unlock_script_record(
                CScriptID(txin_data.scriptSig).GetHex(), // id
                ScriptToAsmStr(txin_data.scriptSig) // script
        );
        block_data.m_script_records.push_back(unlock_script_record);
    }

    if (!transaction_data.is_coinbase) {
        bool in_tx_map = false;
        bool in_tx_index = false;
        bool in_current_block = false;

        CTransactionRef output_transaction;

        ENTER_CRITICAL_SECTION(cs_tx_map);
        auto search = g_tx_map.find(txin_data.prevout.hash);
        in_tx_map = search != g_tx_map.end();
        LEAVE_CRITICAL_SECTION(cs_tx_map);

        if (in_tx_map) {
            in_tx_map = true;
            output_transaction = search->second;
        } else {
            uint256 hash_block;
            in_tx_index = GetTransaction(txin_data.prevout.hash, output_transaction, Params().GetConsensus(), hash_block,
                                            false);

            if (!in_tx_index) {
                for (std::size_t transaction_index = 0; transaction_index < block_data.m_block.vtx.size(); ++transaction_index) {
                    const CTransactionRef &transaction = block_data.m_block.vtx[transaction_index];
                    if (txin_data.prevout.hash == transaction_data.m_transaction->GetHash() ||
                        txin_data.prevout.hash == transaction_data.m_transaction->GetWitnessHash()) {
                        output_transaction = transaction;
                        in_current_block = true;
                    }
                }
            }
        }

        if (!in_tx_map && !in_tx_index && !in_current_block) {
            LogPrintf("%s-%i not found while processing block %s\n", txin_data.prevout.hash.GetHex(),
                      txin_data.prevout.n, block_data.m_block_header_hash);
        }

        if (!in_tx_map) {
            ENTER_CRITICAL_SECTION(cs_tx_map);
            g_tx_map.emplace(txin_data.prevout.hash, output_transaction);
            LEAVE_CRITICAL_SECTION(cs_tx_map);
        }

        const CTxOut &output_txout_data = output_transaction->vout[txin_data.prevout.n];
        transaction_data.utxo_size_inc -= (GetSerializeSize(output_txout_data, PROTOCOL_VERSION) + PER_UTXO_OVERHEAD);
        transaction_data.total_input_value += output_txout_data.nValue;
    }
}

TransactionData::TransactionData(const int &transaction_index, const CTransactionRef &transaction) :
        m_transaction_index(transaction_index),
        m_transaction(transaction)
{
    transaction_hash = transaction->GetHash().GetHex();
    is_coinbase = transaction->IsCoinBase();
    weight = GetTransactionWeight(*transaction);
    vsize = (weight + WITNESS_SCALE_FACTOR - 1) / WITNESS_SCALE_FACTOR;
    is_segwit_out_spend = is_coinbase ? false : !(transaction->GetHash() == transaction->GetWitnessHash());
};