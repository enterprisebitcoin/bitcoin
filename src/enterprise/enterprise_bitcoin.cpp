#include <chain.h>

#include <enterprise/database.h>
#include <enterprise/enterprise_bitcoin.h>

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>



namespace enterprise_bitcoin {

    void UpsertBlocks(const std::vector<BlockData> &blocks)
    {
        std::string block_hash_data = "";
        std::string block_time_data = "";
        std::string block_height_data = "";
        for (auto const& block_data: blocks)
        {
            CBlockIndex block_index = block_data.first;
            CBlock block = block_data.second;
            block_hash_data += "'" + block.GetBlockHeader().GetHash().GetHex() + "', ";
            block_time_data += "'" + std::to_string(block_index.GetBlockTime()) + "', ";
            block_height_data += "'" + std::to_string(block_index.nHeight) + "', ";
        }
        block_hash_data = block_hash_data.substr(0, block_hash_data.size() - 2);
        block_time_data = block_time_data.substr(0, block_time_data.size() - 2);
        block_height_data = block_height_data.substr(0, block_height_data.size() - 2);

        std::string insert_query = "INSERT INTO bitcoin.\"eBlocks\" (hash, time, height) "
                                           "SELECT * FROM ( "
                                           "        SELECT UNNEST(ARRAY[" + block_hash_data + "]) AS hash, "
                                           "        UNNEST(ARRAY[" + block_time_data + "])::BIGINT AS time, "
                                           "        UNNEST(ARRAY[" + block_height_data + "])::BIGINT AS height"
                                           ") AS temptable "
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