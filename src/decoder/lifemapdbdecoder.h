#ifndef LIFEMAPDBDECODER_H
#define LIFEMAPDBDECODER_H

#include <cstdint>
#include <string>
#include <vector>
#include "databasedecoder.h"

class LifeMapDBDecoder : public DatabaseDecoder {
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
