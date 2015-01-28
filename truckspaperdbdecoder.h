#ifndef TrucksPaperDBDecoder_h
#define TrucksPaperDBDecoder_h

#include "databasedecoder.h"

class TrucksPaperDBDecoder : public DatabaseDecoder {
public:
    unsigned long long retrievePoints() override;

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    TrucksPaperDBDecoder(const std::vector<std::string>& parameters);
    std::string m_path;
};

#endif // TrucksPaperDBDecoder_h
