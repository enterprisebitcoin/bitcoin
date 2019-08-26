#include <primitives/block.h>
#include <chain.h>
#include <logging.h>

#include <enterprise/enterprise_notification_interface.h>
#include <enterprise/block_to_sql.h>

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
}

void EnterpriseNotificationInterface::BlockDisconnected(const std::shared_ptr<const CBlock> &block) {
    LogPrintf("BlockDisconnected %s\n", block->GetBlockHeader().GetHash().ToString());
}
