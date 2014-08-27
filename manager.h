#ifndef Manager_h
#define Manager_h

#include <map>
#include "config.h"
#include "gpstuplelistener.h"
#include "gridmanager.h"
#include "trajectorymanager.h"

class DatabaseDecoder;

class Manager : public GPSTupleListener {
public:
    Manager()
        : m_dbDecoder(nullptr)
        , m_gridManager(GridManager(Config::gridSize()))
    { }

    void start();
    void processGPSTuple(const std::tuple<unsigned long, double, double, unsigned long>&) override;

private:
    void retrieveData();
    DatabaseDecoder* m_dbDecoder;
    GridManager m_gridManager;
    TrajectoryManager m_trajectoryManager;
    std::map<unsigned, std::vector<GPSPoint*>> m_pointsPerTimeSlot;
};

#endif // Manager_h
