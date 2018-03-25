#ifndef BITCOIN_ENTERPRISE_NOTIFICATION_INTERFACE_H
#define BITCOIN_ENTERPRISE_NOTIFICATION_INTERFACE_H

#include <validationinterface.h>

class EnterpriseNotificationInterface final : public CValidationInterface {
private:


    /**
     * Notifies listeners of updated block chain tip
     *
     * Called on a background thread.
     */
    void UpdatedBlockTip(const CBlockIndex *pindexNew,
                         const CBlockIndex *pindexFork,
                         bool fInitialDownload) {}


    /**
     * Notifies listeners of a transaction having been added to mempool.
     *
     * Called on a background thread.
     */
    void TransactionAddedToMempool(const CTransactionRef &ptxn) {}


    /**
     * Notifies listeners of a transaction leaving mempool.
     *
     * This only fires for transactions which leave mempool because of expiry,
     * size limiting, reorg (changes in lock times/coinbase maturity), or
     * replacement. This does not include any transactions which are included
     * in BlockConnectedDisconnected either in block->vtx or in txnConflicted.
     *
     * Called on a background thread.
     */
    void TransactionRemovedFromMempool(const CTransactionRef &ptx) {}


    /**
     * Notifies listeners of a block being connected.
     * Provides a vector of transactions evicted from the mempool as a result.
     *
     * Called on a background thread.
     */
    void BlockConnected(const std::shared_ptr<const CBlock> &block,
                        const CBlockIndex *pindex,
                        const std::vector <CTransactionRef> &txnConflicted) {}


    /**
     * Notifies listeners of a block being disconnected
     *
     * Called on a background thread.
     */
    void BlockDisconnected(const std::shared_ptr<const CBlock> &block) {}
};

#endif // BITCOIN_ENTERPRISE_NOTIFICATION_INTERFACE_H