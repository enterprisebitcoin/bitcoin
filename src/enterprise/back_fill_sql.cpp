#include <chainparams.h>

#include "enterprise/database.h"
#include "enterprise/back_fill_sql.h"
#include "enterprise/block_to_sql.h"

#include "enterprise/models/blocks.h"
#include "enterprise/models/blocks-odb.hxx"
#include "enterprise/models/transactions.h"
#include "enterprise/models/transactions-odb.hxx"

#include <odb/database.hxx>
#include <odb/result.hxx>
#include <odb/transaction.hxx>

#include <validation.h>
#include <validationinterface.h>

namespace {
    bool sortDesc(int i, int j) { return i > j; }
}

BackFillSql::BackFillSql(const int current_height, const int back_fill_depth) :
    m_current_height(current_height),
    m_back_fill_depth(back_fill_depth) {

    LOCK(cs_main);

    int target_height = m_current_height - m_back_fill_depth;
    std::auto_ptr <odb::database> enterprise_database(create_enterprise_database());

    typedef odb::result<missing_blocks> missing_blocks_result;

    odb::transaction t (enterprise_database->begin(), false);
    odb::transaction::current(t);

    missing_blocks_result r (enterprise_database->query<missing_blocks> (("SELECT generate_series(" + std::to_string(target_height) + ", " + std::to_string(m_current_height) +  ") AS height EXCEPT SELECT eb.height AS height FROM bitcoin.\"eBlocks\" eb;")));

    std::vector<int> block_heights;

    for (missing_blocks_result::iterator i (r.begin ()); i != r.end (); ++i) {
        const missing_blocks& mb (*i);
        block_heights.push_back(mb.height);
    }

    t.commit ();

    std::sort(block_heights.begin(), block_heights.end(), sortDesc);

    for (auto height: block_heights) {
      if (height > target_height) {
          CBlockIndex* pindex = chainActive[height];
          LogPrintf("Back filling %d %s\n", height, pindex->ToString());
          CBlock block;
          ReadBlockFromDisk(block, pindex, Params().GetConsensus());
          BlockToSql block_to_sql(*pindex, block);
          block_to_sql.InsertBlock();
      }
    }

}