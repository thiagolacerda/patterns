#ifndef TRUCKSDBDECODER_H
#define TRUCKSDBDECODER_H

#include <string>
#include <vector>
#include "basegpsfiledecoder.h"

class TrucksDBDecoder : public BaseGPSFileDecoder {
public:
    std::string decoderName() override { return "TrucksDB"; };

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit TrucksDBDecoder(const std::vector<std::string>& parameters)
        : BaseGPSFileDecoder(parameters, ';')
    {}
};

#endif  // TRUCKSDBDECODER_H
