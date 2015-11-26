#ifndef LIFEMAPDBDECODER_H
#define LIFEMAPDBDECODER_H

#include <cstdint>
#include <string>
#include <vector>
#include "databasedecoder.h"

class LifeMapDBDecoder : public DatabaseDecoder {
public:
    std::string decoderName() override { return "LifeDB"; };
    uint64_t numberOfRecords() override { return 0; }; // TODO: return the correct number programmatically

protected:
    uint64_t doRetrievePoints() override;
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit LifeMapDBDecoder(const std::vector<std::string>& parameters);

    std::string m_path;
    std::string m_dbFilesPrefix;
    uint32_t m_currentId;
};

#endif  // LIFEMAPDBDECODER_H
