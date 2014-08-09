#ifndef DBManager_h
#define DBManager_h

#include <string>

class DatabaseDecoder;

class DBManager {
public:
    DBManager()
        : m_decoder(nullptr)
    { }

    virtual ~DBManager() { }

    void setDecoder(DatabaseDecoder* decoder) { m_decoder = decoder; }
    virtual bool connect(const std::string& address, const std::string& user, const std::string& password) = 0;
    virtual void disconnect() = 0;
    virtual void executeSQLSelect(const std::string& query) = 0;
    virtual double getColumnAsDouble(void* row, int colIndex) = 0;
    virtual std::string getColumnAsString(void* row, int colIndex) = 0;

protected:
    DatabaseDecoder* m_decoder;
};

#endif // DBManager_h
