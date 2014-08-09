#ifndef DatabaseDecoder_h
#define DatabaseDecoder_h

#include <string>
#include <vector>
#include <tuple>
#include "dbmanager.h"

class DBManager;

class DatabaseDecoder {
public:
    virtual void retrievePoints() = 0;
    virtual void decodeRow(void* row) = 0;

    std::vector<std::tuple<int, double, double, long>> results;

    virtual ~DatabaseDecoder()
    {
        if (m_manager)
            delete m_manager;
    }

protected:
    DatabaseDecoder(const std::vector<std::string>& parameters, DBManager* manager)
        : m_manager(manager)
    { }
    DBManager* m_manager;
};

#endif // DatabaseDecoder_h
