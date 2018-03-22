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

    void InsertBlocks(const std::vector <eBlocks> &records) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        odb::transaction t(enterprise_database->begin(), false);
        odb::transaction::current(t);

        for (eBlocks record: records) {
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

    void ProcessBlocks(const std::vector <BlockData> &blocks) {

        std::vector <eBlocks> block_records;
        std::vector <eTransactions> transaction_records;
        std::vector <eOutputs> output_records;
        std::vector <eScripts> script_records;


        for (const BlockData &block_data: blocks) {
            CBlockIndex block_index = block_data.first;
            CBlock block = block_data.second;

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
                const CTransactionRef &transaction = block.vtx[i];

                eTransactions transaction_record(
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
                transaction_records.push_back(transaction_record);

                for (std::size_t n = 0; n < transaction->vout.size(); ++n) {
                    const CTxOut &txout_data = transaction->vout[n];

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
                        EnterpriseDestination enterprise_destination = EnterpriseDestinationEncoder(destination);
                        eAddresses address_record(
                                CScriptID(txout_data.scriptPubKey).GetHex(),
                                enterprise_destination.address,
                                enterprise_destination.source_type,
                                enterprise_destination.hex,
                                enterprise_destination.version,
                                enterprise_destination.length,
                                enterprise_destination.program
                        );

                    }
                }

                for (std::size_t n = 0; n < transaction->vin.size(); ++n) {
                    const CTxIn &txin_data = transaction->vin[n];
                    eOutputs input_record(
                            transaction->IsCoinBase() ? "" :  txin_data.prevout.hash.GetHex(), // output_transaction_hash
                            transaction->IsCoinBase() ? -1 : txin_data.prevout.n, // output_vector
                            block.GetBlockHeader().GetHash().GetHex(), // input_block_hash
                            i, // input_transaction_index
                            transaction->GetHash().GetHex(), // input_transaction_hash
                            n, // input_vector
                            CScriptID(txin_data.scriptSig).GetHex(), // unlocking_script_id
                            txin_data.nSequence == 0xffffffff ? -1 : txin_data.nSequence // sequence
                    );
                    output_records.push_back(input_record);

                    eScripts unlock_script_record(
                            CScriptID(txin_data.scriptSig).GetHex(), // id
                            ScriptToAsmStr(txin_data.scriptSig) // script
                    );
                    script_records.push_back(unlock_script_record);
                }
            }
        }

        std::thread t1(enterprise_bitcoin::InsertBlocks, block_records);
        std::thread t2(enterprise_bitcoin::InsertTransactions, transaction_records);
        std::thread t3(enterprise_bitcoin::InsertOutputs, output_records);
        std::thread t4(enterprise_bitcoin::InsertScripts, script_records);
        t1.join();
        t2.join();
        t3.join();
        t4.join();
    }
}