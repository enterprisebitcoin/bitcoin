#include <base58.h>
#include <util.h>
#include <script/standard.h>
#include <utilstrencodings.h>
#include <wallet/wallet.h>
#include <wallet/rpcdump.cpp>

#include "wallet/enterprise/database.h"
#include "wallet/enterprise/models/addresses.h"
#include "wallet/enterprise/models/blocks.h"
#include "wallet/enterprise/models/output_entries.h"
#include "wallet/enterprise/models/transactions.h"
#include "wallet/enterprise/models/wallet_transactions.h"
#include "wallet/enterprise/models/wallets.h"
#include "wallet/enterprise/views/watch_only_addresses.h"

#include "wallet/enterprise/models/support/addresses-odb.hxx"
#include "wallet/enterprise/models/support/blocks-odb.hxx"
#include "wallet/enterprise/models/support/output_entries-odb.hxx"
#include "wallet/enterprise/models/support/transactions-odb.hxx"
#include "wallet/enterprise/models/support/wallet_transactions-odb.hxx"
#include "wallet/enterprise/models/support/wallets-odb.hxx"
#include "wallet/enterprise/views/support/watch_only_addresses-odb.hxx"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>

#include <wallet/enterprise/enterprise_wallet.h>


namespace enterprise_wallet {


    boost::uuids::uuid GetWalletID() {
        CWalletDBWrapper& dbw = vpwallets[0]->GetDBHandle();
        CWalletDB wallet_db(dbw);
        std::string wallet_id = wallet_db.ReadID();
        return boost::lexical_cast<boost::uuids::uuid>(wallet_id);
    }

