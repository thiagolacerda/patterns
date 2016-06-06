#pragma once

#include "datadecoder.h"

class BerlinMODDecoder : public DataDecoder {
public:
    BerlinMODDecoder()
        : DataDecoder({})
    {
    }

    ~BerlinMODDecoder() override{}

    void decodeData(const RawData& data) override;

    static BerlinMODDecoder* instance(const std::unordered_map<std::string, std::string>&)
    {
        return new BerlinMODDecoder();
    }
};

