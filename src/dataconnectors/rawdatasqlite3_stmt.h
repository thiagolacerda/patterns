#pragma once

#include "rawdata.h"

struct sqlite3;
struct sqlite3_stmt;

class RawDataSQLite3_stmt : public RawData {
public:
    RawDataSQLite3_stmt(sqlite3_stmt* stmt)
        : m_data(stmt)
    {}

    ~RawDataSQLite3_stmt() override {}

    void setSQLite3_stmt(sqlite3_stmt* stmt) { m_data = stmt; }

    int getAsInt(int colIndex) const override;
    std::string getAsString(int colIndex) const override;
    double getAsDouble(int colIndex) const override;
    uint32_t getAsUInt32_t(int colIndex) const override;
    uint64_t getAsUInt64_t(int colIndex) const override;

private:
    sqlite3_stmt* m_data;
};
