#ifndef SQLiteDBManager_h
#define SQLiteDBManager_h

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
    void executeSQLSelect(const std::string& query) override;
    double getColumnAsDouble(void* row, int colIndex) override;
    std::string getColumnAsString(void* row, int colIndex) override;

private:
    sqlite3* m_db;
};

#endif // SQLiteDBManager_h
