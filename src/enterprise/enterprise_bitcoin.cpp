#include <core_io.h>
#include <chain.h>
#include <consensus/validation.h>
#include <script/standard.h>
#include <validation.h>

#include <enterprise/database.h>
#include <enterprise/enterprise_bitcoin.h>
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


namespace enterprise_bitcoin {

    void InsertAddresses(const std::vector <eAddresses> &records) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (eAddresses record: records) {
            enterprise_database->persist(record);
        }
        t.commit();
    }

    void InsertBlocks(const std::vector <eBlocks> &records) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (eBlocks record: records) {
            enterprise_database->persist(record);
        }
        t.commit();
    }

    void InsertInputs(const std::vector <eInputs> &records) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (eInputs record: records) {
            enterprise_database->persist(record);
        }
        t.commit();
    }

    void InsertOutputs(const std::vector <eOutputs> &records) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (eOutputs record: records) {
            enterprise_database->persist(record);
        }
        t.commit();
    }

    void InsertScripts(const std::vector <eScripts> &records) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (eScripts record: records) {
            enterprise_database->persist(record);
        }
        t.commit();
    }

    void InsertTransactions(const std::vector <eTransactions> &records) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (eTransactions record: records) {
            enterprise_database->persist(record);
        }
        t.commit();
    }

    void ProcessBlocks(const std::vector <BlockData> &blocks) {

        std::vector <eAddresses> address_records;
        std::vector <eBlocks> block_records;
        std::vector <eInputs> input_records;
        std::vector <eOutputs> output_records;
        std::vector <eScripts> script_records;
        std::vector <eTransactions> transaction_records;


        for (const BlockData &block_data: blocks) {
            CBlockIndex block_index = block_data.first;
            CBlock block = block_data.second;

            bool block_already_processed = false;
            std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
            {
                typedef odb::query <eBlocks> query;
                odb::transaction t(enterprise_database->begin());
                std::auto_ptr <eBlocks> r(enterprise_database->query_one<eBlocks>(query::hash == block.GetBlockHeader().GetHash().GetHex()));
                if (r.get() != 0) {
                    block_already_processed = true;
                }
                t.commit();
            }
            if (block_already_processed) continue;

            eBlocks block_record(
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
            block_records.push_back(block_record);

            for (std::size_t i = 0; i < block.vtx.size(); ++i) {
                CAmount total_output_value = 0;
                CAmount total_input_value = 0;

                const CTransactionRef &transaction = block.vtx[i];

                // Outputs
                for (std::size_t n = 0; n < transaction->vout.size(); ++n) {
                    const CTxOut &txout_data = transaction->vout[n];
                    total_output_value += txout_data.nValue;

                    txnouttype output_script_type;
                    std::vector<CTxDestination> destinations;
                    int nRequired;
                    ExtractDestinations(txout_data.scriptPubKey, output_script_type, destinations, nRequired);

                    eOutputs output_record(
                            block.GetBlockHeader().GetHash().GetHex(), // output_block_hash
                            i, // output_transaction_index
                            transaction->GetHash().GetHex(), // output_transaction_hash
                            n, // output_vector
                            txout_data.nValue, // value
                            CScriptID(txout_data.scriptPubKey).GetHex(), // locking_script_id
                            nRequired // required_signatures
                    );
                    output_records.push_back(output_record);


                    eScripts locking_script_record(
                            CScriptID(txout_data.scriptPubKey).GetHex(), // id
                            ScriptToAsmStr(txout_data.scriptPubKey), // script
                            GetTxnOutputType(output_script_type) // type
                    );
                    script_records.push_back(locking_script_record);

                    for (CTxDestination &destination: destinations) {
                        EnterpriseDestination enterprise_destination = boost::apply_visitor(EnterpriseDestinationEncoder(Params()), destination);
                        eAddresses address_record(
                                CScriptID(txout_data.scriptPubKey).GetHex(),
                                enterprise_destination.address,
                                enterprise_destination.source_type,
                                enterprise_destination.hex,
                                enterprise_destination.version,
                                enterprise_destination.length,
                                enterprise_destination.program
                        );
                        address_records.push_back(address_record);
                    }
                }

                // Inputs
                for (std::size_t n = 0; n < transaction->vin.size(); ++n) {
                    const CTxIn &txin_data = transaction->vin[n];
                    eInputs input_record(
                            transaction->IsCoinBase() ? "" :  txin_data.prevout.hash.GetHex(), // output_transaction_hash
                            transaction->IsCoinBase() ? -1 : txin_data.prevout.n, // output_vector
                            block.GetBlockHeader().GetHash().GetHex(), // input_block_hash
                            i, // input_transaction_index
                            transaction->GetHash().GetHex(), // input_transaction_hash
                            n, // input_vector
                            CScriptID(txin_data.scriptSig).GetHex(), // unlocking_script_id
                            txin_data.nSequence == 0xffffffff ? -1 : txin_data.nSequence // sequence
                    );
                    input_records.push_back(input_record);

                    eScripts unlock_script_record(
                            CScriptID(txin_data.scriptSig).GetHex(), // id
                            ScriptToAsmStr(txin_data.scriptSig) // script
                    );
                    script_records.push_back(unlock_script_record);

                    if (!transaction->IsCoinBase()) {
                        CTransactionRef output_transaction;
                        uint256 hash_block;
                        CBlockIndex* blockindex = nullptr;
                        GetTransaction(txin_data.prevout.hash, output_transaction, Params().GetConsensus(), hash_block, true, blockindex);

                        const CTxOut &output_txout_data = output_transaction->vout[txin_data.prevout.n];
                        total_input_value += output_txout_data.nValue;

                        eOutputs output_record(
                                hash_block.GetHex(), // output_block_hash
                                -1, // output_transaction_index
                                txin_data.prevout.hash.GetHex(), // output_transaction_hash
                                txin_data.prevout.n, // output_vector
                                output_txout_data.nValue, // value
                                "", // locking_script_id
                                -1 // required_signatures
                        );
                        output_records.push_back(output_record);
                    }
                }

                CAmount fees = transaction->IsCoinBase() ? 0 : total_input_value - total_output_value;
                unsigned int weight = GetTransactionWeight(*transaction);
                unsigned int vsize = (weight + WITNESS_SCALE_FACTOR - 1) / WITNESS_SCALE_FACTOR;
                bool is_segwit_out_spend = transaction->IsCoinBase() ? false : !(transaction->GetHash() == transaction->GetWitnessHash());

                eTransactions transaction_record(
                        block.GetBlockHeader().GetHash().GetHex(), // block_hash
                        i, // index
                        transaction->GetTotalSize(), // size
                        vsize, // vsize
                        weight, // weight
                        transaction->vin.size(), // inputs_count
                        transaction->vout.size(), // outputs_count
                        transaction->nLockTime, // lock_time
                        transaction->nVersion, // version
                        transaction->GetHash().GetHex(), // hash
                        transaction->GetWitnessHash().GetHex(), // witness_hash
                        transaction->IsCoinBase(), // is_coinbase
                        transaction->HasWitness(), // has_witness
                        is_segwit_out_spend, // is_segwit_out_spend
                        total_output_value, // total_output_value
                        total_input_value, // total_input_value
                        fees, // fees
                        static_cast<float>(fees)/transaction->GetTotalSize(), // fee_per_byte
                        static_cast<float>(fees)/vsize, // fee_per_vbyte
                        static_cast<float>(fees)/weight // fee_per_weight_unit
                );
                transaction_records.push_back(transaction_record);
            }
        }

        if (!block_records.size()) return;

//        std::thread t1(enterprise_bitcoin::InsertAddresses, address_records);
//        std::thread t2(enterprise_bitcoin::InsertScripts, script_records);
//        std::thread t3(enterprise_bitcoin::InsertInputs, input_records);
//        std::thread t4(enterprise_bitcoin::InsertOutputs, output_records);
//        t1.join();
//        t2.join();
//        t3.join();
//        t4.join();

        int thread_count = 4;
        std::vector <std::vector <eTransactions>> work(thread_count);
        std::vector <std::thread> threads;
        for (int i = 0; i < transaction_records.size(); ++i) {
            int thread_index = i % thread_count;
            work[thread_index].push_back(transaction_records[i]);
        }

        for (int i = 0; i < thread_count; ++i) {
            std::thread work_thread(enterprise_bitcoin::InsertTransactions, work[i]);
            threads.push_back(std::move(work_thread));
        }

        for (std::thread &t : threads) {
            if (t.joinable()) {
                t.join();
            }
        }
        threads.clear();

        std::thread t6(enterprise_bitcoin::InsertBlocks, block_records);
        t6.join();


//        std::auto_ptr <odb::database> db(create_enterprise_database());
//        odb::transaction t (db->begin ());
//        db->execute ("TRUNCATE bitcoin.\"eInputs\"");
//        db->execute ("TRUNCATE bitcoin.\"eOutputs\"");
//        db->execute ("TRUNCATE bitcoin.\"eScripts\"");
//        db->execute ("TRUNCATE bitcoin.\"eAddresses\"");
//        db->execute ("TRUNCATE bitcoin.\"eTransactions\"");
//        t.commit ();
    }

}