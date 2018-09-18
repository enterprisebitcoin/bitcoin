
#include <enterprise/database.h>
#include "enterprise/prune_sql.h"

#include "enterprise/models/blocks.h"
#include "enterprise/models/blocks-odb.hxx"
#include "enterprise/models/transactions.h"
#include "enterprise/models/transactions-odb.hxx"

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>

PruneSql::PruneSql(const int current_height, const int prune_depth) :
    m_current_height(current_height),
    m_prune_depth(prune_depth) {

    int target_height = m_current_height - m_prune_depth;
    std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());
    typedef odb::query<block_hash> block_hash_query;
    typedef odb::result<block_hash> block_hash_result;

    typedef odb::query<eTransactions> etransactions_query;
    typedef odb::query<eBlocks> eblocks_query;

    odb::transaction t (enterprise_database->begin(), false);
    odb::transaction::current(t);

    block_hash_result r (enterprise_database->query<block_hash> (block_hash_query::height < target_height));

    for (block_hash_result::iterator i (r.begin ()); i != r.end (); ++i)
    {
      const block_hash& bh (*i);
      enterprise_database->erase_query<eTransactions> (etransactions_query::block_hash == bh.hash);
      enterprise_database->erase_query<eBlocks> (eblocks_query::hash == bh.hash);
    }

    t.commit ();

}