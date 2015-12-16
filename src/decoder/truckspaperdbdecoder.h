#ifndef TRUCKSPAPERDBDECODER_H
#define TRUCKSPAPERDBDECODER_H

#include <string>
#include <vector>
#include "databasedecoder.h"

class TrucksPaperDBDecoder : public DatabaseDecoder {
public:
    std::string decoderName() override { return "TrucksPaperDB"; };
    uint64_t numberOfRecords() override;

protected:
    uint64_t doRetrievePoints(int64_t batchSize) override;
    void doDecodeRow(void* row) override;
    void connectToDB() override { m_manager->connect(m_path, "", ""); };

private:
    friend class Factory;
    explicit TrucksPaperDBDecoder(const std::vector<std::string>& parameters);
    std::string m_path;
};

#endif  // TRUCKSPAPERDBDECODER_H
