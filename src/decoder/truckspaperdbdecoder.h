#ifndef TRUCKSPAPERDBDECODER_H
#define TRUCKSPAPERDBDECODER_H

#include <string>
#include <vector>
#include "databasedecoder.h"

class TrucksPaperDBDecoder : public DatabaseDecoder {
public:
    uint64_t retrievePoints() override;

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit TrucksPaperDBDecoder(const std::vector<std::string>& parameters);
    std::string m_path;
};

#endif  // TRUCKSPAPERDBDECODER_H
