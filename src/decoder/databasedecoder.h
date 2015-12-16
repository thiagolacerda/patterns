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
    virtual uint64_t numberOfRecords() = 0;

    virtual void done() {
        m_readRecords = 0;
        m_manager->disconnect();
    };

    void setGPSTupleListener(GPSTupleListener* listener) { m_listener = listener; }

    uint64_t retrievePoints(int64_t batchSize = -1)
    {
        if (!m_manager->isConnected())
            connectToDB();

        return doRetrievePoints(batchSize);
    }

    void decodeRow(void* row)
    {
        ++m_readRecords;
        if (Config::isInCompatibilityMode() && m_readRecords == 1)
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
        , m_readRecords(0)
    {
        m_manager->setDecoder(this);
    }
    virtual uint64_t doRetrievePoints(int64_t batchSize) = 0;
    virtual void doDecodeRow(void* row) = 0;
    virtual void connectToDB() = 0;
    DBManager* m_manager;
    GPSTupleListener* m_listener;
    uint64_t m_readRecords;
};

#endif  // DATABASEDECODER_H
