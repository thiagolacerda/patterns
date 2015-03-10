#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include <cstdint>
#include <string>
#include "dbmanager.h"

struct sqlite3;

class SQLiteDBManager : public DBManager {
public:
    SQLiteDBManager()
        : DBManager()
        , m_db(nullptr)
    { }

    ~SQLiteDBManager() { disconnect(); }

    bool connect(const std::string& address, const std::string& user, const std::string& password) override;
    void disconnect() override;
    uint64_t retrievePoints(const std::string& query) override;
    double getColumnAsDouble(void* row, int colIndex) override;
    std::string getColumnAsString(void* row, int colIndex) override;
    uint32_t getColumnAsUInt32(void* row, int colIndex) override;
    uint64_t getColumnAsUInt64(void* row, int colIndex) override;

private:
    sqlite3* m_db;
};

#endif  // SQLITEDBMANAGER_H
