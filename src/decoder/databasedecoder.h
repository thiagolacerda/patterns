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
    virtual std::string decoderName() = 0;

    void setGPSTupleListener(GPSTupleListener* listener) { m_listener = listener; }

    uint64_t retrievePoints()
    {
        m_readLines = 0;
        return doRetrievePoints();
    }

    void decodeRow(void* row)
    {
        ++m_readLines;
        if (Config::isInCompatibilityMode() && m_readLines == 1)
            return;

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
        , m_readLines(0)
    {
        m_manager->setDecoder(this);
    }
    virtual uint64_t doRetrievePoints() = 0;
    virtual void doDecodeRow(void* row) = 0;
    DBManager* m_manager;
    GPSTupleListener* m_listener;
    uint64_t m_readLines;
};

#endif  // DATABASEDECODER_H
