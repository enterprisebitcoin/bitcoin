#ifndef ENTERPRISE_BITCOIN_H
#define ENTERPRISE_BITCOIN_H

#include <chain.h>

typedef std::pair<CBlockIndex, CBlock> BlockData;

namespace enterprise_bitcoin
{
    void UpsertBlocks(const std::vector<BlockData> &blocks);
}

#endif //ENTERPRISE_BITCOIN_H
