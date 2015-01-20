#ifndef DatabaseDecoder_h
#define DatabaseDecoder_h

#include <string>
#include <vector>
#include "dbmanager.h"

class DBManager;
class GPSTupleListener;

class DatabaseDecoder {
public:
    virtual unsigned long long retrievePoints() = 0;
    virtual void decodeRow(void* row) = 0;
    void setGPSTupleListener(GPSTupleListener* listener) { m_listener = listener; }

    virtual ~DatabaseDecoder()
    {
        if (m_manager)
            delete m_manager;
    }

protected:
    DatabaseDecoder(DBManager* manager)
        : m_manager(manager)
    { }
    DBManager* m_manager;
    GPSTupleListener* m_listener;
};

#endif // DatabaseDecoder_h
