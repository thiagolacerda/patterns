#include "sqlitedataconnector.h"

#include <iostream>
#include <sqlite3.h>
#include "componentfactory.h"
#include "rawdatasqlite3_stmt.h"

SQLiteDataConnector::SQLiteDataConnector(const std::unordered_map<std::string, std::string>& parameters)
    : DataConnector(parameters)
{
    m_address = parameters.at("address");
    auto iter = parameters.find("query");
    if (iter == parameters.end())
        m_queryString = "SELECT * FROM " + parameters.at("table");
    else
        m_queryString = iter->second;
}

bool SQLiteDataConnector::connect()
{
    if (m_db)
        disconnect();

    int rc = sqlite3_open(m_address.c_str(), &m_db);
    if (!rc)
        return true;

    // an error occurred
    sqlite3_close(m_db);
    m_db = nullptr;
    std::cerr << "Could not connect to " << m_address << std::endl;
    return false;
}

void SQLiteDataConnector::disconnect()
{
    if (!m_db)
        return;

    sqlite3_close(m_db);
    m_db = nullptr;

}

bool SQLiteDataConnector::isConnected()
{
    return m_db != nullptr;
}

void SQLiteDataConnector::retrieveData()
{
    if (!connect())
        return;

    sqlite3_stmt* currentStatement = nullptr;

    if (sqlite3_prepare_v2(m_db, m_queryString.c_str(), -1, &currentStatement, nullptr) != SQLITE_OK) {
        std::cerr << "Could not execute sqlite3_prepare_v2" << std::endl;
        return;
    }

    while (currentStatement) {
        if (sqlite3_step(currentStatement) != SQLITE_ROW) {
            sqlite3_finalize(currentStatement);
            currentStatement = nullptr;
            continue;
        }
        RawDataSQLite3_stmt data(currentStatement);
        sendToDecoders(data);
    }
    std::string error = sqlite3_errmsg(m_db);
    if (error != "not an error")
        std::cerr << m_queryString << " " << error << std::endl;

    sqlite3_finalize(currentStatement);
}

REGISTER_DATA_CONNECTOR("sqlite", "s", SQLiteDataConnector::instance);

