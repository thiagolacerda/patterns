#ifndef TrucksDBDecoder_h
#define TrucksDBDecoder_h

#include "databasedecoder.h"

class TrucksDBDecoder : public DatabaseDecoder {
public:
    void decodeRow(void* row) override;
    unsigned long long retrievePoints() override;

private:
    friend class Factory;
    TrucksDBDecoder(const std::vector<std::string>& parameters);
    std::string m_path;
};

#endif // TrucksDBDecoder_h
