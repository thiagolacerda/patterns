#pragma once

#include "datadecoder.h"

class CustomDecoder : public DataDecoder {
public:
    CustomDecoder()
        : DataDecoder({})
    {
    }

    void decodeData(const RawData& data) override;
};

