#ifndef TRUCKSDBDECODER_H
#define TRUCKSDBDECODER_H

#include <cstdint>
#include <string>
#include <vector>
#include "databasedecoder.h"

class TrucksDBDecoder : public DatabaseDecoder {
public:
    std::string decoderName() override { return "TrucksDB"; };
    uint64_t numberOfRecords() override;

protected:
    uint64_t doRetrievePoints(int64_t batchSize) override;
    void doDecodeRow(void* row) override;
    void connectToDB() override { m_manager->connect(m_path, "", ""); };

private:
    friend class Factory;
    explicit TrucksDBDecoder(const std::vector<std::string>& parameters);
    std::string m_path;
};

#endif  // TRUCKSDBDECODER_H