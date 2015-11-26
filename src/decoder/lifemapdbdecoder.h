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
    void done() override;

    void setPath(const std::string& path) { m_path = path; }
    void setDBFilesPrefix(const std::string& prefix) { m_dbFilesPrefix = prefix; }

protected:
    uint64_t doRetrievePoints(int64_t batchSize) override;
    void doDecodeRow(void* row) override;
    void connectToDB() override;

private:
    friend class Factory;
    explicit LifeMapDBDecoder(const std::vector<std::string>& parameters);

    std::string m_path;
    std::string m_dbFilesPrefix;
    uint32_t m_currentId;
    uint32_t m_currentDbFileIndex;
    std::vector<std::string> m_dbFiles;
    std::string m_selectQuery;
};

#endif  // LIFEMAPDBDECODER_H
