#include <numeric>

#include <chain.h>

#include <enterprise/database.h>
#include <enterprise/enterprise_bitcoin.h>

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>



namespace enterprise_bitcoin {

    void UpsertBlocks(const std::vector<BlockData> &blocks)
    {
        std::vector<std::string> blocks_values_vector;
        for (const std::vector<BlockData> &block_data: blocks)
        {
            std::vector<std::string> block_values;
            CBlockIndex block_index = block_data.first;
            CBlock block = block_data.second;

            block_values.push_back("'" + block.GetBlockHeader().GetHash().GetHex() + "'"); // hash
            block_values.push_back("'" + block_index.hashMerkleRoot.GetHex() + "'"); // merkle_root
            block_values.push_back(std::to_string(block_index.GetBlockTime())); // time
            block_values.push_back(std::to_string(block_index.nHeight)); // height
            block_values.push_back(std::to_string(block_index.nTx)); // transactions_count
            block_values.push_back(std::to_string(block_index.nVersion)); // version
            block_values.push_back(std::to_string(block_index.nStatus)); // status
            block_values.push_back(std::to_string(block_index.nBits)); // bits
            block_values.push_back(std::to_string(block_index.nNonce)); // nonce

            blocks_values_vector.push_back(std::accumulate(std::begin(block_values), std::end(block_values), std::string(),
                            [](std::string &accumulation_value, std::string &current_element)
                            {
                                return accumulation_value.empty()
                                       ?  current_element
                                       : accumulation_value + ", " + current_element;
                            }));

            for (const CTransactionRef &transaction_data: block.vtx) {

            }
        }
        std::string values = std::accumulate(std::begin(blocks_values_vector), std::end(blocks_values_vector), std::string(),
                                                [](std::string &accumulation_value, std::string &current_element)
                                                {
                                                    return accumulation_value.empty()
                                                           ? "(" + current_element + ")"
                                                           : accumulation_value + ", (" + current_element + ")";
                                                });
        std::string table_columns = "(hash, merkle_root, time, height, transactions_count, version, status, bits, nonce) ";
        std::string insert_query = "INSERT INTO bitcoin.\"eBlocks\" " + table_columns +
                                           "SELECT * FROM (VALUES " + values +
                                           ") AS temptable " + table_columns +
                                           "WHERE NOT EXISTS ("
                                           "SELECT 1 FROM bitcoin.\"eBlocks\" eb "
                                           "WHERE eb.hash=temptable.hash"
                                           ");";

//        std::string update_query = "UPDATE wallet.\"eAddresses\" SET name=temptable.name, purpose=temptable.purpose "
//                                           "FROM ( "
//                                           "        SELECT UNNEST(ARRAY[" + addresses_data + "]) AS p2pkh_address, "
//                                           "        UNNEST(ARRAY[" + name_data + "]) AS name, "
//                                           "        UNNEST(ARRAY[" + purpose_data + "]) AS purpose,"
//                                           "        UNNEST(ARRAY[" + wallet_id_data + "])::UUID AS wallet_id"
//                                           ") AS temptable "
//                                           "WHERE wallet.\"eAddresses\".p2pkh_address=temptable.p2pkh_address "
//                                           "AND wallet.\"eAddresses\".wallet_id=temptable.wallet_id::UUID"
//                                           ";";

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            odb::transaction t(enterprise_database->begin(), false);
            odb::transaction::current(t);
            enterprise_database->execute(insert_query);
//            enterprise_database->execute(update_query);
            t.commit();
        }
    }
}