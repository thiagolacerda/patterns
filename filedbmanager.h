#ifndef FileDBManager_h
#define FileDBManager_h

#include <fstream>
#include "dbmanager.h"

class FileDBManager : public DBManager {
public:
    FileDBManager(char separator)
        : DBManager()
        , m_separator(separator)
    { }

    ~FileDBManager() { disconnect(); }

    bool connect(const std::string& address, const std::string& user, const std::string& password) override;
    void disconnect() override;
    unsigned long long retrievePoints(const std::string& query) override;
    double getColumnAsDouble(void* row, int colIndex) override;
    unsigned long getColumnAsUnsignedLong(void* row, int colIndex) override;
    std::string getColumnAsString(void* row, int colIndex) override;

private:
    std::string getColumnValue(const std::string& row, int colIndex);
    std::ifstream m_file;
    char m_separator;
};

#endif // FileDBManager_h
