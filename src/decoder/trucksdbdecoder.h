#ifndef TRUCKSDBDECODER_H
#define TRUCKSDBDECODER_H

#include <cstdint>
#include <string>
#include <vector>
#include "databasedecoder.h"

class TrucksDBDecoder : public DatabaseDecoder {
public:
    uint64_t retrievePoints() override;

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    explicit TrucksDBDecoder(const std::vector<std::string>& parameters);
    std::string m_path;
};

#endif  // TRUCKSDBDECODER_H
