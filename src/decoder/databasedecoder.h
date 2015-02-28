#ifndef DATABASEDECODER_H
#define DATABASEDECODER_H

#include <cstdint>
#include <string>
#include <vector>
#include "config.h"
#include "dbmanager.h"

class DBManager;
class GPSTupleListener;

class DatabaseDecoder {
public:
    virtual uint64_t retrievePoints() = 0;
    void setGPSTupleListener(GPSTupleListener* listener) { m_listener = listener; }
    void decodeRow(void* row)
    {
        if (m_ignoreFirstRow) {
            m_ignoreFirstRow = false;
            return;
        }

        doDecodeRow(row);
    }

    virtual ~DatabaseDecoder()
    {
        if (m_manager)
            delete m_manager;
    }

protected:
    explicit DatabaseDecoder(DBManager* manager)
        : m_manager(manager)
        , m_ignoreFirstRow(Config::isInCompatibilityMode())
    {
        m_manager->setDecoder(this);
    }
    virtual void doDecodeRow(void* row) = 0;
    DBManager* m_manager;
    GPSTupleListener* m_listener;
    bool m_ignoreFirstRow;  // compatibility mode
};

#endif  // DATABASEDECODER_H
