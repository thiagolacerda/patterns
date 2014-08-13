#include "sqlitedbmanager.h"

#include <iostream>
#include <sqlite3.h>
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

void SQLiteDBManager::retrievePoints(const std::string& query)
{
    sqlite3_stmt* selectStmt;
    if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &selectStmt, NULL) != SQLITE_OK) {
        std::cerr << "Could not executer sqlite3_prepare_v2" << std::endl;
        return;
    }

    while(true) {
        if(sqlite3_step(selectStmt) != SQLITE_ROW)
            break;

        m_decoder->decodeRow(selectStmt);
    }
    sqlite3_finalize(selectStmt);
    std::string error = sqlite3_errmsg(m_db);
    if(error != "not an error")
        std::cerr << query << " " << error << std::endl;
}

double SQLiteDBManager::getColumnAsDouble(void* row, int colIndex)
{
    sqlite3_stmt* statement = (sqlite3_stmt*) row;
    return sqlite3_column_double(statement, colIndex);
}

std::string SQLiteDBManager::getColumnAsString(void* row, int colIndex)
{
    sqlite3_stmt* statement = (sqlite3_stmt*) row;
    return reinterpret_cast<const char*>(sqlite3_column_text(statement, colIndex));
}
