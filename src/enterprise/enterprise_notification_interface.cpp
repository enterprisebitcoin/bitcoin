#include <primitives/block.h>
#include <chain.h>
#include <util.h>

#include <enterprise/enterprise_notification_interface.h>
#include <enterprise/block_to_sql.h>
#include <enterprise/prune_sql.h>
#include <enterprise/back_fill_sql.h>

void EnterpriseNotificationInterface::UpdatedBlockTip(const CBlockIndex *pindexNew,
                                                      const CBlockIndex *pindexFork,
                                                      bool fInitialDownload) {
    LogPrintf("UpdatedBlockTip pindexNew %s\n", pindexNew->ToString());
    if (pindexFork) {
        LogPrintf("UpdatedBlockTip pindexFork %s\n", pindexFork->ToString());
    }
}

void EnterpriseNotificationInterface::TransactionAddedToMempool(const CTransactionRef &ptxn) {
    ;
//    LogPrintf("TransactionAddedToMempool %s", ptxn->ToString());
}

void EnterpriseNotificationInterface::TransactionRemovedFromMempool(const CTransactionRef &ptx) {
    ;
//    LogPrintf("TransactionRemovedFromMempool %s", ptx->ToString());
}

void EnterpriseNotificationInterface::BlockConnected(const std::shared_ptr<const CBlock> &block,
                                                     const CBlockIndex *pindex,
                                                     const std::vector <CTransactionRef> &txnConflicted) {
    LogPrintf("BlockConnected %s\n", pindex->ToString());

    BlockToSql block_to_sql(*pindex, *block);
    block_to_sql.InsertBlock();

    int prune_depth = gArgs.GetArg("-enterprise_pruning", 4032);
    if (prune_depth) {
        PruneSql(pindex->nHeight, prune_depth);
    }

    int back_fill_depth = gArgs.GetArg("-enterprise_back_fill_depth", 4032);
    if (back_fill_depth) {
        BackFillSql(pindex->nHeight, back_fill_depth);
    }

}

void EnterpriseNotificationInterface::BlockDisconnected(const std::shared_ptr<const CBlock> &block) {
    LogPrintf("BlockDisconnected %s\n", block->GetBlockHeader().GetHash().ToString());
}
