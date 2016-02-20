#ifndef BERLINMODDECODER_H
#define BERLINMODDECODER_H

#include <string>
#include <vector>
#include "basegpsfiledecoder.h"

class BerlinMODDecoder : public BaseGPSFileDecoder {
public:
    std::string decoderName() override { return "BerlinMOD"; };

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit BerlinMODDecoder(const std::vector<std::string>& parameters)
        : BaseGPSFileDecoder(parameters, ',')
    {}
};

#endif  // BERLINMODDECODER_H
