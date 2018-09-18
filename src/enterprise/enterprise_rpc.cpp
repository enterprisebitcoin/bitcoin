
#include <chain.h>
#include <univalue.h>
#include <utilstrencodings.h>
#include <validation.h>

#include <rpc/server.h>

#include <enterprise/enterprise_rpc.h>
#include <enterprise/back_fill_sql.h>

static UniValue backfill(const JSONRPCRequest& request)
{
    if (request.fHelp || request.params.size() != 1)
        throw std::runtime_error(
            "backfill \"back_fill_depth\"\n"
        );

    int back_fill_depth = request.params[0].get_int();
    const CBlockIndex* pindex;
    pindex = chainActive.Tip();

    UniValue ret(UniValue::VOBJ);
    BackFillSql(pindex->nHeight, back_fill_depth);
    ret.pushKV("success", true);
    return ret;

}

// clang-format off
static const CRPCCommand commands[] =
{ //  category              name                      actor (function)         argNames
  //  --------------------- ------------------------  -----------------------  ----------
    { "enterprise",          "backfill",               &backfill,               {"back_fill_depth"} },
};
// clang-format on

void RegisterEnterpriseRPCCommands(CRPCTable &t)
{
    for (unsigned int vcidx = 0; vcidx < ARRAYLEN(commands); vcidx++)
        t.appendCommand(commands[vcidx].name, &commands[vcidx]);
}