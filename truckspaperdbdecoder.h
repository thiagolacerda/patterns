#ifndef TrucksPaperDBDecoder_h
#define TrucksPaperDBDecoder_h

#include "databasedecoder.h"

class TrucksPaperDBDecoder : public DatabaseDecoder {
public:
    void decodeRow(void* row) override;
    unsigned long long retrievePoints() override;

private:
    friend class Factory;
    TrucksPaperDBDecoder(const std::vector<std::string>& parameters);
    std::string m_path;
};

#endif // TrucksPaperDBDecoder_h
