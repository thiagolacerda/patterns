#ifndef LifeMapDBDecoder_h
#define LifeMapDBDecoder_h

#include "databasedecoder.h"

class LifeMapDBDecoder : public DatabaseDecoder {
public:
    void decodeRow(void* row) override;
    void retrievePoints() override;
    void insertTupleInResults(const std::tuple<long, double, double, unsigned long>&) override;

private:
    friend class Factory;
    LifeMapDBDecoder(const std::vector<std::string>& parameters);

    std::string m_path;
    std::string m_dbFilesPrefix;
    long m_currentId;
};

#endif // LifeMapDBDecoder_h
