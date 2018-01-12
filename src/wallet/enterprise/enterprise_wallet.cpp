#include <base58.h>
#include <util.h>
#include <wallet/wallet.h>

#include "wallet/enterprise/database.h"
#include "wallet/enterprise/models/addresses.h"
#include "wallet/enterprise/models/output_entries.h"
#include "wallet/enterprise/models/transactions.h"
#include "wallet/enterprise/models/wallets.h"

#include "wallet/enterprise/models/support/addresses-odb.hxx"
#include "wallet/enterprise/models/support/output_entries-odb.hxx"
#include "wallet/enterprise/models/support/transactions-odb.hxx"
#include "wallet/enterprise/models/support/wallets-odb.hxx"

#include <odb/database.hxx>
#include <odb/transaction.hxx>

#include <wallet/enterprise/enterprise_wallet.h>


namespace enterprise_wallet {

    void UpsertWallet() {
        CWalletDBWrapper& dbw = vpwallets[0]->GetDBHandle();
        CWalletDB walletdb(dbw);
        std::string name = dbw.GetName();
        std::string wallet_id = walletdb.ReadID();

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eWallets> query;
            odb::transaction t(enterprise_database->begin());
            std::auto_ptr <eWallets> r(enterprise_database->query_one<eWallets>(query::wallet_id == wallet_id));
            if (r.get() != 0) {
                r->name = name;
                enterprise_database->update(*r);
            } else {
                eWallets new_r(wallet_id, name);
                enterprise_database->persist(new_r);
            }
            t.commit();
        }
    }

    void TopUpAddressPool() {
        UpsertWallet();
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
            {
                typedef odb::query <address_stats> query;
                odb::transaction t (enterprise_database->begin ());
                address_stats as (enterprise_database->query_value<address_stats> (query::is_used == false));
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
        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eAddresses> query;
            odb::transaction t(enterprise_database->begin());
            std::auto_ptr <eAddresses> ea(enterprise_database->query_one<eAddresses>(query::p2pkh_address == p2pkh_address));
            if (ea.get() != 0) {
                ea->name = name;
                ea->purpose = purpose;
                enterprise_database->update(*ea);
            } else {
                eAddresses new_ea(p2pkh_address, sw_bech32_address, sw_p2sh_address, name, purpose, GetTimeMillis(), false);
                enterprise_database->persist(new_ea);
            }
            t.commit();
        }
    }

    void UpsertTx(const CWalletTx &wtx) {
        CAmount nFee;
        std::string strSentAccount;
        std::list <COutputEntry> listReceived;
        std::list <COutputEntry> listSent;
        isminefilter filter = ISMINE_ALL;
        wtx.GetAmounts(listReceived, listSent, nFee, strSentAccount, filter);

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eTransactions> query;
            typedef odb::query <eOutputEntries> output_query;
            unsigned int etransaction_id;
            uint256 hash = wtx.GetHash();
            std::string txid = hash.GetHex();

            odb::transaction t(enterprise_database->begin());
            // Select the transaction by its txid, insert if it's not found, update if it is
            std::auto_ptr <eTransactions> etx(enterprise_database->query_one<eTransactions>(query::txid == txid));
            if (etx.get() != 0) {
                etx->block_index = wtx.nIndex;
                etx->is_trusted = wtx.IsTrusted();
                etx->size = wtx.tx->GetTotalSize();
                etx->time = wtx.GetTxTime();
                etx->time_received = wtx.nTimeReceived;
                enterprise_database->update(*etx);
                etransaction_id = etx->id;
            } else {
                eTransactions new_etx(wtx.nIndex,
                                      wtx.IsTrusted(),
                                      wtx.tx->GetTotalSize(),
                                      wtx.GetTxTime(),
                                      wtx.nTimeReceived,
                                      txid);
                etransaction_id = enterprise_database->persist(new_etx);
            }
            t.commit();

            for (const COutputEntry &sent_output : listSent) {
                odb::transaction t(enterprise_database->begin());
                std::auto_ptr <eOutputEntries> eout(
                        enterprise_database->query_one<eOutputEntries>(output_query::etransaction_id == etransaction_id
                                                                       && output_query::vector == sent_output.vout));
                if (eout.get() != 0) {
                    eout->amount = -sent_output.amount;
                    eout->category = std::string("send");
                    eout->destination = EncodeDestination(sent_output.destination);
                    enterprise_database->update(*eout);
                } else {
                    eOutputEntries new_eout(
                            etransaction_id,
                            sent_output.vout,
                            -sent_output.amount,
                            std::string("send"),
                            EncodeDestination(sent_output.destination)
                    );
                    enterprise_database->persist(new_eout);
                }
                t.commit();
            }

            for (const COutputEntry &received_output : listReceived) {
                odb::transaction t(enterprise_database->begin());
                std::auto_ptr <eOutputEntries> eout(
                        enterprise_database->query_one<eOutputEntries>(output_query::etransaction_id == etransaction_id
                                                                       &&
                                                                       output_query::vector == received_output.vout));
                if (eout.get() != 0) {
                    eout->amount = received_output.amount;
                    eout->category = std::string("receive");
                    eout->destination = EncodeDestination(received_output.destination);
                    enterprise_database->update(*eout);
                } else {
                    eOutputEntries new_eout(
                            etransaction_id,
                            received_output.vout,
                            received_output.amount,
                            std::string("receive"),
                            EncodeDestination(received_output.destination)
                    );
                    enterprise_database->persist(new_eout);
                }
                t.commit();
            }

        }
    }

    void DeleteTx(uint256 &hash) {

        std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
        {
            typedef odb::query <eTransactions> query;

            odb::transaction t(enterprise_database->begin());
            std::auto_ptr <eTransactions> etx(
                    enterprise_database->query_one<eTransactions>(query::txid == hash.GetHex()));

            if (etx.get() != 0)
                enterprise_database->erase(*etx);
            t.commit();
        }
    }
}
