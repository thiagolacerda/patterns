#ifndef LifeMapDBDecoder_h
#define LifeMapDBDecoder_h

#include "databasedecoder.h"

class LifeMapDBDecoder : public DatabaseDecoder {
public:
    unsigned long long retrievePoints() override;

protected:
    void doDecodeRow(void* row) override;

private:
    friend class Factory;
    LifeMapDBDecoder(const std::vector<std::string>& parameters);

    std::string m_path;
    std::string m_dbFilesPrefix;
    unsigned long m_currentId;
};

#endif // LifeMapDBDecoder_h
