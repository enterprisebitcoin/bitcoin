#ifndef ENTERPRISE_BITCOIN_H
#define ENTERPRISE_BITCOIN_H

#include <chain.h>

#include "enterprise/models/blocks.h"
#include "enterprise/models/outputs.h"
#include "enterprise/models/scripts.h"
#include "enterprise/models/transactions.h"

typedef std::pair <CBlockIndex, CBlock> BlockData;

namespace enterprise_bitcoin {
    void InsertBlocks(const std::vector <eBlocks> &records);

    void InsertTransactions(const std::vector <eTransactions> &records);

    void InsertOutputs(const std::vector <eOutputs> &records);

    void InsertScripts(const std::vector <eScripts> &records);

    void ProcessBlocks(const std::vector <BlockData> &blocks);
}

#endif //ENTERPRISE_BITCOIN_H
