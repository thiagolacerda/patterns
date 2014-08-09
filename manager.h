#ifndef Manager_h
#define Manager_h

#include "config.h"
#include "gridmanager.h"
#include "trajectorymanager.h"

class DatabaseDecoder;

class Manager {
public:
    Manager()
        : m_dbDecoder(nullptr)
        , m_gridManager(GridManager(Config::gridSize()))
    { }

    void start();

private:
    void retrieveData();
    DatabaseDecoder* m_dbDecoder;
    GridManager m_gridManager;
    TrajectoryManager m_trajectoryManager;
};

#endif // Manager_h
