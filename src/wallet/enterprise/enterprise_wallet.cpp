#include <base58.h>
#include <util.h>
#include <script/standard.h>
#include <wallet/wallet.h>
#include <wallet/rpcdump.cpp>

#include "wallet/enterprise/database.h"
#include "wallet/enterprise/models/addresses.h"
#include "wallet/enterprise/models/blocks.h"
#include "wallet/enterprise/models/output_entries.h"
#include "wallet/enterprise/models/transactions.h"
#include "wallet/enterprise/models/wallets.h"
#include "wallet/enterprise/views/watch_only_addresses.h"

#include "wallet/enterprise/models/support/addresses-odb.hxx"
#include "wallet/enterprise/models/support/blocks-odb.hxx"
#include "wallet/enterprise/models/support/output_entries-odb.hxx"
#include "wallet/enterprise/models/support/transactions-odb.hxx"
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
        bool found_transaction = GetTransaction(hash, tx, Params().GetConsensus(), block_hash, true);

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

    void UpsertInputs(const std::vector<CTxIn> vin, const unsigned int input_etransaction_id) {
        for(unsigned int index = 0; index < vin.size(); index++) {
            const CTxIn& input = vin[index];

            isminetype is_input_mine = vpwallets[0]->IsMine(input);

            CTransactionRef output_transaction;
            uint256 output_block_hash;
            bool found_output_transaction = GetTransaction(input.prevout.hash, output_transaction, Params().GetConsensus(), block_hash, true);
            const CTxOut& output = output_transaction->vout[input.prevout.n];

            unsigned int output_etransaction_id = UpsertTx(output_transaction, false);

            CTxDestination address;
            ExtractDestination(output.scriptPubKey, address);

            odb::transaction t(enterprise_database->begin());
            std::auto_ptr <eOutputEntries> eout(
                    enterprise_database->query_one<eOutputEntries>(output_query::input_etransaction_id == input_etransaction_id
                                                                   && output_query::input_vector == vin));
            if (eout.get() != 0) {
                eout->output_etransaction_id = output_etransaction_id;
                eout->output_vector = input.prevout.n;
                eout->is_output_mine = is_input_mine;
                eout->amount = output.nValue;
                eout->destination = EncodeDestination(sent_output.destination);
                enterprise_database->update(*eout);
            } else {
                eOutputEntries new_eout(
                        etransaction_id,
                        vout,
                        -sent_output.amount,
                        EncodeDestination(address)
                );
                enterprise_database->persist(new_eout);
            }
            t.commit();
        }
    }

    unsigned int UpsertTransaction(const CTransactionRef tx) {

        uint256 hash = tx.GetHash();
        boost::uuids::uuid wallet_id = GetWalletID();
        unsigned int block_id = GetBlockID(hash);
        std::string txid = hash.GetHex();

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eTransactions> query;
            typedef odb::query <eOutputEntries> output_query;
            unsigned int etransaction_id;

            LOCK(cs_main);

            odb::transaction t(enterprise_database->begin());
            // Select the transaction by its txid, insert if it's not found, update if it is
            std::auto_ptr <eTransactions> etx(enterprise_database->query_one<eTransactions>(query::txid == txid
                                                                                            && query::wallet_id ==
                                                                                               wallet_id));
            if (etx.get() != 0) {
                etx->block_id = block_id;
                etx->size = tx->GetTotalSize();
                enterprise_database->update(*etx);
                etransaction_id = etx->id;
            } else {
                eTransactions new_etx(block_id,
                                      wtx.nIndex,
                                      wtx.IsTrusted(),
                                      wtx.tx->GetTotalSize(),
                                      wtx.GetTxTime(),
                                      wtx.nTimeReceived,
                                      wtx.GetDebit(ISMINE_ALL),
                                      wtx.GetCredit(ISMINE_ALL),
                                      txid,
                                      wallet_id);
                etransaction_id = enterprise_database->persist(new_etx);
            }
            t.commit();
            return etransaction_id;
        }
    }

    void UpsertWalletTransaction(const CWalletTx &wtx) {
        LogPrintf("Upsert transaction %s \n", txid);

        uint256 hash = wtx.GetHash();
        std::string txid = hash.GetHex();

        boost::uuids::uuid wallet_id = GetWalletID();
        unsigned int block_id = GetBlockID(hash);

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eTransactions> query;
            typedef odb::query <eOutputEntries> output_query;
            unsigned int etransaction_id;

            LOCK(cs_main);

            odb::transaction t(enterprise_database->begin());
            // Select the transaction by its txid, insert if it's not found, update if it is
            std::auto_ptr <eTransactions> etx(enterprise_database->query_one<eTransactions>(query::txid == txid
                                                                                            && query::wallet_id == wallet_id));
            if (etx.get() != 0) {
                etx->block_id = block_id;
                etx->block_index = wtx.nIndex;
                etx->is_trusted = wtx.IsTrusted();
                etx->size = wtx.tx->GetTotalSize();
                etx->time = wtx.GetTxTime();
                etx->time_received = wtx.nTimeReceived;
                etx->debit = wtx.GetDebit(ISMINE_ALL);
                etx->credit = wtx.GetCredit(ISMINE_ALL);
                enterprise_database->update(*etx);
                etransaction_id = etx->id;
            } else {
                eTransactions new_etx(block_id,
                                      wtx.nIndex,
                                      wtx.IsTrusted(),
                                      wtx.tx->GetTotalSize(),
                                      wtx.GetTxTime(),
                                      wtx.nTimeReceived,
                                      wtx.GetDebit(ISMINE_ALL),
                                      wtx.GetCredit(ISMINE_ALL),
                                      txid,
                                      wallet_id);
                etransaction_id = enterprise_database->persist(new_etx);
            }
            t.commit();

            for(unsigned int vout = 0; vout < wtx.tx->vout.size(); vout++) {
                const CTxOut& output = wtx.tx->vout[vout];
                isminetype is_output_mine = vpwallets[0]->IsMine(output);
                CTxDestination address;
                ExtractDestination(output.scriptPubKey, address);

                odb::transaction t(enterprise_database->begin());
                std::auto_ptr <eOutputEntries> eout(
                        enterprise_database->query_one<eOutputEntries>(output_query::output_etransaction_id == etransaction_id
                                                                       && output_query::output_vector == vout));
                if (eout.get() != 0) {
                    eout->amount = output.nValue;
                    eout->destination = EncodeDestination(sent_output.destination);
                    enterprise_database->update(*eout);
                } else {
                    eOutputEntries new_eout(
                            etransaction_id,
                            vout,
                            output.nValue,
                            EncodeDestination(address)
                    );
                    enterprise_database->persist(new_eout);
                }
                t.commit();
            }


        }
    }

    void DeleteTx(uint256 &hash) {
        boost::uuids::uuid wallet_id = GetWalletID();

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eTransactions> query;

            odb::transaction t(enterprise_database->begin());
            std::auto_ptr <eTransactions> etx(
                    enterprise_database->query_one<eTransactions>(query::txid == hash.GetHex() && query::wallet_id == wallet_id));

            if (etx.get() != 0)
                enterprise_database->erase(*etx);
            t.commit();
        }
    }
}
