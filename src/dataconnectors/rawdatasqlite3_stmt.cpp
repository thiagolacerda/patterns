#include "rawdatasqlite3_stmt.h"

#include <sqlite3.h>

int RawDataSQLite3_stmt::getAsInt(int colIndex) const
{
    return std::stoi(std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_data, colIndex))));
}

std::string RawDataSQLite3_stmt::getAsString(int colIndex) const
{
    return reinterpret_cast<const char*>(sqlite3_column_text(m_data, colIndex));
}

double RawDataSQLite3_stmt::getAsDouble(int colIndex) const
{
    return sqlite3_column_double(m_data, colIndex);
}

uint32_t RawDataSQLite3_stmt::getAsUInt32_t(int colIndex) const
{
    return std::stoul(std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_data, colIndex))));
}

uint64_t RawDataSQLite3_stmt::getAsUInt64_t(int colIndex) const
{
    return std::stoull(std::string(reinterpret_cast<const char*>(sqlite3_column_text(m_data, colIndex))));
}

