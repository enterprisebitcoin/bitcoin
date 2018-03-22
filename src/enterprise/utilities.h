#ifndef ENTERPRISE_BITCOIN_UTILITIES_H
#define ENTERPRISE_BITCOIN_UTILITIES_H

struct EnterpriseDestination {
    std::string address;
    std::string source_type;
    std::string hex;
    unsigned int version;
    unsigned int length;
    unsigned char program[40];
};

class EnterpriseDestinationEncoder : public boost::static_visitor<EnterpriseDestination>
{
private:
    const CChainParams& m_params;

public:
    DestinationEncoder(const CChainParams& params) : m_params(params) {}

    std::string operator()(const CKeyID& id) const
    {
        EnterpriseDestination destination;
        destination.source_type = "CKeyID";
        destination.hex = id.ToString();

        std::vector<unsigned char> data = m_params.Base58Prefix(CChainParams::PUBKEY_ADDRESS);
        data.insert(data.end(), id.begin(), id.end());

        destination.address = EncodeBase58Check(data);
        return destination;
    }

    std::string operator()(const CScriptID& id) const
    {
        EnterpriseDestination destination;
        destination.source_type = "CScriptID";
        destination.hex = id.ToString();

        std::vector<unsigned char> data = m_params.Base58Prefix(CChainParams::SCRIPT_ADDRESS);
        data.insert(data.end(), id.begin(), id.end());
        destination.address = EncodeBase58Check(data);

        return destination;;
    }

    std::string operator()(const WitnessV0KeyHash& id) const
    {
        EnterpriseDestination destination;
        destination.source_type = "WitnessV0KeyHash";
        destination.hex = id.ToString();

        std::vector<unsigned char> data = {0};
        ConvertBits<8, 5, true>(data, id.begin(), id.end());
        destination.address = bech32::Encode(m_params.Bech32HRP(), data);

        return destination;
    }

    std::string operator()(const WitnessV0ScriptHash& id) const
    {
        EnterpriseDestination destination;
        destination.source_type = "WitnessV0ScriptHash";
        destination.hex = id.ToString();

        std::vector<unsigned char> data = {0};
        ConvertBits<8, 5, true>(data, id.begin(), id.end());
        destination.address = bech32::Encode(m_params.Bech32HRP(), data);

        return destination;
    }

    std::string operator()(const WitnessUnknown& id) const
    {
        EnterpriseDestination destination;
        destination.source_type = "WitnessUnknown";
        destination.version = id.version;
        destination.length = id.length;

        if (id.version < 1 || id.version > 16 || id.length < 2 || id.length > 40) {
            return destination;
        }

        std::vector<unsigned char> data = {(unsigned char)id.version};
        ConvertBits<8, 5, true>(data, id.program, id.program + id.length);
        destination.address = bech32::Encode(m_params.Bech32HRP(), data);

        return destination;
    }

    std::string operator()(const CNoDestination& no) const { return {}; }
};

#endif // ENTERPRISE_BITCOIN_UTILITIES_H