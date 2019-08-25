#include <chain.h>
#include <chainparams.h>
#include <consensus/validation.h>
#include <core_io.h>
#include <logging.h>
#include <pubkey.h>
#include <primitives/block.h>
#include <rpc/blockchain.h>
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

BlockToSql::BlockToSql(const CBlockIndex block_index, const CBlock block) : m_block_index(block_index),
                                                                            m_block(block),
                                                                            m_block_header_hash(block.GetBlockHeader().GetHash().GetHex())
{
    // Check if block is already in the SQL database
    std::auto_ptr<odb::database> enterprise_database(create_enterprise_database());
    {
        typedef odb::query<eBlocks> query;
        odb::transaction t(enterprise_database->begin());
        std::auto_ptr<eBlocks> r(enterprise_database->query_one<eBlocks>(query::hash == m_block_header_hash));
        if (r.get() != 0) {
            return;
        }
        t.commit();
    }

    // Initialize the block's SQL data record
    eBlocks block_record(
            m_block_header_hash,                   // hash
            m_block_index.hashMerkleRoot.GetHex(), // merkle_root
            m_block_index.GetBlockTime(),          // time
            m_block_index.GetMedianTimePast(),     // median_time
            m_block_index.nHeight,                 // height
            GetBlockSubsidy(m_block_index.nHeight, Params().GetConsensus()), // subsidy
            m_block_index.nTx,                     // transactions_count
            m_block_index.nVersion,                // version
            m_block_index.nStatus,                 // status
            m_block_index.nBits,                   // bits
            m_block_index.nNonce,                  // nonce
            GetDifficulty(&m_block_index),         // difficulty
            m_block_index.nChainWork.GetHex()      // chain_work
    );

    for (std::size_t transaction_index = 0; transaction_index < m_block.vtx.size(); ++transaction_index) {
        const CTransactionRef& transaction = m_block.vtx[transaction_index];

        TransactionData transaction_data = TransactionData {transaction_index, m_block.vtx[transaction_index]};

        // Outputs
        for (std::size_t output_vector = 0; output_vector < transaction->vout.size(); ++output_vector) {
            const CTxOut& txout_data = transaction->vout[output_vector];
            transaction_data.total_output_value += txout_data.nValue;
            transaction_data.utxo_size_inc += GetSerializeSize(txout_data, PROTOCOL_VERSION) + PER_UTXO_OVERHEAD;
        }

        // Inputs
        for (std::size_t input_vector = 0; input_vector < transaction->vin.size(); ++input_vector) {
            if (transaction_data.is_coinbase) {
                continue;
            }

            const CTxIn& txin_data = transaction->vin[input_vector];

            CTransactionRef output_transaction;
            uint256 hash_block;
            bool in_current_block = false;
            bool was_found = GetTransaction(txin_data.prevout.hash, output_transaction, Params().GetConsensus(), hash_block);
            if (!was_found) {
                for (std::size_t transaction_index = 0; transaction_index < m_block.vtx.size(); ++transaction_index) {
                    const CTransactionRef& transaction = m_block.vtx[transaction_index];
                    if (txin_data.prevout.hash == transaction->GetHash() ||
                        txin_data.prevout.hash == transaction->GetWitnessHash()) {
                        output_transaction = transaction;
                        in_current_block = true;
                    }
                }
            }

            if (!was_found && !in_current_block) {
                LogPrintf("%s-%i not found while processing block \n", txin_data.prevout.hash.GetHex(),
                          txin_data.prevout.n);
            }

            const CTxOut& output_txout_data = output_transaction->vout[txin_data.prevout.n];
            transaction_data.total_input_value += output_txout_data.nValue;
            transaction_data.utxo_size_inc -= (GetSerializeSize(output_txout_data, PROTOCOL_VERSION) + PER_UTXO_OVERHEAD);
        }

        block_record.segwit_spend_count += transaction_data.is_segwit_out_spend;
        block_record.outputs_count += transaction_data.m_transaction->vout.size();
        block_record.inputs_count += transaction_data.m_transaction->vin.size();
        block_record.total_output_value += transaction_data.total_output_value;
        block_record.total_fees += transaction_data.GetFee();
        block_record.total_size += transaction_data.m_transaction->GetTotalSize();
        block_record.total_vsize += transaction_data.vsize;
        block_record.total_weight += transaction_data.weight;
        std::ostringstream oss2;
        oss2 << "";
        oss2 << transaction_data.m_transaction->GetTotalSize() << "," ;
        oss2 << transaction_data.vsize << ",";
        oss2 << transaction_data.weight << ",";
        oss2 << transaction_data.GetFee();
        oss2 << ";";
        block_record.fee_data += oss2.str();
    }

    // Insert block into the database
    odb::transaction t(enterprise_database->begin(), false);
    odb::transaction::current(t);
    enterprise_database->persist(block_record);
    t.commit();
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
