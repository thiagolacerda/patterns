#pragma once

#include "datadecoder.h"

class CustomDecoder : public DataDecoder {
public:
    CustomDecoder()
        : DataDecoder({})
    {
    }

    ~CustomDecoder() override {}

    void decodeData(const RawData& data) override;

    static CustomDecoder* instance(const std::unordered_map<std::string, std::string>&)
    {
        return new CustomDecoder();
    }
};

