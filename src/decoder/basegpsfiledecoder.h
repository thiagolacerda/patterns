#ifndef BASEGPSFILEDECODER_H
#define BASEGPSFILEDECODER_H

#include <string>
#include <vector>
#include "databasedecoder.h"

class BaseGPSFileDecoder : public DatabaseDecoder {
public:
    uint64_t numberOfRecords() override;

    void setPath(const std::string& path) { m_path = path; }

protected:
    uint64_t doRetrievePoints(int64_t batchSize) override;
    void connectToDB() override { m_manager->connect(m_path, "", ""); };

    explicit BaseGPSFileDecoder(const std::vector<std::string>& parameters, char separator);

private:
    std::string m_path;
};

#endif  // BASEGPSFILEDECODER_H
