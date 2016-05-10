#pragma once

#include "datadecoder.h"

class TDriveDecoder : public DataDecoder {
public:
    TDriveDecoder()
        : DataDecoder({})
    {
    }

    void decodeData(const RawData& data) override;

    static TDriveDecoder* instance(const std::unordered_map<std::string, std::string>&)
    {
        return new TDriveDecoder();
    }
};

