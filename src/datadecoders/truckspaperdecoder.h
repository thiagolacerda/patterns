#pragma once

#include "datadecoder.h"

class TrucksPaperDecoder : public DataDecoder {
public:
    TrucksPaperDecoder()
        : DataDecoder({})
    {
    }

    void decodeData(const RawData& data) override;
};

