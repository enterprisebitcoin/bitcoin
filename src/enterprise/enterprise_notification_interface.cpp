#include <primitives/block.h>
#include <chain.h>
#include <util.h>

#include <enterprise/enterprise_notification_interface.h>
#include <enterprise/enterprise_bitcoin.h>

void EnterpriseNotificationInterface::UpdatedBlockTip(const CBlockIndex *pindexNew,
                                                      const CBlockIndex *pindexFork,
                                                      bool fInitialDownload) {
    LogPrintf("UpdatedBlockTip pindexNew %s", pindexNew->ToString());
    LogPrintf("UpdatedBlockTip pindexFork %s", pindexFork->ToString());
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
    LogPrintf("BlockConnected %s", pindex->ToString());


    std::vector<BlockData> blocks;
    blocks.push_back(std::make_tuple(*pindex, *block));
    enterprise_bitcoin::ProcessBlocks(blocks);
}

void EnterpriseNotificationInterface::BlockDisconnected(const std::shared_ptr<const CBlock> &block) {
    LogPrintf("BlockDisconnected %s", block->GetBlockHeader().GetHash().ToString());
}