    void ImportWatchOnlyAddresses() {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::result <watch_only_addresses> query_result;
            std::string wallet_id = boost::lexical_cast<std::string>(GetWalletID());
            odb::transaction t(enterprise_database->begin());
            LogPrintf("Querying for watch only addresses\n");
            query_result r(enterprise_database->query<watch_only_addresses>(
                    "SELECT wallet_id, address, source " \
                     "FROM wallet.watch_only_addresses " \
                     "WHERE watch_only_address_id IS NULL AND wallet_id='" + wallet_id + "'"
            ));
            for (query_result::iterator i (r.begin ()); i != r.end (); ++i)
            {
                watch_only_addresses watch_only_address (*i);
                LogPrintf("Importing watch only address %s %s \n", watch_only_address.address, watch_only_address.source);
                std::string strLabel = "Source: " + watch_only_address.source;
                CTxDestination dest = DecodeDestination(watch_only_address.address);
                if (IsValidDestination(dest)) {
                    CScript script = GetScriptForDestination(dest);

                    CWalletRef pwallet = ::vpwallets[0];
                    LOCK2(cs_main, pwallet->cs_wallet);
                    pwallet->MarkDirty();

                    CTxDestination destination;
                    if (ExtractDestination(script, destination)) {
                        pwallet->AddWatchOnly(script, 0);
                        pwallet->SetAddressBook(destination, strLabel, "receive");
                    }
                }
            }
            t.commit();
        }
    }


    void UpsertWallet() {
        CWalletDBWrapper& dbw = vpwallets[0]->GetDBHandle();
        std::string name = dbw.GetName();
        boost::uuids::uuid wallet_id = GetWalletID();

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eWallets> query;
            odb::transaction t(enterprise_database->begin());
            std::auto_ptr <eWallets> r(enterprise_database->query_one<eWallets>(query::wallet_id == wallet_id));
            if (r.get() != 0) {
                r->name = name;
                enterprise_database->update(*r);
            } else {
                eWallets new_r(wallet_id, name, "New wallet");
                enterprise_database->persist(new_r);
            }
            t.commit();
        }
    }

    void TopUpAddressPool() {
        boost::uuids::uuid wallet_id = GetWalletID();
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
            {
                typedef odb::query <address_stats> query;
                odb::transaction t (enterprise_database->begin ());
                address_stats as (enterprise_database->query_value<address_stats> (query::is_used == false && query::wallet_id == wallet_id));
                LogPrintf("Unused address count %d \n", as.count);
                t.commit ();
                if (as.count < 100) {
                    int new_addresses = 100 - as.count;
                    LogPrintf("New addresses needed: %d \n", new_addresses);
                    for (int i=0; i<new_addresses; ++i) {
                        CPubKey newKey;
                        vpwallets[0]->GetKeyFromPool(newKey);
                        CKeyID keyID = newKey.GetID();
                        std::string strAccount;
                        vpwallets[0]->SetAddressBook(keyID, strAccount, "receive");
                        LogPrintf("New address created: %s \n", EncodeDestination(keyID));
                    }
                }
            }
    };

    void UpsertAddress(const std::string &p2pkh_address,
                       const std::string &sw_bech32_address,
                       const std::string &sw_p2sh_address,
                       const std::string &name,
                       const std::string &purpose) {
        boost::uuids::uuid wallet_id = GetWalletID();

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eAddresses> query;
            odb::transaction t(enterprise_database->begin(), false);
            odb::transaction::current (t);
            std::auto_ptr <eAddresses> ea(enterprise_database->query_one<eAddresses>(query::p2pkh_address == p2pkh_address && query::wallet_id == wallet_id));
            if (ea.get() != 0) {
                ea->name = name;
                ea->purpose = purpose;
                enterprise_database->update(*ea);
            } else {
                eAddresses new_ea(p2pkh_address, sw_bech32_address, sw_p2sh_address,
                                  name, purpose, GetTimeMillis(), false, wallet_id);
                enterprise_database->persist(new_ea);
            }
            t.commit();
        }
    }

    void UpsertAddressBook(const std::map<CTxDestination, CAddressBookData>& address_book)
    {
        std::string wallet_id = boost::lexical_cast<std::string>(GetWalletID());

        std::string addresses_data = "";
        std::string name_data = "";
        std::string purpose_data = "";
        std::string wallet_id_data = "";
        for (const std::pair<CTxDestination, CAddressBookData>& address : address_book)
        {
            addresses_data += "'" + EncodeDestination(address.first) + "', ";
            name_data += "'" + address.second.name + "', ";
            purpose_data += "'" + address.second.purpose + "', ";
            wallet_id_data += "'" + wallet_id + "', ";
        }
        addresses_data = addresses_data.substr(0, addresses_data.size() - 2);
        name_data = name_data.substr(0, name_data.size() - 2);
        purpose_data = purpose_data.substr(0, purpose_data.size() - 2);
        wallet_id_data = wallet_id_data.substr(0, wallet_id_data.size() - 2);

        std::string insert_query = "INSERT INTO wallet.\"eAddresses\" (p2pkh_address, name, purpose, wallet_id) "
                                    "SELECT * FROM ( "
                                    "        SELECT UNNEST(ARRAY[" + addresses_data + "]) AS p2pkh_address, "
                                    "        UNNEST(ARRAY[" + name_data + "]) AS name, "
                                    "        UNNEST(ARRAY[" + purpose_data + "]) AS purpose,"
                                    "        UNNEST(ARRAY[" + wallet_id_data + "])::UUID AS wallet_id"
                                    ") AS temptable "
                                    "WHERE NOT EXISTS ("
                                    "SELECT 1 FROM wallet.\"eAddresses\" tt "
                                    "WHERE tt.p2pkh_address=temptable.p2pkh_address "
                                      "AND tt.wallet_id=temptable.wallet_id::UUID"
                                    ");";

        std::string update_query = "UPDATE wallet.\"eAddresses\" SET name=temptable.name, purpose=temptable.purpose "
                                           "FROM ( "
                                           "        SELECT UNNEST(ARRAY[" + addresses_data + "]) AS p2pkh_address, "
                                           "        UNNEST(ARRAY[" + name_data + "]) AS name, "
                                           "        UNNEST(ARRAY[" + purpose_data + "]) AS purpose,"
                                           "        UNNEST(ARRAY[" + wallet_id_data + "])::UUID AS wallet_id"
                                           ") AS temptable "
                                           "WHERE wallet.\"eAddresses\".p2pkh_address=temptable.p2pkh_address "
                                           "AND wallet.\"eAddresses\".wallet_id=temptable.wallet_id::UUID"
                                           ";";

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            odb::transaction t(enterprise_database->begin(), false);
            odb::transaction::current(t);
            enterprise_database->execute(insert_query);
            enterprise_database->execute(update_query);
            t.commit();
        }
    }

    unsigned int UpsertBlock(const uint256 &binary_hash) {

        std::string hash_str = binary_hash.ToString();

        if (mapBlockIndex.count(binary_hash) == 0) {
            LogPrintf("Block not found %s \n", hash_str);
            return 0;
        }

        CBlockIndex* blockindex = mapBlockIndex[binary_hash];

        int64_t time = blockindex->GetBlockTime();
        int height = blockindex->nHeight;

        LogPrintf("Upsert block %s \n", hash_str);

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eBlocks> query;
            odb::transaction t(enterprise_database->begin(), false);
            odb::transaction::current (t);
            std::auto_ptr <eBlocks> eb(enterprise_database->query_one<eBlocks>(query::hash == hash_str));
            if (eb.get() != 0) {
                eb->time = time;
                eb->height = height;
                enterprise_database->update(*eb);
            } else {
                eBlocks new_eb(hash_str, time, height);
                enterprise_database->persist(new_eb);
            }
            t.commit();

            odb::transaction id_t(enterprise_database->begin(), false);
            odb::transaction::current (id_t);
            std::auto_ptr <eBlocks> eb_id(enterprise_database->query_one<eBlocks>(query::hash == hash_str));
            unsigned int block_id = eb_id->id;
            id_t.commit();
            return block_id;
        }
    }

    /**
     * Utility function for upserting the block that tx_hash belongs to and returning the block ID
     * @param tx_hash
     * @return eBlocks ID
     */
    unsigned int GetBlockID(uint256& tx_hash) {
        unsigned int block_id = 0;
        CTransactionRef tx;
        uint256 block_hash;
        bool found_transaction = GetTransaction(tx_hash, tx, Params().GetConsensus(), block_hash, true);

        if (found_transaction) {
            block_id = UpsertBlock(block_hash);
        } else {
            if (fTxIndex) {
                LogPrintf("No such mempool or blockchain transaction \n");
            } else {
                LogPrintf("No such mempool transaction. Use -txindex to enable blockchain transaction queries \n");
            };
        }
        return block_id;
    }

    void UpsertOutputs(const std::vector<CTxOut> vout, const unsigned int output_etransaction_id) {
        for(unsigned int index = 0; index < vout.size(); index++) {
            const CTxOut &output = vout[index];
            isminetype is_output_mine = vpwallets[0]->IsMine(output);
            CTxDestination output_destination;
            ExtractDestination(output.scriptPubKey, output_destination);
            std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
            {
                typedef odb::query <eOutputEntries> output_query;
                odb::transaction t(enterprise_database->begin());
                std::auto_ptr <eOutputEntries> eout(
                        enterprise_database->query_one<eOutputEntries>(
                                output_query::output_etransaction_id == output_etransaction_id
                                && output_query::output_vector == index));
                if (eout.get() != 0) {
                    eout->output_etransaction_id = output_etransaction_id;
                    eout->output_vector = index;
                    eout->is_output_mine = is_output_mine;
                    eout->n_value = output.nValue;
                    eout->destination = EncodeDestination(output_destination);
                    eout->script_pub_key = ScriptToAsmStr(output.scriptPubKey);

                    enterprise_database->update(*eout);
                } else {
                    eOutputEntries new_eout(
                            output_etransaction_id,
                            index,
                            is_output_mine,
                            output.nValue,
                            EncodeDestination(output_destination),
                            ScriptToAsmStr(output.scriptPubKey),

                            0,
                            0,
                            "",
                            "",
                            0
                    );
                    enterprise_database->persist(new_eout);
                }
                t.commit();
            }
        }
    }

    void UpsertInputs(const std::vector<CTxIn> vin, const unsigned int input_etransaction_id) {
        for(unsigned int index = 0; index < vin.size(); index++) {
            const CTxIn &input = vin[index];
            isminetype is_output_mine = vpwallets[0]->IsMine(input);

            CTransactionRef output_transaction;
            uint256 output_block_hash;
            bool found_output_transaction = GetTransaction(input.prevout.hash, output_transaction,
                                                           Params().GetConsensus(), output_block_hash, true);
            if (!found_output_transaction) {
                LogPrintf("Did not find transaction %s \n", input.prevout.hash.GetHex());
                return;
            }
            const CTxOut &output = output_transaction->vout[input.prevout.n];
            unsigned int output_etransaction_id = UpsertTransaction(output_transaction, false);
            CTxDestination output_destination;
            ExtractDestination(output.scriptPubKey, output_destination);

            std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
            {
                typedef odb::query <eOutputEntries> output_query;
                odb::transaction t(enterprise_database->begin());
                std::auto_ptr <eOutputEntries> eout(
                        enterprise_database->query_one<eOutputEntries>(
                                (output_query::input_etransaction_id == input_etransaction_id
                                && output_query::input_vector == index)
                            || (output_query::output_etransaction_id == output_etransaction_id
                                && output_query::output_vector == input.prevout.n)));
                if (eout.get() != 0) {
                    eout->output_etransaction_id = output_etransaction_id;
                    eout->output_vector = input.prevout.n;
                    eout->is_output_mine = is_output_mine;
                    eout->n_value = output.nValue;
                    eout->destination = EncodeDestination(output_destination);
                    eout->script_pub_key = ScriptToAsmStr(output.scriptPubKey);

                    eout->input_etransaction_id = input_etransaction_id;
                    eout->input_vector = index;
                    eout->script_sig = ScriptToAsmStr(input.scriptSig, true);
                    eout->script_witness = input.scriptWitness.ToString();
                    eout->n_sequence = input.nSequence;

                    enterprise_database->update(*eout);
                } else {
                    eOutputEntries new_eout(
                            output_etransaction_id,
                            input.prevout.n,
                            is_output_mine,
                            output.nValue,
                            EncodeDestination(output_destination),
                            ScriptToAsmStr(output.scriptPubKey),

                            input_etransaction_id,
                            index,
                            ScriptToAsmStr(input.scriptSig, true),
                            input.scriptWitness.ToString(),
                            input.nSequence
                    );
                    enterprise_database->persist(new_eout);
                }
                t.commit();
            }
        }
    }

    unsigned int UpsertTransaction(const CTransactionRef tx, const bool upsert_inputs) {

        uint256 binary_hash = tx->GetHash();
        std::string hash = binary_hash.GetHex();

        uint256 binary_witness_hash = tx->GetWitnessHash();
        std::string witness_hash = binary_witness_hash.GetHex();

        unsigned int block_id = GetBlockID(binary_hash);

        unsigned int etransaction_id;

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eTransactions> query;

            LOCK(cs_main);

            odb::transaction t(enterprise_database->begin());
            // Select the transaction by its txid, insert if it's not found, update if it is
            std::auto_ptr <eTransactions> etx(enterprise_database->query_one<eTransactions>(query::hash == hash));
            if (etx.get() != 0) {
                etx->block_id = block_id;
                etx->total_size = tx->GetTotalSize();
                etx->inputs_count = tx->vin.size();
                etx->outputs_count = tx->vout.size();
                etx->value_out = tx->GetValueOut();
                etx->n_lock_time = tx->nLockTime;
                etx->n_version = tx->nVersion;
                etx->hash = hash;
                etx->witness_hash = witness_hash;
                etx->is_coinbase = tx->IsCoinBase();
                etx->has_witness = tx->HasWitness();

                enterprise_database->update(*etx);
                etransaction_id = etx->id;
            } else {
                eTransactions new_etx(block_id,
                                      tx->GetTotalSize(),
                                      tx->vin.size(),
                                      tx->vout.size(),
                                      tx->GetValueOut(),
                                      tx->nLockTime,
                                      tx->nVersion,
                                      hash,
                                      witness_hash,
                                      tx->IsCoinBase(),
                                      tx->HasWitness());
                etransaction_id = enterprise_database->persist(new_etx);
            }
            t.commit();
        }

        if (upsert_inputs) {
            UpsertInputs(tx->vin, etransaction_id);
        }

        UpsertOutputs(tx->vout, etransaction_id);

        return etransaction_id;
    }

    void UpsertWalletTransaction(const CWalletTx& wallet_transaction) {
        unsigned int etransaction_id = UpsertTransaction(wallet_transaction.tx, true);
        boost::uuids::uuid wallet_id = GetWalletID();

        unsigned int ewallet_transaction_id;

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eWalletTransactions> query;
            typedef odb::query <eOutputEntries> output_query;

            LOCK(cs_main);

            odb::transaction t(enterprise_database->begin());
            // Select the transaction by its txid, insert if it's not found, update if it is
            std::auto_ptr <eWalletTransactions> etx(enterprise_database->query_one<eWalletTransactions>(query::etransaction_id == etransaction_id
                                                                                            && query::wallet_id == wallet_id));
            if (etx.get() != 0) {
                etx->is_trusted = wallet_transaction.IsTrusted();
                etx->n_time_smart = wallet_transaction.nTimeSmart;
                etx->n_time_received = wallet_transaction.nTimeReceived;
                etx->debit = wallet_transaction.GetDebit(ISMINE_ALL);
                etx->credit = wallet_transaction.GetCredit(ISMINE_ALL);
                enterprise_database->update(*etx);
                ewallet_transaction_id = etx->id;
            } else {
                eWalletTransactions new_etx(etransaction_id,
                                      wallet_transaction.IsTrusted(),
                                      wallet_transaction.nTimeSmart,
                                      wallet_transaction.nTimeReceived,
                                      wallet_transaction.GetDebit(ISMINE_ALL),
                                      wallet_transaction.GetCredit(ISMINE_ALL),
                                      wallet_id);
                ewallet_transaction_id = enterprise_database->persist(new_etx);
            }
            t.commit();
        }
    }

    void DeleteTx(uint256 &hash) {
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eTransactions> query;

            odb::transaction t(enterprise_database->begin());
            std::auto_ptr <eTransactions> etx(enterprise_database->query_one<eTransactions>(query::hash == hash.GetHex()));
            if (etx.get() != 0)
                enterprise_database->erase(*etx);
            t.commit();
        }
    }
}
