#include <core_io.h>
#include <chain.h>
#include <script/standard.h>

#include <enterprise/database.h>
#include <enterprise/enterprise_bitcoin.h>

#include "enterprise/models/blocks.h"
#include "enterprise/models/outputs.h"
#include "enterprise/models/scripts.h"
#include "enterprise/models/transactions.h"

#include "enterprise/models/blocks-odb.hxx"
#include "enterprise/models/outputs-odb.hxx"
#include "enterprise/models/scripts-odb.hxx"
#include "enterprise/models/transactions-odb.hxx"

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>


namespace enterprise_bitcoin {

    void UpsertBlocks(const std::vector <BlockData> &blocks) {

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());

        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (const BlockData &block_data: blocks) {
            CBlockIndex block_index = block_data.first;
            CBlock block = block_data.second;

            eBlocks new_block_record(
                    block.GetBlockHeader().GetHash().GetHex(), // hash
                    block_index.hashMerkleRoot.GetHex(), // merkle_root
                    block_index.GetBlockTime(), // time
                    block_index.nHeight, // height
                    block_index.nTx, // transactions_count
                    block_index.nVersion, // version
                    block_index.nStatus, // status
                    block_index.nBits, // bits
                    block_index.nNonce // nonce
            );
            enterprise_database->persist(new_block_record);

            for (std::size_t i = 0; i < block.vtx.size(); ++i) {
                const CTransactionRef &transaction = block.vtx[i];

                eTransactions new_transaction_record(
                        block.GetBlockHeader().GetHash().GetHex(), // block_hash
                        i, // index
                        transaction->GetTotalSize(), // total_size
                        transaction->vin.size(), // inputs_count
                        transaction->vout.size(), // outputs_count
                        transaction->GetValueOut(), // value_out
                        transaction->nLockTime, // lock_time
                        transaction->nVersion, // version
                        transaction->GetHash().GetHex(), // hash
                        transaction->GetWitnessHash().GetHex(), // witness_hash
                        transaction->IsCoinBase(), // is_coinbase
                        transaction->HasWitness() // has_witness
                );
                enterprise_database->persist(new_transaction_record);

                for (std::size_t n = 0; n < transaction->vout.size(); ++n) {
                    const CTxOut &txout_data = transaction->vout[n];
                    eOutputs new_output_record(
                            block.GetBlockHeader().GetHash().GetHex(), // output_block_hash
                            i, // output_transaction_index
                            transaction->GetHash().GetHex(), // output_transaction_hash
                            n, // output_vector
                            txout_data.nValue, // value
                            CScriptID(txout_data.scriptPubKey).GetHex() // locking_script_id
                    );
                    enterprise_database->persist(new_output_record);

                    std::vector <std::vector<unsigned char>> vSolutions;
                    txnouttype output_script_type;
                    if (!Solver(txout_data.scriptPubKey, output_script_type, vSolutions)) {
                        output_script_type = TX_NONSTANDARD;
                    }

                    eScripts new_locking_script_record(
                            CScriptID(txout_data.scriptPubKey).GetHex(), // id
                            ScriptToAsmStr(txout_data.scriptPubKey), // script
                            GetTxnOutputType(output_script_type) // type
                    );
                    enterprise_database->persist(new_locking_script_record);

                }

                for (std::size_t n = 0; n < transaction->vin.size(); ++n) {
                    const CTxIn &txin_data = transaction->vin[n];
                    eOutputs new_input_record(
                            transaction->IsCoinBase() ? "" :  txin_data.prevout.hash.GetHex(), // output_transaction_hash
                            transaction->IsCoinBase() ? -1 : txin_data.prevout.n, // output_vector
                            block.GetBlockHeader().GetHash().GetHex(), // input_block_hash
                            i, // input_transaction_index
                            transaction->GetHash().GetHex(), // input_transaction_hash
                            n, // input_vector
                            CScriptID(txin_data.scriptSig).GetHex(), // unlocking_script_id
                            txin_data.nSequence == 0xffffffff ? -1 : txin_data.nSequence // sequence
                    );
                    enterprise_database->persist(new_input_record);

                    eScripts new_unlock_script_record(
                            CScriptID(txin_data.scriptSig).GetHex(), // id
                            ScriptToAsmStr(txin_data.scriptSig) // script
                    );
                    enterprise_database->persist(new_unlock_script_record);
                }
            }
        }

        t.commit();
    }
}