#ifndef SORTEDFILEDECODER_H
#define SORTEDFILEDECODER_H

#include <string>
#include <vector>
#include "basegpsfiledecoder.h"

class SortedFileDecoder : public BaseGPSFileDecoder {
public:
    std::string decoderName() override { return "SortedFile"; };

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit SortedFileDecoder(const std::vector<std::string>& parameters)
        : BaseGPSFileDecoder(parameters, ';')
    {}
};

#endif  // SORTEDFILEDECODER_H
