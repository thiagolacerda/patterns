#ifndef TRUCKSPAPERDBDECODER_H
#define TRUCKSPAPERDBDECODER_H

#include <string>
#include <vector>
#include "basegpsfiledecoder.h"

class TrucksPaperDBDecoder : public BaseGPSFileDecoder {
public:
    std::string decoderName() override { return "TrucksPaperDB"; };

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit TrucksPaperDBDecoder(const std::vector<std::string>& parameters)
        : BaseGPSFileDecoder(parameters, '\t')
    {}
};

#endif  // TRUCKSPAPERDBDECODER_H
