#ifndef TDRIVEDECODER_H
#define TDRIVEDECODER_H

#include <string>
#include <vector>
#include "basegpsfiledecoder.h"

class TDriveDecoder : public BaseGPSFileDecoder {
public:
    std::string decoderName() override { return "TDrive"; };

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit TDriveDecoder(const std::vector<std::string>& parameters)
        : BaseGPSFileDecoder(parameters, ',')
    {}
};

#endif  // TDRIVEDECODER_H
