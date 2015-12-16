#include "sqlitedbmanager.h"

#include <sqlite3.h>
#include <iostream>
#include <tuple>
#include <vector>
#include "databasedecoder.h"
#include "patterns.h"

bool SQLiteDBManager::connect(const std::string& address, const std::string& user, const std::string& password)
{
    UNUSED(user);
    UNUSED(password);

    if (m_db)
        disconnect();

    int rc = sqlite3_open(address.c_str(), &m_db);
    if (rc) {
        sqlite3_close(m_db);
        m_db = nullptr;
        return false;
    }

    return true;
}

void SQLiteDBManager::disconnect()
{
    if (!m_db)
        return;

    sqlite3_close(m_db);
    m_db = nullptr;
}

uint64_t SQLiteDBManager::retrievePoints(const std::string& query, uint64_t batchSize)
{
    if (!m_currentStatement && sqlite3_prepare_v2(m_db, query.c_str(), -1, &m_currentStatement, NULL) != SQLITE_OK) {
        std::cerr << "Could not executer sqlite3_prepare_v2" << std::endl;
        return 0;
    }

    uint64_t retrieved = 0;
    while (m_currentStatement && (batchSize == -1 || retrieved < batchSize)) {
        if (sqlite3_step(m_currentStatement) != SQLITE_ROW) {
            sqlite3_finalize(m_currentStatement);
            m_currentStatement = nullptr;
            continue;
        }

        m_decoder->decodeRow(m_currentStatement);
        retrieved++;
    }
    std::string error = sqlite3_errmsg(m_db);
    if (error != "not an error")
        std::cerr << query << " " << error << std::endl;

    return retrieved;
}

double SQLiteDBManager::getColumnAsDouble(void* row, int colIndex)
{
    sqlite3_stmt* statement = reinterpret_cast<sqlite3_stmt*>(row);
    return sqlite3_column_double(statement, colIndex);
}

std::string SQLiteDBManager::getColumnAsString(void* row, int colIndex)
{
    sqlite3_stmt* statement = reinterpret_cast<sqlite3_stmt*>(row);
    return reinterpret_cast<const char*>(sqlite3_column_text(statement, colIndex));
}

uint32_t SQLiteDBManager::getColumnAsUInt32(void* row, int colIndex)
{
    sqlite3_stmt* statement = reinterpret_cast<sqlite3_stmt*>(row);
    return stoul(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, colIndex))));
}

uint64_t SQLiteDBManager::getColumnAsUInt64(void* row, int colIndex)
{
    sqlite3_stmt* statement = reinterpret_cast<sqlite3_stmt*>(row);
    return stoull(std::string(reinterpret_cast<const char*>(sqlite3_column_text(statement, colIndex))));
}
