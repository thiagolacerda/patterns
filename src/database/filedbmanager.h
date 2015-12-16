#ifndef FILEDBMANAGER_H
#define FILEDBMANAGER_H

#include <cstdint>
#include <fstream>
#include <string>
#include "dbmanager.h"

class FileDBManager : public DBManager {
public:
    explicit FileDBManager(char separator)
        : DBManager()
        , m_separator(separator)
    { }

    ~FileDBManager() { disconnect(); }

    bool isConnected() override { return m_file.is_open(); }
    bool connect(const std::string& address, const std::string& user, const std::string& password) override;
    void disconnect() override;
    uint64_t retrievePoints(const std::string& query, uint64_t batchSize) override;
    double getColumnAsDouble(void* row, int colIndex) override;
    uint32_t getColumnAsUInt32(void* row, int colIndex) override;
    uint64_t getColumnAsUInt64(void* row, int colIndex) override;
    std::string getColumnAsString(void* row, int colIndex) override;

private:
    std::string getColumnValue(const std::string& row, int colIndex);
    std::ifstream m_file;
    char m_separator;
};

#endif  // FILEDBMANAGER_H
