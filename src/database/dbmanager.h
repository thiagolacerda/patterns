#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <string>
#include "patterns.h"

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
    virtual bool isConnected() = 0;
    virtual uint64_t retrievePoints(const std::string& query = "") = 0;
    virtual double getColumnAsDouble(void* row, int colIndex)
    {
        UNUSED(row);
        UNUSED(colIndex);
        return 0.0;
    }
    virtual uint32_t getColumnAsUInt32(void* row, int colIndex)
    {
        UNUSED(row);
        UNUSED(colIndex);
        return 0;
    }
    virtual uint64_t getColumnAsUInt64(void* row, int colIndex)
    {
        UNUSED(row);
        UNUSED(colIndex);
        return 0;
    }
    virtual int getColumnAsInt(void* row, int colIndex)
    {
        UNUSED(row);
        UNUSED(colIndex);
        return 0;
    }
    virtual std::string getColumnAsString(void* row, int colIndex)
    {
        UNUSED(row);
        UNUSED(colIndex);
        return "";
    }

protected:
    DatabaseDecoder* m_decoder;
};

#endif  // DBMANAGER_H
