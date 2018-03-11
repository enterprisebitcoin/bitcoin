#include <numeric>

#include <core_io.h>
#include <chain.h>
#include <script/standard.h>

#include <enterprise/database.h>
#include <enterprise/enterprise_bitcoin.h>

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>


namespace enterprise_bitcoin {

    void UpsertBlocks(const std::vector <BlockData> &blocks) {
        std::vector <std::string> blocks_values_vector;
        std::vector <std::string> transactions_values_vector;
        std::vector <std::string> txout_values_vector;
        std::vector <std::string> txin_values_vector;
        std::vector <std::string> locking_scripts_values_vector;
        std::vector <std::string> unlocking_scripts_values_vector;

        for (const BlockData &block_data: blocks) {
            CBlockIndex block_index = block_data.first;
            CBlock block = block_data.second;

            std::vector <std::string> block_values = {
                    "'" + block.GetBlockHeader().GetHash().GetHex() + "'", // hash
                    "'" + block_index.hashMerkleRoot.GetHex() + "'", // merkle_root
                    std::to_string(block_index.GetBlockTime()), // time
                    std::to_string(block_index.nHeight), // height
                    std::to_string(block_index.nTx), // transactions_count
                    std::to_string(block_index.nVersion), // version
                    std::to_string(block_index.nStatus), // status
                    std::to_string(block_index.nBits), // bits
                    std::to_string(block_index.nNonce), // nonce
            };

            blocks_values_vector.push_back(
                    std::accumulate(std::begin(block_values), std::end(block_values), std::string(),
                                    [](std::string &accumulation_value, std::string &current_element) {
                                        return accumulation_value.empty()
                                               ? current_element
                                               : accumulation_value + ", " + current_element;
                                    }));

            for (std::size_t i = 0; i < block.vtx.size(); ++i) {
                const CTransactionRef &transaction = block.vtx[i];
                std::vector <std::string> transaction_values = {
                        "'" + block.GetBlockHeader().GetHash().GetHex() + "'", // block_hash
                        std::to_string(i), // index
                        std::to_string(transaction->GetTotalSize()), // total_size
                        std::to_string(transaction->vin.size()), // inputs_count
                        std::to_string(transaction->vout.size()), // outputs_count
                        std::to_string(transaction->GetValueOut()), // value_out
                        std::to_string(transaction->nLockTime), // lock_time
                        std::to_string(transaction->nVersion), // version
                        "'" + transaction->GetHash().GetHex() + "'", // hash
                        "'" + transaction->GetWitnessHash().GetHex() + "'", // witness_hash
                        std::string(transaction->IsCoinBase() ? "TRUE" : "FALSE"), // is_coinbase
                        std::string(transaction->HasWitness() ? "TRUE" : "FALSE"), // has_witness
                };
                transactions_values_vector.push_back(
                        std::accumulate(std::begin(transaction_values), std::end(transaction_values), std::string(),
                                        [](std::string &accumulation_value, std::string &current_element) {
                                            return accumulation_value.empty()
                                                   ? current_element
                                                   : accumulation_value + ", " + current_element;
                                        }));


                for (std::size_t n = 0; n < transaction->vout.size(); ++n) {
                    const CTxOut &txout_data = transaction->vout[n];
                    std::vector <std::string> vout_values = {
                            "'" + block.GetBlockHeader().GetHash().GetHex() + "'", // output_block_hash
                            std::to_string(i), // output_transaction_index
                            "'" + transaction->GetHash().GetHex() + "'", // output_transaction_hash
                            std::to_string(n), // output_vector
                            std::to_string(txout_data.nValue), // value
                            "'" + CScriptID(txout_data.scriptPubKey).GetHex() + "'" // locking_script_id
                    };
                    txout_values_vector.push_back(
                            std::accumulate(std::begin(vout_values), std::end(vout_values), std::string(),
                                            [](std::string &accumulation_value, std::string &current_element) {
                                                return accumulation_value.empty()
                                                       ? current_element
                                                       : accumulation_value + ", " + current_element;
                                            }));

                    std::vector <std::vector<unsigned char>> vSolutions;
                    txnouttype output_script_type;
                    if (!Solver(txout_data.scriptPubKey, output_script_type, vSolutions)) {
                        output_script_type = TX_NONSTANDARD;
                    }

                    std::vector <std::string> script_values = {
                            "'" + CScriptID(txout_data.scriptPubKey).GetHex() + "'", // id
                            "'" + ScriptToAsmStr(txout_data.scriptPubKey) + "'", // script
                            GetTxnOutputType(output_script_type) // type
                    };
                    locking_scripts_values_vector.push_back(
                            std::accumulate(std::begin(script_values), std::end(script_values), std::string(),
                                            [](std::string &accumulation_value, std::string &current_element) {
                                                return accumulation_value.empty()
                                                       ? current_element
                                                       : accumulation_value + ", " + current_element;
                                            }));
                }

                for (std::size_t n = 0; n < transaction->vin.size(); ++n) {
                    const CTxIn &ctxin_data = transaction->vin[n];
                    std::vector <std::string> vin_values = {
                            std::string(transaction->IsCoinBase() ? "NULL" : "'" + ctxin_data.prevout.hash.GetHex() + "'"), // output_transaction_hash
                            std::string(transaction->IsCoinBase() ? "-1" : std::to_string(ctxin_data.prevout.n)), // output_vector
                            "'" + block.GetBlockHeader().GetHash().GetHex() + "'", // input_block_hash
                            std::to_string(i), // input_transaction_index
                            "'" + transaction->GetHash().GetHex() + "'", // input_transaction_hash
                            std::to_string(n), // input_vector
                            "'" + CScriptID(ctxin_data.scriptSig).GetHex() + "'", // unlocking_script_id
                            std::string(ctxin_data.nSequence == 0xffffffff ? "-1" : std::to_string(ctxin_data.nSequence)) // sequence
                    };
                    txin_values_vector.push_back(
                            std::accumulate(std::begin(vin_values), std::end(vin_values), std::string(),
                                            [](std::string &accumulation_value, std::string &current_element) {
                                                return accumulation_value.empty()
                                                       ? current_element
                                                       : accumulation_value + ", " + current_element;
                                            }));

                    std::vector <std::string> script_values = {
                            "'" + CScriptID(ctxin_data.scriptSig).GetHex() + "'", // id
                            ScriptToAsmStr(ctxin_data.scriptSig) // script
                    };
                    unlocking_scripts_values_vector.push_back(
                            std::accumulate(std::begin(script_values), std::end(script_values), std::string(),
                                            [](std::string &accumulation_value, std::string &current_element) {
                                                return accumulation_value.empty()
                                                       ? current_element
                                                       : accumulation_value + ", " + current_element;
                                            }));
                }
            }
        }
        std::string blocks_values = std::accumulate(std::begin(blocks_values_vector), std::end(blocks_values_vector),
                                                    std::string(),
                                                    [](std::string &accumulation_value, std::string &current_element) {
                                                        return accumulation_value.empty()
                                                               ? "(" + current_element + ")"
                                                               : accumulation_value + ", (" + current_element + ")";
                                                    });
        std::string blocks_table_columns = "(hash, merkle_root, time, height, transactions_count, version, status, bits, nonce) ";
        std::string blocks_insert_query = "INSERT INTO bitcoin.\"eBlocks\" " + blocks_table_columns +
                                          "SELECT * FROM (VALUES " + blocks_values +
                                          ") AS temptable " + blocks_table_columns +
                                          "WHERE NOT EXISTS ("
                                                  "SELECT 1 FROM bitcoin.\"eBlocks\" eb "
                                                  "WHERE eb.hash=temptable.hash"
                                                  ");";

        std::string transactions_values = std::accumulate(std::begin(transactions_values_vector),
                                                          std::end(transactions_values_vector), std::string(),
                                                          [](std::string &accumulation_value,
                                                             std::string &current_element) {
                                                              return accumulation_value.empty()
                                                                     ? "(" + current_element + ")"
                                                                     : accumulation_value + ", (" + current_element +
                                                                       ")";
                                                          });
        std::string transactions_table_columns = "(block_hash, index, total_size, inputs_count, outputs_count, value_out, lock_time, version, hash, witness_hash, is_coinbase, has_witness) ";
        std::string transactions_insert_query = "INSERT INTO bitcoin.\"eTransactions\" " + transactions_table_columns +
                                                "SELECT * FROM (VALUES " + transactions_values +
                                                ") AS temptable " + transactions_table_columns +
                                                "WHERE NOT EXISTS ("
                                                        "SELECT 1 FROM bitcoin.\"eTransactions\" eb "
                                                        "WHERE eb.block_hash=temptable.block_hash AND eb.index=temptable.index"
                                                        ");";

        std::string txout_values = std::accumulate(std::begin(txout_values_vector),
                                                   std::end(txout_values_vector),
                                                   std::string(),
                                                   [](std::string &accumulation_value,
                                                      std::string &current_element) {
                                                       return accumulation_value.empty()
                                                              ? "(" + current_element + ")"
                                                              : accumulation_value + ", (" + current_element +
                                                                ")";
                                                   });
        std::string txout_table_columns = "(output_block_hash, output_transaction_index, output_transaction_hash, output_vector, value, locking_script_id) ";
        std::string txout_insert_query = "INSERT INTO bitcoin.\"eOutputs\" " + txout_table_columns +
                                         "SELECT * FROM (VALUES " + txout_values +
                                         ") AS temptable " + txout_table_columns +
                                         "WHERE NOT EXISTS ("
                                                 "SELECT 1 FROM bitcoin.\"eOutputs\" eb "
                                                 "WHERE eb.output_block_hash=temptable.output_block_hash "
                                                 "AND eb.output_transaction_index=temptable.output_transaction_index"
                                                 "AND eb.output_vector=temptable.output_vector"
                                                 ");";


        std::string txin_values = std::accumulate(std::begin(txin_values_vector),
                                                  std::end(txin_values_vector),
                                                  std::string(),
                                                  [](std::string &accumulation_value,
                                                     std::string &current_element) {
                                                      return accumulation_value.empty()
                                                             ? "(" + current_element + ")"
                                                             : accumulation_value + ", (" + current_element +
                                                               ")";
                                                  });
        std::string txin_table_columns = "(output_transaction_hash, output_vector, input_block_hash, input_transaction_index, input_transaction_hash, "
                                          "input_vector, unlocking_script_id, sequence) ";
        std::string txin_insert_query = "INSERT INTO bitcoin.\"eOutputs\" " + txin_table_columns +
                                        "SELECT * FROM (VALUES " + txin_values +
                                        ") AS temptable " + txin_table_columns +
                                        "WHERE NOT EXISTS ("
                                                "SELECT 1 FROM bitcoin.\"eOutputs\" eb "
                                                "WHERE eb.output_transaction_hash=temptable.output_transaction_hash "
                                                "AND eb.output_vector=temptable.output_vector"
                                                ");";

        std::string txin_update_query = "UPDATE bitcoin.\"eOutputs\" "
                                        "SET "
                                           "input_block_hash=temptable.input_block_hash, "
                                           "input_transaction_index=temptable.input_transaction_index, "
                                           "input_transaction_hash=temptable.input_transaction_hash, "
                                           "input_vector=temptable.input_vector, "
                                           "unlocking_script_id=temptable.unlocking_script_id, "
                                           "sequence=temptable.sequence "
                                           "FROM ("
                                                "SELECT * FROM (VALUES " + txin_values + ") v " + txin_table_columns +
                                           ") AS temptable "
                                           "WHERE bitcoin.\"eOutputs\".output_transaction_hash=temptable.output_transaction_hash "
                                           "AND bitcoin.\"eOutputs\".output_vector=temptable.output_vector"
                                           ";";

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            odb::transaction t(enterprise_database->begin(), false);
            odb::transaction::current(t);
            enterprise_database->execute(blocks_insert_query);
            enterprise_database->execute(transactions_insert_query);
            enterprise_database->execute(txout_insert_query);
            enterprise_database->execute(txin_insert_query);
            enterprise_database->execute(txin_update_query);
            t.commit();
        }
    }
}