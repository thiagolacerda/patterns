#ifndef BRINKHOFFMNTGDECODER_H
#define BRINKHOFFMNTGDECODER_H

#include <string>
#include <vector>
#include "basegpsfiledecoder.h"

class BrinkhoffMNTGDecoder : public BaseGPSFileDecoder {
public:
    std::string decoderName() override { return "BrinkhoffMNTG"; }

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit BrinkhoffMNTGDecoder(const std::vector<std::string>& parameters)
        : BaseGPSFileDecoder(parameters, ' ')
    {}
};

#endif  // BRINKHOFFMNTGDECODER_H
