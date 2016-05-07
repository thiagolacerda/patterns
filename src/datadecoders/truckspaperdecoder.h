#pragma once

#include "datadecoder.h"

class TrucksPaperDecoder : public DataDecoder {
public:
    TrucksPaperDecoder()
        : DataDecoder({})
    {
    }

    void decodeData(const RawData& data) override;

    static TrucksPaperDecoder* instance(const std::unordered_map<std::string, std::string>&)
    {
        return new TrucksPaperDecoder();
    }
};

