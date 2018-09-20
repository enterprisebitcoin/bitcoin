#include <sstream>

#include <core_io.h>
#include <chain.h>
#include <chainparams.h>
#include <consensus/validation.h>
#include <pubkey.h>
#include <script/standard.h>
#include <serialize.h>
#include <validation.h>

#include <enterprise/block_to_sql.h>
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

BlockToSql::BlockToSql(const CBlockIndex block_index, const CBlock block) :
        m_block_index(block_index),
        m_block(block),
        m_block_header_hash(block.GetBlockHeader().GetHash().GetHex()) {

    bool block_already_processed = GetBlockRecord();
    if(!block_already_processed) {
        for (std::size_t transaction_index = 0; transaction_index < m_block.vtx.size(); ++transaction_index) {
            const CTransactionRef &transaction = m_block.vtx[transaction_index];
            GetTransactionRecord(transaction_index, transaction);
        }
    }
}

void BlockToSql::InsertBlock(const bool insert_transactions) {
    if (insert_transactions) {
        int thread_count = 4;
        std::vector <std::vector<eTransactions>> work(thread_count);
        std::vector <std::thread> threads;
        for (size_t i = 0; i < m_transaction_records.size(); ++i) {
            int thread_index = i % thread_count;
            work[thread_index].push_back(m_transaction_records[i]);
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

    m_block_records.push_back(m_block_record);
    std::thread t6(Insert<eBlocks>, m_block_records);
    t6.join();
}

bool BlockToSql::GetBlockRecord() {
    bool block_already_processed = false;
    std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
    {
        typedef odb::query <eBlocks> query;
        odb::transaction t(enterprise_database->begin());
        std::auto_ptr <eBlocks> r(enterprise_database->query_one<eBlocks>(query::hash == m_block_header_hash));
        if (r.get() != 0) {
            block_already_processed = true;
        }
        t.commit();
    }
    if (block_already_processed) return block_already_processed;

    eBlocks block_record(
            m_block_header_hash, // hash
            m_block_index.hashMerkleRoot.GetHex(), // merkle_root
            m_block_index.GetBlockTime(), // time
            m_block_index.GetMedianTimePast(), // median_time
            m_block_index.nHeight, // height
            GetBlockSubsidy(m_block_index.nHeight, Params().GetConsensus()), // subsidy
            m_block_index.nTx, // transactions_count
            m_block_index.nVersion, // version
            m_block_index.nStatus, // status
            m_block_index.nBits, // bits
            m_block_index.nNonce, // nonce
            GetDifficulty(&m_block_index),
            m_block_index.nChainWork.GetHex() // chain_work
    );
    m_block_record = block_record;
    return false;
}

void BlockToSql::GetTransactionRecord(const int &transaction_index, const CTransactionRef &transaction) {
    CAmount total_output_value = 0;
    CAmount total_input_value = 0;
    const std::string transaction_hash = transaction->GetHash().GetHex();
    const bool is_coinbase = transaction->IsCoinBase();
    int64_t utxo_size_inc = 0;

    // Outputs
    for (std::size_t output_vector = 0; output_vector < transaction->vout.size(); ++output_vector) {
        const CTxOut &txout_data = transaction->vout[output_vector];
        GetOutputRecord(transaction_index, transaction_hash, output_vector, txout_data, total_output_value,
                        utxo_size_inc);
    }

    // Inputs
    for (std::size_t input_vector = 0; input_vector < transaction->vin.size(); ++input_vector) {
        const CTxIn &txin_data = transaction->vin[input_vector];
        GetInputRecord(transaction_index, transaction_hash, input_vector, txin_data, is_coinbase, total_input_value,
                       utxo_size_inc);
    }

    CAmount fees = is_coinbase ? 0 : total_input_value - total_output_value;
    unsigned int weight = GetTransactionWeight(*transaction);
    unsigned int vsize = (weight + WITNESS_SCALE_FACTOR - 1) / WITNESS_SCALE_FACTOR;

    bool is_segwit_out_spend = is_coinbase ? false : !(transaction->GetHash() == transaction->GetWitnessHash());
    m_block_record.segwit_spend_count += is_segwit_out_spend;
    m_block_record.outputs_count += transaction->vout.size();
    m_block_record.inputs_count += transaction->vin.size();
    m_block_record.total_output_value += total_output_value;
    m_block_record.total_fees += fees;
    m_block_record.total_size += transaction->GetTotalSize();
    m_block_record.total_vsize += vsize;
    m_block_record.total_weight += weight;

    std::ostringstream oss;
    oss << transaction->GetTotalSize() << "," << vsize << "," << weight << "," << fees << ";";
    m_block_record.fee_data += oss.str();

    eTransactions transaction_record(
            m_block_header_hash, // block_hash
            transaction_index, // index
            transaction->GetTotalSize(), // size
            vsize, // vsize
            weight, // weight
            transaction->vin.size(), // inputs_count
            transaction->vout.size(), // outputs_count
            transaction->nLockTime, // lock_time
            transaction->nVersion, // version
            transaction->GetHash().GetHex(), // hash
            transaction->GetWitnessHash().GetHex(), // witness_hash
            is_coinbase, // is_coinbase
            transaction->HasWitness(), // has_witness
            is_segwit_out_spend, // is_segwit_out_spend
            total_output_value, // total_output_value
            total_input_value, // total_input_value
            fees, // fees
            static_cast<float>(fees) / transaction->GetTotalSize(), // fee_per_byte
            static_cast<float>(fees) / vsize, // fee_per_vbyte
            static_cast<float>(fees) / weight // fee_per_weight_unit
    );
    m_transaction_records.push_back(transaction_record);

}

void BlockToSql::GetOutputRecord(const int &output_transaction_index, const std::string &output_transaction_hash,
                                 const int &output_vector, const CTxOut &txout_data, CAmount &total_output_value,
                                 int64_t &utxo_size_inc) {
    total_output_value += txout_data.nValue;
    utxo_size_inc += GetSerializeSize(txout_data, PROTOCOL_VERSION) + PER_UTXO_OVERHEAD;

    txnouttype output_script_type;
    std::vector <CTxDestination> destinations;
    int nRequired;
    ExtractDestinations(txout_data.scriptPubKey, output_script_type, destinations, nRequired);

    eOutputs output_record(
            m_block_header_hash, // output_block_hash
            output_transaction_index, // output_transaction_index
            output_transaction_hash, // output_transaction_hash
            output_vector, // output_vector
            txout_data.nValue, // value
            CScriptID(txout_data.scriptPubKey).GetHex(), // locking_script_id
            nRequired // required_signatures
    );
    m_output_records.push_back(output_record);


    eScripts locking_script_record(
            CScriptID(txout_data.scriptPubKey).GetHex(), // id
            ScriptToAsmStr(txout_data.scriptPubKey), // script
            GetTxnOutputType(output_script_type) // type
    );
    m_script_records.push_back(locking_script_record);

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
        m_address_records.push_back(address_record);
    }
}

void BlockToSql::GetInputRecord(const int &input_transaction_index, const std::string &input_transaction_hash,
                                const int &input_vector, const CTxIn &txin_data, const bool is_coinbase,
                                CAmount &total_input_value, int64_t &utxo_size_inc) {
    eInputs input_record(
            is_coinbase ? "" : txin_data.prevout.hash.GetHex(), // output_transaction_hash
            is_coinbase ? -1 : txin_data.prevout.n, // output_vector
            m_block_header_hash, // input_block_hash
            input_transaction_index, // input_transaction_index
            input_transaction_hash, // input_transaction_hash
            input_vector, // input_vector
            CScriptID(txin_data.scriptSig).GetHex(), // unlocking_script_id
            txin_data.nSequence == 0xffffffff ? -1 : txin_data.nSequence // sequence
    );
    m_input_records.push_back(input_record);

    eScripts unlock_script_record(
            CScriptID(txin_data.scriptSig).GetHex(), // id
            ScriptToAsmStr(txin_data.scriptSig) // script
    );
    m_script_records.push_back(unlock_script_record);

    if (!is_coinbase) {
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
                for (std::size_t transaction_index = 0; transaction_index < m_block.vtx.size(); ++transaction_index) {
                    const CTransactionRef &transaction = m_block.vtx[transaction_index];
                    if (txin_data.prevout.hash == transaction->GetHash() ||
                        txin_data.prevout.hash == transaction->GetWitnessHash()) {
                        output_transaction = transaction;
                        in_current_block = true;
                    }
                }
            }
        }

        if (!in_tx_map && !in_tx_index && !in_current_block) {
            LogPrintf("%s-%i not found while processing block %s\n", txin_data.prevout.hash.GetHex(),
                      txin_data.prevout.n, m_block_header_hash);
        }

        if (!in_tx_map) {
            ENTER_CRITICAL_SECTION(cs_tx_map);
            g_tx_map.emplace(txin_data.prevout.hash, output_transaction);
            LEAVE_CRITICAL_SECTION(cs_tx_map);
        }

        const CTxOut &output_txout_data = output_transaction->vout[txin_data.prevout.n];
        total_input_value += output_txout_data.nValue;
        utxo_size_inc -= (GetSerializeSize(output_txout_data, PROTOCOL_VERSION) + PER_UTXO_OVERHEAD);

    }
}
