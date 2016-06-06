#pragma once

#include "datadecoder.h"

class BrinkhoffMNTGDecoder : public DataDecoder {
public:
    BrinkhoffMNTGDecoder()
        : DataDecoder({})
    {
    }

    ~BrinkhoffMNTGDecoder() override {}

    void decodeData(const RawData& data) override;

    static BrinkhoffMNTGDecoder* instance(const std::unordered_map<std::string, std::string>&)
    {
        return new BrinkhoffMNTGDecoder();
    }
};
