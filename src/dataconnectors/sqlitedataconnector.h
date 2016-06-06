#pragma once

#include <string>
#include "dataconnector.h"

struct sqlite3;
struct sqlite3_stmt;

class SQLiteDataConnector : public DataConnector {
public:
    SQLiteDataConnector(const std::unordered_map<std::string, std::string>& parameters);

    ~SQLiteDataConnector() override {}

    bool connect() override;
    void disconnect() override;
    bool isConnected() override;
    void retrieveData() override;

    void onDataProviderFinished() override {}

    static SQLiteDataConnector* instance(const std::unordered_map<std::string, std::string>& parameters)
    {
        return new SQLiteDataConnector(parameters);
    }

private:
    sqlite3* m_db;
    std::string m_address;
    std::string m_queryString;
};

