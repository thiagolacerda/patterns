#ifndef SQLITEDBMANAGER_H
#define SQLITEDBMANAGER_H

#include <cstdint>
#include <string>
#include "dbmanager.h"

struct sqlite3;
struct sqlite3_stmt;

class SQLiteDBManager : public DBManager {
public:
    SQLiteDBManager()
        : DBManager()
        , m_db(nullptr)
        , m_currentStatement(nullptr)
    { }

    ~SQLiteDBManager() { disconnect(); }

    bool isConnected() override { return m_db && true; } // TODO: implement!!
    bool connect(const std::string& address, const std::string& user, const std::string& password) override;
    void disconnect() override;
    uint64_t retrievePoints(const std::string& query, int64_t batchSize) override;
    double getColumnAsDouble(void* row, int colIndex) override;
    std::string getColumnAsString(void* row, int colIndex) override;
    uint32_t getColumnAsUInt32(void* row, int colIndex) override;
    uint64_t getColumnAsUInt64(void* row, int colIndex) override;

private:
    sqlite3* m_db;
    sqlite3_stmt* m_currentStatement;
};

#endif  // SQLITEDBMANAGER_H
